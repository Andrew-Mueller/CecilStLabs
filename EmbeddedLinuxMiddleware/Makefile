# ----------
# Makes the shared archive file for common code.
#
# This build script assumes the following variables have been set external
# C_COMPILER
# CPP_COMPILER
# INC_DIR
# LIB_DIR
# LIB_FLAGS
# COMPILE_FLAGS
# TEST_C_COMPILER
# TEST_CPP_COMPILER
TEST_C_COMPILER=gcc
TEST_CPP_COMPILER=g++

# additionally for unit tests, the following should be defined
# These are locations of the Google Unit Test Framework
# TODO: pass in the GTEST_INCLUDE_DIR
GTEST_FRAMEWORK_DIR := test/UnitTestFramework/gtest-1.6.0
UNIT_TEST_DIR := test

UNIT_TEST_INC_DIR = -I$(GTEST_FRAMEWORK_DIR)/include
UNIT_TEST_FLAGS = -m32 -g -Wall -Wextra --coverage
# ----------

SOURCES := common/util/BasicUtil.cpp \
           common/util/DebugPrint.cpp \
           common/util/EnumByName.cpp \
           common/util/NameValuePair.cpp \
           common/config/ConfigActionRead.cpp \
           common/config/ConfigActionWrite.cpp \
           common/config/NullConfig.cpp \
           common/config/NetworkConfigSection.cpp \
           common/CRC/CRC16.cpp \
           common/CriticalSection/CriticalSection.cpp \
           common/fileMonitoring/fileMonitor.cpp \
           common/hardware/LinuxClockDriver.cpp \
           common/hardware/LinuxRS232.cpp \
           common/hardware/AudioPattern.cpp \
           common/hardware/sensor/SensorReading.cpp \
           common/hardware/sensor/SensorThreshold.cpp \
           common/guid.cpp \
           common/LinkedList/SingleLink.cpp \
           common/LinkedList/SingleLinkedList.cpp \
           common/logging/LogEntry.cpp \
           common/logging/emptyLogDriver.cpp \
           common/movingAverage/MovingAverage.cpp \
           common/RingBuffer/RingBuffer.cpp \
           common/DateTime/DateTime.cpp \
           common/stateMachine/State.cpp \
           common/stateMachine/StateMachine.cpp \
           common/stateMachine/Transition.cpp \
           common/threads/PeriodicThread.cpp \
           common/threads/ScheduledThread.cpp \
           common/threads/OneShotThread.cpp \
           common/tinyxml/tinyxml2.cpp \
           common/shell/LocalShell.cpp \
           common/shell/LocalShellBaseHandler.cpp \
           common/shell/LocalShellNormalHandler.cpp \
           common/shell/LocalShellNetHandler.cpp \
           common/shell/LocalShellNetDnsHandler.cpp \
           common/NetCfg/CommandActionSystem.cpp \
           common/InternalEvent/GenericInternalEvent.cpp \
           common/InternalEvent/SetConfigInternalEvent.cpp \
           common/InternalEvent/InternalEventRegistry.cpp \
           common/InternalEvent/InternalEventRegistryEntry.cpp \
           common/XMLFileTable/XMLFileTable.cpp \
           comms/RXData/RXData.cpp \
           comms/NetUtil.cpp \
           comms/SocketBase.cpp \
           comms/Socket.cpp \
           comms/SecureSession.cpp \
           comms/SecureSocket.cpp \
           comms/WebSocket.cpp \
           comms/HTTP/HTTPResponse.cpp \
           comms/HTTP/HTTPS.cpp \
           comms/WebSocketProtocol/WebSocketProtocol.cpp \
           comms/WebSocketProtocol/WebSocketResponse.cpp \
           comms/WebSocketProtocol/WebSocketFrame.cpp \
           comms/CommQueue.cpp \
           comms/CommRecord.cpp \
           comms/CommDAL.cpp \
           comms/CommStateMachine/BidirectionalCommsAvailableState.cpp \
           comms/CommStateMachine/UnidirectionalCommsAvailableState.cpp \
           comms/CommStateMachine/CommsUnavailableState.cpp
           

OBJDIR=obj

OBJS := BasicUtil.o \
        DebugPrint.o \
        EnumByName.o \
        NameValuePair.o \
        ConfigActionRead.o \
        ConfigActionWrite.o \
        NullConfig.o \
        NetworkConfigSection.o \
        CRC16.o \
        CriticalSection.o \
        fileMonitor.o \
        LinuxClockDriver.o \
        LinuxRS232.o \
        AudioPattern.o \
        SensorReading.o \
        SensorThreshold.o \
        guid.o \
        SingleLink.o \
        SingleLinkedList.o \
        LogEntry.o \
        emptyLogDriver.o \
        MovingAverage.o \
        RingBuffer.o \
        DateTime.o \
        State.o \
        StateMachine.o \
        Transition.o \
        LocalShell.o \
        LocalShellBaseHandler.o \
        LocalShellNormalHandler.o \
        LocalShellNetHandler.o \
        LocalShellNetDnsHandler.o \
        CommandActionSystem.o \
        GenericInternalEvent.o \
        SetConfigInternalEvent.o \
        InternalEventRegistry.o \
        InternalEventRegistryEntry.o \
        XMLFileTable.o \
        RXData.o \
        NetUtil.o \
        SocketBase.o \
        Socket.o \
        SecureSession.o \
        SecureSocket.o \
        WebSocket.o \
        HTTPResponse.o \
        HTTPS.o \
        WebSocketProtocol.o \
        WebSocketResponse.o \
        WebSocketFrame.o \
        CommQueue.o \
        CommRecord.o \
        CommDAL.o \
        BidirectionalCommsAvailableState.o \
        UnidirectionalCommsAvailableState.o \
        CommsUnavailableState.o \
        OneShotThread.o \
        PeriodicThread.o \
        SysLogMonitorThread.o \
        CommQueueThread.o \
        ScheduledThread.o \
        tinyxml2.o \
        Command.o \
        CommandList.o \
        ResetCommand.o \
        GetConfigValuesCommand.o \
        SetConfigValuesCommand.o \
        SyncTime_Request.o \
        sqlite3.o \        

# TODO: socket unit tests are omitted for now.
#                    $(UNIT_TEST_DIR)/comms/Socket_Test.cpp
        
UNIT_TEST_SOURCES := $(UNIT_TEST_DIR)/common/MemoryPool/MemoryPool_Test.cpp \
                     $(UNIT_TEST_DIR)/common/movingAverage/MovingAverage_Test.cpp \
                     $(UNIT_TEST_DIR)/common/RingBuffer/RingBuffer_Test.cpp \
                     $(UNIT_TEST_DIR)/common/config/NetworkConfigSection_Test.cpp \
                     $(UNIT_TEST_DIR)/comms/RXData/RXData_Test.cpp \
                     $(UNIT_TEST_DIR)/comms/HTTP/HTTPResponse_Test.cpp \
                     $(UNIT_TEST_DIR)/comms/CommRecord_Test.cpp \
                     $(UNIT_TEST_DIR)/common/guid_Test.cpp \
                     $(UNIT_TEST_DIR)/common/DateTime/DateTime_Test.cpp \
                     $(UNIT_TEST_DIR)/common/util/EnumByName_Test.cpp \
                     $(UNIT_TEST_DIR)/common/util/NameValuePair_Test.cpp \
                     $(UNIT_TEST_DIR)/common/util/BasicUtil_Test.cpp \
                     $(UNIT_TEST_DIR)/common/fileMonitoring/fileMonitor_Test.cpp \
                     $(UNIT_TEST_DIR)/comms/WebSocketProtocol/WebSocketFrame_Test.cpp \
                     $(UNIT_TEST_DIR)/comms/CommDAL_Test.cpp \
                     $(UNIT_TEST_DIR)/common/stateMachine/Transition_Test.cpp \
                     $(UNIT_TEST_DIR)/common/stateMachine/State_Test.cpp \
                     $(UNIT_TEST_DIR)/common/stateMachine/StateMachine_Test.cpp \
                     $(UNIT_TEST_DIR)/common/CRC/CRC16_Test.cpp \
                     $(UNIT_TEST_DIR)/common/shell/LocalShell_Test.cpp \
                     $(UNIT_TEST_DIR)/common/InternalEvent/InternalEvent_Test.cpp \
                     $(UNIT_TEST_DIR)/common/hardware/sensor/SensorReading_Test.cpp \
                     $(UNIT_TEST_DIR)/common/threads/PeriodicThread_Test.cpp

UNIT_TEST_OBJS    := MemoryPool_Test.o \
                     MovingAverage_Test.o \
                     RingBuffer_Test.o \
                     NetworkConfigSection_Test.o \
                     RXData_Test.o \
                     HTTPResponse_Test.o \
                     CommRecord_Test.o \
                     CommandList_Test.o \
                     GetConfigValuesCommand_Test.o \
                     SetConfigValuesCommand_Test.o \
                     SensorReadingEvent_Test.o \
                     guid_Test.o \
                     DateTime_Test.o \
                     inventory_Test.o \
                     EnumByName_Test.o \
                     NameValuePair_Test.o \
                     BasicUtil_Test.o \
                     fileMonitor_Test.o \
                     WebSocketFrame_Test.o \
                     CommDAL_Test.o \
                     InternalEvent_Test.o \
                     LocalShell_Test.o \
                     SensorReading_Test.o \
                     PeriodicThread_Test.o
                     

# by default, gnu make only looks at the first target, so put the first target 
# as the default.
# NOTE: lint is currently not included in the arm configuration until it gets resolved on the build server.
.PHONY: all
all: SVNVersion EmbeddedLinuxMiddleware.a

# TODO: convert this to GitVersion
# Capture the subversion repository revision and make it available in a constant accessible from code.
# This is to simulate the tortoise svn subwcrev command.
.PHONY: SVNVersion
SVNVersion:
	build/generateSvnVersion.sh $(BUILD) $(SPRINT)

# Create the Archive (library) for re-use within the proper application
# the library is all of the modules excluding main.  This allows this code
# to be linked into other programs with their own main() such as the UnitTest framework. 
EmbeddedLinuxMiddleware.a: sqlite
	$(CPP_COMPILER) $(INC_DIR) $(SOURCES) $(EXT_SOURCES) $(COMPILE_FLAGS) -g -c $(LIB_DIR) $(LIB_FLAGS)
	ar $(ARFLAGS) $(EmbeddedLinuxMiddlewareOutput) $(OBJS)

# Only compile the Unit tests and package for an archive.  They will be built
# into the unit test project of the user of the library.
unitTests:
	$(TEST_CPP_COMPILER) $(UNIT_TEST_INC_DIR) $(UNIT_TEST_FLAGS) $(COMPILE_FLAGS) $(UNIT_TEST_SOURCES) -c $(LIB_DIR) $(LIB_FLAGS)
	ar $(ARFLAGS) bin/EmbeddedLinuxMiddleware_UnitTests.a $(UNIT_TEST_OBJS)

# Compile the core sqlite library seperately as required by the libraries 
# documentation.
.PHONY: sqlite
sqlite:
	$(C_COMPILER) $(COMPILE_FLAGS) sqlite/sqlite3.c -c
	
.PHONY : lint
lint:
	# generate the c and c++ macro definitions
	touch lint/empty.c
	touch lint/empty.cpp
	$(C_COMPILER) -E $(COMPILE_FLAGS) -dM lint/empty.c >lint/lint_cmac.h
	$(CPP_COMPILER) -E $(COMPILE_FLAGS) -dM lint/empty.cpp >lint/lint_cppmac.h 

	# remove the empty files created for generating the macros
	rm -f lint/empty.c
	rm -f lint/empty.cpp

	# we want each filename on its own line in project.lnt
	# First we generate the input of the source files
	# next we substitute a new line for the space seperating each file.
	# finally prepend a ../ to the start of every line in the file.
	echo $(SOURCES) | tr ' ' '\n' | sed -e 's/^/..\//' >lint/project.lnt
	
	# copy the platform specific gcc-include-path.lnt file into the working directory
	cp $(LINT_CONF)/gcc-include-path.lnt lint
	
	# lint all of the source files
	cd lint; FlexeLint9 std.lnt -w1 project.lnt

.PHONY : coverage
coverage:
	
	# generate the code coverage statistics for all of the source files
	# here we must call gcov with the parameter appended onto each of the source
	# file to properly filter out the system and included files.
	for i in $(SOURCES); do gcov --long-file-names $$i; done
	
	# remove the system header files that we don't care about...
	rm *\#\#*.gcov
	
	# remove the tiny xml library from coverage (external library)
	rm tinyxml2.cpp.gcov

# TODO: add code complexity tool
.PHONY : complexity
complexity:
        py lizard
	
# Provide removal of the intermediate files and the final output.	
.PHONY: clean
clean: removeOutputs removeObj removeSVNVersion removeCoverage
	
# force the svnVersion.h file to be regenerated
.PHONY: removeSVNVersion
removeSVNVersion:
	rm -f common/svnVersion.h
	rm -f docs/svnVersion.js
	
.PHONY: removeOutputs
removeOutputs:
	rm -rf bin/*
	
.PHONY: removeObj 
removeObj:
	rm -f *.gcno
	rm -f *.gcda
	rm -f *.o
	rm -f *.a
	
.PHONY: removeCoverage
removeCoverage:
	rm -f *.gcov
	rm -f coverage.xml
