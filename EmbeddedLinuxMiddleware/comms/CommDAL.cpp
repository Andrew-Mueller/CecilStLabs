#include <cstdio>
#include <cstdlib>
#include "../sqlite/sqlite3.h"

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
using namespace std;

#include "../common/basicTypes.h"               //lint !e761
#include "../common/util/EnumByName.h"
#include "../common/DateTime/DateTime.h"
#include "../common/util/BasicUtil.h"

#include "../common/logging/LogEntry.h"
#include "../common/logging/ILogDriver.h"
#include "../common/logging/emptyLogDriver.h"
#include "../common/logging/ILoggable.h"

#include "CommRecord.h"
#include "CommDAL.h"

// change this to debug
#undef DAL_DEBUG_PRINT

namespace CecilStLabs
{
   const std::string CommDAL::ID_COLUMN_STR = "ID";
   const std::string CommDAL::URL_COLUMN_STR = "URL";
   const std::string CommDAL::METHOD_COLUMN_STR = "METHOD";
   const std::string CommDAL::MESSAGE_COLUMN_STR = "MESSAGE";
   const std::string CommDAL::COMM_MSG_ID_STR = "COMM_MSG_ID";
   const std::string CommDAL::TIMESTAMP_COLUMN_STR = "TIMESTAMP";
   const std::string CommDAL::CRC16_COLUMN_STR = "CRC16";

   const std::string CommDAL::QUERY_TABLE_CREATION = "SELECT name FROM sqlite_master WHERE type=\'table\' AND name=\'COMMQUEUE\';";
   const std::string CommDAL::CREATE_TABLE_SQL = "CREATE TABLE COMMQUEUE (ID INTEGER PRIMARY KEY, URL TEXT, METHOD INTEGER, MESSAGE TEXT, COMM_MSG_ID INTEGER, TIMESTAMP TEXT, CRC16 INTEGER);";

   const std::string CommDAL::INSERT_SQL = "INSERT INTO COMMQUEUE (URL, METHOD, MESSAGE, COMM_MSG_ID, TIMESTAMP, CRC16) VALUES(?, ?, ?, ?, ?, ?);";
   const std::string CommDAL::DELETE_SQL = "DELETE FROM COMMQUEUE WHERE ID = ?;";
   const std::string CommDAL::SELECT_SQL = "SELECT ID, URL, METHOD, MESSAGE, COMM_MSG_ID, TIMESTAMP, CRC16 FROM COMMQUEUE ORDER BY TIMESTAMP LIMIT 1;";

   CommDAL::CommDAL(std::string dbFilename)
      : m_databaseFilename(dbFilename),
        m_database(NULL),
        m_databaseSemaphore(),
        m_tableCreated(false)
   {
      // expect the database filename not to be empty
      BasicUtil::Assert(!BasicUtil::trim(dbFilename).empty(), __FILE__, __LINE__);

      // setup the semaphore to protect the database in from multi-thread usage.
      BasicUtil::Assert((0 == pthread_mutex_init(&m_databaseSemaphore, NULL)), __FILE__, __LINE__);

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_databaseSemaphore))
      {
         // open up the SQLite database file for use.
         openDatabase();

         // create the table in the database if it doesn't already exist.
         createTable();

         pthread_mutex_unlock(&m_databaseSemaphore);
      }
   }

   CommDAL::~CommDAL()
   {
      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_databaseSemaphore))
      {
         if (NULL != m_database)
         {
            BasicUtil::Assert((SQLITE_OK == sqlite3_close(m_database)), __FILE__, __LINE__);
         }

         BasicUtil::Assert((0 == pthread_mutex_unlock(&m_databaseSemaphore)), __FILE__, __LINE__);

         BasicUtil::Assert((0 == pthread_mutex_destroy(&m_databaseSemaphore)), __FILE__, __LINE__);
      }
   }

   bool CommDAL::insert(CommRecord& record)
   {
      bool inserted = false;

      // The documentation implies that sqlite simply points this to a position
      // within the prepared statement.
      // Therefore we do not need to worry about unallocated memory used by sqlite here.
      const char* unusedStatementPortion;

      sqlite3_stmt* insertStatement = NULL;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_databaseSemaphore))
      {
         int result = sqlite3_prepare_v2(m_database,
                                      INSERT_SQL.c_str(),
                                      INSERT_SQL.length() + 255,  // note this is the MAXIMUM number of bytes in the statement after parameters are added.
                                      &insertStatement,
                                      &unusedStatementPortion);

         if ((SQLITE_OK == result) &&
             (NULL != insertStatement))
         {
            int paramIndex = 0;

            sqlite3_bind_text(insertStatement,
                              ++paramIndex,
                              record.getURL().c_str(),
                              record.getURL().length(),
                              0);

            sqlite3_bind_int(insertStatement,
                              ++paramIndex,
                              (int)record.getMethod());

            sqlite3_bind_text(insertStatement,
                              ++paramIndex,
                              record.getMessage().c_str(),
                              record.getMessage().length(),
                              0);

            sqlite3_bind_int(insertStatement,
                             ++paramIndex,
                             (int)record.getMessageID());


            sqlite3_bind_text(insertStatement,
                              ++paramIndex,
                              record.getTimestamp().toUTCString().c_str(),
                              record.getTimestamp().toUTCString().length(),
                              0);

            sqlite3_bind_int(insertStatement,
                             ++paramIndex,
                             (int)(record.getCRC16()));

            // execute the insert statement.
            result = sqlite3_step(insertStatement);

            if (SQLITE_DONE != result)
            {
                  // an error occurred during execution of the sql statement.

                  std::stringstream errorMessage("Sqlite Error Inserting a new record: ");
                  errorMessage << result;
                  getLogDriver()->log(errorMessage.str(), LoggingError);
            }
            else
            {
               inserted = true;
            }
         }
         else
         {
            getLogDriver()->log(string("error preparing statement."), LoggingError);
         }

         pthread_mutex_unlock(&m_databaseSemaphore);
      }

      sqlite3_finalize(insertStatement);

      return inserted;
   }

   bool CommDAL::remove(uint32_t id)
   {
      bool removed = false;

      // The documentation implies that sqlite simply points this to a position
      // within the prepared statement.
      // Therefore we do not need to worry about unallocated memory used by sqlite here.
      const char* unusedStatementPortion;

      sqlite3_stmt* deleteStatement = NULL;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_databaseSemaphore))
      {
         int result = sqlite3_prepare_v2(m_database,
                                         DELETE_SQL.c_str(),
                                         DELETE_SQL.length() + 255,
                                         &deleteStatement,
                                         &unusedStatementPortion);

         if ((SQLITE_OK == result) &&
             (NULL != deleteStatement))
         {
            sqlite3_bind_int(deleteStatement,
                             1,
                             (int)id);

            result = sqlite3_step(deleteStatement);

            if (SQLITE_DONE != result)
            {
                  // an error occurred during execution of the sql statement.

                  stringstream errorMessage("Sqlite Error Deleting a record: ");
                  errorMessage << result;
                  getLogDriver()->log(errorMessage.str(), LoggingError);
            }
            else
            {
               removed = true;
            }
         }

         pthread_mutex_unlock(&m_databaseSemaphore);
      }

      sqlite3_finalize(deleteStatement);

      return removed;
   }

   CommRecord* CommDAL::getOldestRecord()
   {
      CommRecord* currRecord = NULL;

      if (MUTEX_SUCCESS == pthread_mutex_lock(&m_databaseSemaphore))
      {
         char* errorMessage = NULL;

         // zero the current record before attempting to retrieve latest records.
         m_currentRecord.zero();

         // NOTE: the callback is not actually called when no records are retrieved.
         int status = sqlite3_exec(m_database,
                                   SELECT_SQL.c_str(),
                                   SQLSelectCallback,
                                   this,
                                   &errorMessage);

         if (SQLITE_OK != status)
         {
            if (NULL != errorMessage)
            {
               // an error occurred during execution of the sql statement.

               string errorLogMessage("Sqlite select error: ");
               errorLogMessage.append(errorMessage);
               getLogDriver()->log(errorLogMessage, LoggingError);

               // we need to free the error message that was allocated inside of
               // the sqlite exec call when an error occurs.
               sqlite3_free((void*)errorMessage);
            }
         }
         else
         {
            CommRecord zeroRecord;
            zeroRecord.zero();

            if (m_currentRecord != zeroRecord)
            {
               currRecord = &m_currentRecord;
            }
         }

         pthread_mutex_unlock(&m_databaseSemaphore);
      }

      return currRecord;
   }

   void CommDAL::openDatabase()
   {
      // NOTE: we don't protect access to the database here using the mutex
      //       because the mutex protection is retrieved one level higher in
      //       the function that is calling these methods.

      int status = sqlite3_open(m_databaseFilename.c_str(), &m_database);

      if (status)
      {
         if (NULL != m_database)
         {
            BasicUtil::Assert((SQLITE_OK == sqlite3_close(m_database)), __FILE__, __LINE__);
         }

         // database could not be opened for use.
         BasicUtil::Assert(false, __FILE__, __LINE__);
      }

      // ensure the database object is valid.
      BasicUtil::Assert((SQLITE_OK == status), __FILE__, __LINE__);
   }

   bool CommDAL::isTableCreated()
   {
      char* errorMessage = NULL;

      // NOTE: we don't protect access to the database here using the mutex
      //       because the mutex protection is retrieved one level higher in
      //       the function that is calling these methods.

      // check if the table needs to be created
      // TODO: replace the use of sqlite3_exec and the callbacks to the use
      //       of sqlite3_prepare(), sqlite3_bind_*(), sqlite3_step(), and sqlite3_finalize()
      //       to be consistent
      int status = sqlite3_exec(m_database,
                                QUERY_TABLE_CREATION.c_str(),
                                SQLTableCreatedCallback,
                                this,
                                &errorMessage);

      if (SQLITE_OK != status)
      {
         if (NULL != errorMessage)
         {
            // an error occurred during execution of the sql statement.

            string errorLogMessage("table query error: ");
            errorLogMessage.append(errorMessage);
            getLogDriver()->log(errorLogMessage, LoggingError);

            // we need to free the error message that is allocated inside
            // sqlite3_exec() when an error occurs.
            sqlite3_free((void*)errorMessage);
         }
      }

      return m_tableCreated;
   }

   void CommDAL::createTable()
   {
      char* errorMessage = NULL;

      if (!isTableCreated())
      {
         // NOTE: we don't protect access to the database here using the mutex
         //       because the mutex protection is retrieved one level higher in
         //       the function that is calling these methods.

         // create the database if it does not already exist
         int status = sqlite3_exec(m_database, CREATE_TABLE_SQL.c_str(), NULL, NULL, &errorMessage);

         if (SQLITE_OK == status)
         {
            if (NULL != errorMessage)
            {
               // an error occurred during execution of the sql statement.

               string errorLogMessage("table creation error: ");
               errorLogMessage.append(errorMessage);
               getLogDriver()->log(errorLogMessage, LoggingError);

               sqlite3_free((void*)errorMessage);
            }
         }
      }
   }

   void CommDAL::SelectCallback(int numColumns, char** columnTexts, char** columnNames)
   {
      if (numColumns > 0)
      {
         // iterate over all of the columns and fill in the currently selected
         // record.
         for(int i = 0; i < numColumns; i++)
         {
            if (ID_COLUMN_STR == columnNames[i])
            {
               m_currentRecord.setID((uint32_t)atoi(columnTexts[i]));
            }
            else if (URL_COLUMN_STR == columnNames[i])
            {
               m_currentRecord.setURL(string(columnTexts[i]));
            }
            else if (METHOD_COLUMN_STR == columnNames[i])
            {
               m_currentRecord.setMethod((HTTP_Method_e)(uint32_t)atoi(columnTexts[i]));
            }
            else if (MESSAGE_COLUMN_STR == columnNames[i])
            {
               m_currentRecord.setMessage(string(columnTexts[i]));
            }
            else if (COMM_MSG_ID_STR == columnNames[i])
            {
               m_currentRecord.setCommMessageID((eCommMessage)(uint32_t)atoi(columnTexts[i]));
            }
            else if (TIMESTAMP_COLUMN_STR == columnNames[i])
            {
               // NOTE: the timestamp is simply stored in the database as a UTC time string.
               DateTime timestamp;
               timestamp.fromTimeString(string(columnTexts[i]));

               m_currentRecord.setTimestamp(timestamp);
            }
            else if (CRC16_COLUMN_STR == columnNames[i])
            {
               m_currentRecord.setCRC16((uint16_t)atoi(columnTexts[i]));
            }
         }
      }
      else
      {
         // in the case no records are recorded, make sure to set the current
         // record to an empty record so the previously recorded value is not
         // used again.
         m_currentRecord.zero();
      }
   }

   void CommDAL::TableCreationCallback()
   {
      m_tableCreated = true;
   }

   int SQLSelectCallback(void* arg, int numColumns, char** columnTexts, char** columnNames)
   {
      if (NULL != arg)
      {
         CommDAL* commDAL = (CommDAL*)arg;

         // call back into the object to properly encapsulate this result.
         commDAL->SelectCallback(numColumns, columnTexts, columnNames);
      }

		#ifdef DAL_DEBUG_PRINT
		{
			  stringstream columnStr;

			  // example to simply print the resulting records selected.
			  for(int i = 0; i < numColumns; i++)
			  {
				 columnStr << columnNames[i] << " = " << columnTexts[i] << endl;
			  }

			  getLogDriver()->log(columnStr.str(), LoggingDebug);
		}
		#endif

      // TODO: i'm not sure what the SQLite expectation for return value is here.
      return 0;
   }

   int SQLTableCreatedCallback(void* arg, int numColumns, char** columnTexts, char** columnNames)
   {
      if (NULL != arg)
      {
         CommDAL* commDAL = (CommDAL*)arg;

         if (numColumns > 0)
         {
            commDAL->TableCreationCallback();
         }
      }

      // TODO: i'm not sure what the SQLite expectation for return value is here.
      return 0;
   }
}
