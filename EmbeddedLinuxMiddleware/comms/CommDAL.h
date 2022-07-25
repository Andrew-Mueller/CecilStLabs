#ifndef COMMDAL_H_
#define COMMDAL_H_

#include "../sqlite/sqlite3.h"

namespace CecilStLabs
{

   /**
    * Data Access Layer to the sqlite system for persistent storage of data in the system.
    *
    * Implemented using a single table, COMMQUEUE:
    * ------------------------
    * |PK | ID                |
    * |   | URL               |
    * |   | message           |
    * |   | comm_msg_id  |
    * |   | timestamp         |
    * |   | CRC16             |
    * ------------------------
    *
    * TODO: this data access layer is currently implemented using a combination of
    *       sqlite's callback (sqlite3_exec) and the inline prepare, step finalize.
    *
    *       This should be refactored to remove the callback usage and consistently
    *       only use the prepare, step, and finalize.
    */
   class CommDAL : public ILoggable
   {
      public:
         /**
          * Open the database file with the specified file name.
          *
          * @param dbFilename Filename of the database file to open.
          */
         CommDAL(std::string dbFilename);
         virtual ~CommDAL();

         /**
          * Inserts the specified communication record into the database.
          *
          * @param record new message to add to the database.
          *
          * @return True if the record was properly inserted.  False if it could
          *         not be added.
          */
         bool insert(CommRecord& record);

         /**
          * Removes the specified communication record from the database.
          *
          * @param id The record id to remove from the database.
          *
          * @return True if the data was properly removed, False if an error occurred.
          */
         bool remove(uint32_t id);

         /**
          * Retrieves the OLDEST record based on the datetime of the record.
          *
          * @return The Communication Record found based on the selection criteria.
          *         NULL if no records exist in the database.
          */
         CommRecord* getOldestRecord();

         /**
          * When a select action completes, this method is called with the record
          * returned.
          * Needs to be public because it is called from a non-member function
          * (SQLite rules).
          *
          * @param numColumns The number of columns returned in the record.
          * @param columnTexts The text of the column returned
          * @param columnNames The name of the columns returned.
          */
         void SelectCallback(int numColumns, char** columnTexts, char** columnNames);

         /**
          * Called when the SQL completes creating the table within our database.
          */
         void TableCreationCallback();

      protected:

         /**
          * sql statement for table creation.
          */
         static const std::string CREATE_TABLE_SQL;

         /**
          * sql statement for checking if the table was created.
          */
         static const std::string QUERY_TABLE_CREATION;

         /**
          * sql statement for inserting communication records into the database.
          */
         static const std::string INSERT_SQL;

         /**
          * sql statement for deleting communication records from the database.
          */
         static const std::string DELETE_SQL;

         /**
          * sql statement for removing sql statements from the database.
          */
         static const std::string SELECT_SQL;

         // Following are all of the strings that represent the columns of the
         // database table.
         static const std::string ID_COLUMN_STR;
         static const std::string URL_COLUMN_STR;
         static const std::string METHOD_COLUMN_STR;
         static const std::string MESSAGE_COLUMN_STR;
         static const std::string COMM_MSG_ID_STR;
         static const std::string TIMESTAMP_COLUMN_STR;
         static const std::string CRC16_COLUMN_STR;

         /**
          * Opens the persistent storage, sqlite database and creates it if it
          * does not already exist.
          */
         void openDatabase();

         /**
          * Queries the database to see if the table was created successfully.
          *
          * @returns True if the database table was created, False if it was not.
          */
         bool isTableCreated();

         /**
          * Creates the table in the database if it has not already been created.
          */
         void createTable();

         /**
          * The filename of the database file.
          */
         std::string m_databaseFilename;

         /**
          * instance of the database we are operating on.
          */
         sqlite3* m_database;

         /**
          * semaphore to ensure only one process can access the database at a once.
          */
         pthread_mutex_t m_databaseSemaphore;

         /**
          * internal creation flag if the database has been already created.
          */
         bool m_tableCreated;

         /**
          * currently retrieved record from the database.
          */
         CommRecord m_currentRecord;
   };

   /**
    * Non-member function used for the sqlite3 callback method.
    * SQLite3 execution requires a non-class member function as a startup (c style).
    * This non-member function is passed an instance of the CommDAL object.
    * The passed object's member function ::SQLCallback() method is called
    * to perform the work.
    *
    * @param CommDAL object to call back to the select method.
    * @param numColumns The number of columns returned in the record.
    * @param columnTexts The text of the column returned
    * @param columnNames The name of the columns returned.
    */
   int SQLSelectCallback(void* arg, int numColumns, char** columnTexts, char** columnNames);
   int SQLTableCreatedCallback(void* arg, int numColumns, char** columnTexts, char** columnNames);

}
#endif
