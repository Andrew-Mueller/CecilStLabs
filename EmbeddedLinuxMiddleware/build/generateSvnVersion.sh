#!/bin/bash -e

# generate the file containing the SVN repository version, date, and  path (tag
# when appropriate)

# NOTE: utilize the bash shell instead of the sh shell because the sh shell 
#       does not honor the -n option

# Version of the repository

cp build/versionTemplate.h build/svnVersion.h
cp docs/versionTemplate.js docs/svnVersion.js

SVN_VERSION=$(svnversion -n)
sed "s/__COMPONENT_SVN_VERSION__/${SVN_VERSION}/g" build/svnVersion.h > svnVersion1.h
sed "s/__COMPONENT_SVN_VERSION__/${SVN_VERSION}/g" docs/svnVersion.js > svnVersion1.js

# Bamboo Build Number
sed "s/__BUILD_NUMBER__/$1/g" svnVersion1.h > svnVersion2.h
sed "s/__BUILD_NUMBER__/$1/g" svnVersion1.js > svnVersion2.js

# Bamboo Sprint Number
sed "s/__SPRINT_NUMBER__/$2/g" svnVersion2.h > svnVersion1.h
sed "s/__SPRINT_NUMBER__/$2/g" svnVersion2.js > svnVersion1.js

# Date of the last commit
SVN_DATE=$(svn info | grep Date)
sed "s/__COMPONENT_SVN_DATE__/${SVN_DATE}/g" svnVersion1.h > svnVersion2.h
sed "s/__COMPONENT_SVN_DATE__/${SVN_DATE}/g" svnVersion1.js > svnVersion2.js

# path of the repository (significant to capture tagged versions)
SVN_PATH=$(svn info | grep URL)
SVN_PATH=$(echo $SVN_PATH | tr ' ' '_')

# notice that we use the @ sign instead of the slash because the 
# url has slashes in them.
sed -e "s@__COMPONENT_SVN_PATH__@${SVN_PATH}@g" svnVersion2.h > build/svnVersion.h
sed -e "s@__COMPONENT_SVN_PATH__@${SVN_PATH}@g" svnVersion2.js > docs/svnVersion.js

# cleanup the temp files
rm svnVersion1.h
rm svnVersion2.h

rm svnVersion1.js
rm svnVersion2.js
