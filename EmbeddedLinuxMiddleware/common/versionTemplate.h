/**
 * Specifies the user controlled version number, generate version
 * information of the repository.
 * This template is intended to be filled in with values at build time.  the file
 * svn_version.h is intended to be created
 */

#ifndef VERSIONTEMPLATE_H_
#define VERSIONTEMPLATE_H_

/**
 *  The user specified version number of the software.
 *  Change the major parts of this version number by hand
 *  Currently the "build" number is the sprint number.
 *  TODO: allow build system to set the "build" number automatically.
 *        Can I get this from an environment variable set by Bamboo?
 */
#define VERSION_NUMBER ("0.9.27.0")

class SwVersion
{
   public:
      /**
       *  The user specified version number of the software.
       */
      static const char* swVersion(void)
      {
         return VERSION_NUMBER;
      }

      /**
       * The current revision number (reported by the SVN tool)
       */
      static const char* svnVersion(void)
      {
         // 81 or 73:81M if modified
         const char* SVN_Version = "__SVN_VERSION__";
         return SVN_Version;
      }

      static const char* svnDate(void)
      {
         // Last Changed Date: 2013-04-01 12:10:54 -0500 (Mon, 01 Apr 2013)
         const char* SVN_DATE = "__SVN_DATE__";
         return SVN_DATE;
      }

      /**
       * The current revision path.  This is useful for which svn
       * tag was created for a release.
       */
      static const char* svnPath(void)
      {
         const char* SVN_PATH = "__SVN_PATH__";
         return SVN_PATH;
      }
};

#endif
