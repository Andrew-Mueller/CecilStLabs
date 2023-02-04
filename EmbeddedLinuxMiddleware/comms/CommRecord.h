#ifndef COMMRECORD_H_
#define COMMRECORD_H_

namespace CecilStLabs
{
   /**
    * Contents stored when a message waiting to be transmitted is created.
    */
   class CommRecord
   {
      public:

         /**
          * Create an empty communication record.
          */
         CommRecord();

         /**
          * Create the communication record with the specified message and timestamp.
          *
          * @param method The communication method used for sending the request.
          * @param url The URL to post the message to.
          * @param message Data of the communication record.
          * @param timestamp Current date and time when the record is created.
          */
         CommRecord(HTTP_Method_e method,
                    std::string url,
                    std::string message,
                    uint32_t messageID,
                    DateTime timestamp);

         /**
          * release resources consumed by the record.
          */
         virtual ~CommRecord();

         /**
          * Comparison not equal to operator for the communication records.
          *
          * @param record CommRecord to compare against.
          *
          * @return True if the two communication records contain the same data,
          *         False if they do not.
          */
         bool operator!=(const CommRecord &record) const;

         /**
          * Comparison equal to operator for the communication records
          *
          * @param record CommRecord to compare against.
          *
          * @return True if the two communication records contains the same data,
          *         False if they do not match.
          */
         bool operator==(const CommRecord &record) const;

         /**
          * sets all of the members of the comm record class to their default
          * values.
          */
         void zero();

         // getters for all of the data members.
         inline uint32_t getID() { return m_id; };
         inline HTTP_Method_e getMethod() { return m_method; };
         inline std::string getURL() { return m_url; };
         inline std::string getMessage() { return m_message; };
         inline uint32_t getMessageID() { return m_messageID; };
         inline DateTime getTimestamp() { return m_timestamp; };
         inline uint16_t getCRC16() { return m_CRC16; };

         // setters for all of the data members.
         inline void setID(uint32_t id) { m_id = id; calculateCRC(); };
         inline void setMethod(HTTP_Method_e method) { m_method = method; calculateCRC(); };
         inline void setURL(std::string url) { m_url = url; calculateCRC();};
         inline void setMessage(std::string message) { m_message = message; calculateCRC();};
         inline void setMessageID(uint32_t messageID) { m_messageID = messageID; calculateCRC();};
         inline void setTimestamp(DateTime timestamp) { m_timestamp = timestamp; calculateCRC();};
         inline void setCRC16(uint16_t crc) { m_CRC16 = crc; calculateCRC();};

      protected:

         // NOTE: define the copy constructor and the assignment operator to ensure
         //       the compiler does not define it for us.
         CommRecord(CommRecord* copy __attribute__((unused)))
            : m_id(0),
              m_method(POST),
              m_url(""),
              m_message(""),
              m_messageID(0),
              m_timestamp(),
              m_CRC16(0)
         {};

         /**
          * Define the assignment operator to ensure a record cannot be created
          * from another record by assignment.

          * @param copy pointer to another CommRecord to copy from.
          *
          * @return CommRecord containing the same data as the specified "copy", CommRecord parameter.
          */
         CommRecord* operator=(CommRecord* copy __attribute__((unused))) { return NULL; };

         /**
          * calculates the CRC using all of the current member variable values.
          * This needs to be called when any of the values change.
          */
         void calculateCRC();

         /**
          * unique identifier of the message used by the data store where messages
          * exist while waiting for transfer.
          */
         uint32_t m_id;

         /**
          * communication method enumeration used for sending the message.
          */
         HTTP_Method_e m_method;

         /**
          * The destination that the message is going to.
          */
         std::string m_url;

         /**
          * message intended to be communicated.
          */
         std::string m_message;

         /**
          * unique identifier for the message
          */
         uint32_t m_messageID;

         /**
          * timestamp when the record is created.
          */
         DateTime m_timestamp;

         /**
          * checksum of the message and timestamp fields of the data.
          */
         uint16_t m_CRC16;
   };

}
#endif
