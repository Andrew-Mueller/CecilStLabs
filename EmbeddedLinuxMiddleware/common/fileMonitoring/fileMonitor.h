#ifndef FILEMONITOR_H_
#define FILEMONITOR_H_

namespace CecilStLabs
{
   /**
    * Monitor the specified file for change.
    *
    * NOTE: The implementation detail of the file manipulation is specific to its
    * original use in that it was reading from a named pipe that was being written
    * to by the syslog.
    *
    * NOTE: this file is currently NOT re-entrant.  the last modified time is an
    *       internal variable so a single object cannot be shared between threads.
    *       However, if each thread creates its own separate instance of this
    *       class (even if they point to the same file), there is no re-entrance
    *       problem.
    */
   class fileMonitor : public IFileMonitor
   {
      public:

         /**
          * Create the file monitor to watch for changes in the specified file.
          *
          * @param filePath Path to the file to watch in the file system.
          * @param handler Object that implements the File Monitor Handler
          *                interface to handle changes when they are detected
          *                in the file.
          */
         fileMonitor(std::string filePath, IFileMonitorHandler* handler);

         /**
          * Release resources consumed by the file monitor.
          */
         virtual ~fileMonitor();

         /**
          * Reads the lines added to the file being monitored since the last
          * time the file was read.
          *
          * @return String containing the new text, or empty string if no text
          *         was added.
          */
         std::string getLinesSinceLastChange();

         /**
          * Sets the file monitor to watch for changes in the specified file.
          *
          * @param handler Object that implements the File Monitor Handler
          *                interface to handle changes when they are detected
          *                in the file.
          */
         inline void setFileMonitorHandler(IFileMonitorHandler* handler) { m_monitorHandler = handler; }

         /**
          * Implementation of the IFileMonitor interface.  See @ref IFileMonitor.h
          * for documentation.
          */
         void initialize();
         void monitor();
         void destroy();

      protected:

         /**
          * The maximum size of the line that we will read at a time.
          */
         static const uint16_t MAX_LINE_SIZE = 1024;

         /**
          * Checks if the file has been modified since the last check.
          * NOTE: the method that is currently being used to see if the file has
          *       been changed has only one second resolution at best.
          *       This means that if the file has changed, this method will
          *       not notify us of the change for up to 1 second.
          *
          * @return True if the file was modified, False if it is the same.
          */
         bool isModified();

         /**
          * Path of the file being monitored for changes.
          */
         std::string m_filepath;

         /**
          * the timestamp of the last modification.
          */
         time_t m_lastModifiedTime;

         /**
          * handle to the file we are tracking changes for.
          */
         int m_fileHandle;

         /**
          * event handler called when changes are read from the file.
          */
         IFileMonitorHandler* m_monitorHandler;

   };

}
#endif
