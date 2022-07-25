#ifndef BASICUTIL_H_
#define BASICUTIL_H_

namespace CecilStLabs
{
   class ILogDriver;

   /**
    * Utility class used to encapsulate items that don't belong anywhere else.
    */
   class BasicUtil
   {
      public:

         /**
          * Custom assert handler.
          *
          * @param conditional If the condition evaluates to false, the assert
          *                    will trap the application and log an error.
          * @param filename The name of the file that is executing the assert.
          * @param lineNumber The line of code where the assert is evaluated.
          */
         static void Assert(bool conditional,
                            const char* filename,
                            uint16_t lineNumber);

         /**
          * Assert if the architecture is not the correct Endian-ness.
          *
          * @param filename The file that is executing this big endian check.
          * @param lineNumber The line number for this big endian check.
          */
         static void AssertBigEndian(const char* filename, uint16_t lineNumber);
         static void AssertLittleEndian(const char* filename, uint16_t lineNumber);

         /**
          * Swaps the position of the high byte and the low byte within the specified
          * 2 byte word.  This is most often used within communication protocols
          * that are big endian while the architecture of the system is little
          * endian (or vice versa).  Bytes are received in the opposite order than
          * they are expected so when cast into a data structure, the order of the
          * bytes in any non-single byte member need to be switched.
          *
          * TODO: does this need to support larger types single word values?
          *       For example, will we ever need to swap in order all of the bytes
          *       in a MAC address?
          *
          * @param word The two byte value that we want to switch the bytes for.
          *
          * @return The bytes of the input word specified but with the high byte
          *         in the low bytes spot and the low byte in the high bytes spot.
          */
         static uint16_t swapBytes(const uint16_t word);

         /**
          * returns a fully qualified path of the executable, (excluding the
          * executables name).
          *
          * @return The fully qualified path where the executable is run from.
          */
         static std::string FullyQualifiedPath();

         /**
          * Checks if the specified file already exists.
          *
          * @param filePath The path to the file to check foe existance.
          *
          * @return True if the file specified by the file path exists, False if
          *         it does not.
          */
         static bool FileExists(std::string filePath);

         /**
          * Sets the concrete instance of the log driver to allow any Assertions
          * to send messages to the system's error log.
          *
          * @param logDriver ILogDriver instance to use for adding messages to
          *                  to the system's error log.
          */
         static void SetLogDriver(ILogDriver* logDriver);

         /**
          * Logs the specified string to the log driver.
          *
          * @param message String message to log to the log driver.
          */
         static void log(string message);

         /**
          * Gets the number of clock ticks per second of the system's processor.
          *
          * @return The number of ticks per second of the system clock.
          */
         static uint64_t ticksPerSecond();

         // NOTE: The next three string functions are useful cleaning up strings
         //       by trimming empty spaces away from strings.
         //       Similar functions are typically available for modern managed
         //       languages.

         /**
          * Trim spaces from the left side (start) of the specified string.
          *
          * @param str String to trim spaces off of.
          *
          * @return String with spaces trimmed off of the left side (start). of the string.
          */
         static std::string& ltrim(std::string& str);

         /**
          * Trim spaces from the right side (end) of the specified string.
          *
          * @param str String to trim spaces from.
          *
          * @return String with spaces trimmed off of the right side (end) of the string.
          */
         static std::string& rtrim(std::string& str);

         /**
          * Trim spaces from both ends of the specified string
          *
          * @param str String to trim spaces from.
          *
          * @return String with spaces trimmed from both the start and the end.
          */
         static std::string& trim(std::string& str);

      protected:
         BasicUtil();
         virtual ~BasicUtil();

         /**
          * The maximum number of characters in the executable path.
          */
         static const uint16_t MAX_PATH_LENGTH = 4096;

         static BasicUtil* m_instance;

         std::string m_executablePath;

         ILogDriver* m_logDriver;
   };

   /**
    * Setup all the generic Linux Signal Handlers.
    */
   void setSignalHandler();

   /**
    * Generic Signal Handler called for all of the signal
    *
    * @param signal The system signal sent to the handler.
    */
   void genericSignalHandler(int signal);
}
#endif
