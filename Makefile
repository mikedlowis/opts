###############################################################################
#
# Name:    Opts
# Type:    Library
# Author:  Mike Lowis
# License: BSD 2-Clause
#
###############################################################################

# Utility Function Definitions
#-----------------------------
# Function for generating an file list
flist = $(shell env find $(1) -name *.$(strip $(2)) -print)

# Function for generating an file list
dlist = $(shell env find $(1) -type d -print)

# Function for generating an include list
incdirs = $(addprefix -I, $(call dlist, $(1)))

# Project and Artifact Names
#---------------------------
PROJ_NAME   = opts
TEST_RUNNER = test_runner
SHARED_NAME = lib$(PROJ_NAME).lib
STATIC_NAME = lib$(PROJ_NAME).a

# File and Directory Settings
#----------------------------
# Root Directories
SRC_ROOT   = source/
TESTS_ROOT = tests/

# File Extensions
SRC_EXT    = c
TEST_EXT   = cpp

# Libraries to Link Against
LIBS      =
TEST_LIBS = $(LIBS) \
            tools/UnitTest++/libUnitTest++.a

# Source File Lists
SRC_FILES  = $(call flist, $(SRC_ROOT), $(SRC_EXT))
TEST_FILES = $(call flist, $(TESTS_ROOT), $(TEST_EXT))

# Object File Lists
SRC_OBJS  = $(SRC_FILES:%.$(SRC_EXT)=%.o)
TEST_OBJS = $(TEST_FILES:%.$(TEST_EXT)=%.o)

# Include Directories
INC_DIRS = $(call incdirs, $(SRC_ROOT)) \
           $(call incdirs, tools/UnitTest++/src)

# Compiler and Linker Options
#----------------------------
CXXFLAGS      = $(INC_DIRS) -Wall -fPIC
TEST_CXXFLAGS = $(INC_DIRS) -Wall
ARFLAGS       = rcs

# Build Rules
#------------
all: release test

release: $(SHARED_NAME) $(STATIC_NAME)

test: $(TEST_RUNNER)
	./$(TEST_RUNNER)

# Binaries
$(SHARED_NAME): $(SRC_OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(SRC_OBJS) $(LIBS)

$(STATIC_NAME): $(SRC_OBJS)
	$(AR) $(ARFLAGS) -o $@ $(SRC_OBJS) $(LIBS)

$(TEST_RUNNER): unit_test_pp $(SRC_OBJS) $(TEST_OBJS)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $(SRC_OBJS) $(TEST_OBJS) $(TEST_LIBS)

# Libraries
unit_test_pp:
	$(MAKE) -C tools/UnitTest++

# Object Files
$(SRC_OBJS): %.o : %.$(SRC_EXT)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TEST_OBJS): %.o : %.$(TEST_EXT)
	$(CXX) -c $(TEST_CXXFLAGS) -o $@ $<

# Cleanup
clean:
	$(MAKE) -C tools/UnitTest++ clean
	$(RM) $(SRC_OBJS)
	$(RM) $(TEST_OBJS)

