CC := gcc

# Place to store your newly build files
BUILD_DIR := debug
# place for the configurations only *.h files in this folder
CONFIG_DIR := config
# folder where the unit tests are located
TEST_DIR := test

ifeq ($(OS),Windows_NT)
TARGET_NAME := buffers.exe
else
TARGET_NAME := buffers.out
endif

INCLUDE_PATHS := fifo \
	lifo \
	${CONFIG_DIR} \
	${TEST_DIR}

SOURCE_DIRS = fifo \
	lifo

# CAUTION - Do not edit anything below if you are not sure what you are doing.
# STAY AWAY
# Add -I for include paths for GCC include
INCLUDE_PARAMS := $(addprefix -I ,$(INCLUDE_PATHS))

# Find all the *.c files in the source directories.
# Note sub folders are not automatically found
SOURCE_FILES := $(shell find ${SOURCE_DIRS} -maxdepth 1 -name '*.c')
TEST_SOURCE_FILES := $(shell find ${TEST_DIR} -name '*.c')

# Make all the *.c files to *.o files
OBJECT_FILES = $(patsubst %.c,%.o,$(notdir $(SOURCE_FILES)))
TEST_OBJECT_FILES = $(patsubst %.c,%.o,$(notdir $(TEST_SOURCE_FILES)))

OPTS = -g3 -Wall -c -Wno-unused-function

.PHONY: run
.PHONY: all
.PHONY: link
.PHONY: build
.PHONY: clean
.PHONY: pristine

# Compile and link default
all: build
	$(info Build Complete)

# Compile only
build: clean ${OBJECT_FILES}
	$(info Source build complete)

# Makes a new directory for the build after cleaning
clean: pristine
	$(info Starting build)
	mkdir ${BUILD_DIR}

# Cleans the build directories
pristine:
	$(info Cleaning all the files)
	rm -rf ${BUILD_DIR}

# For each object file find the corresponding c file to build from the root directory
%.o:
	${CC} ${OPTS} ${INCLUDE_PARAMS} $(shell find . -name ${subst .o,.c,${notdir $@}} -type f) -o ${BUILD_DIR}/$@

# Run Unit tests
run_unit_tests: build ${TEST_OBJECT_FILES}
	$(info Building unit tests)
	$(info The target built is ${BUILD_DIR}/${TARGET_NAME})
	${CC} -o ${BUILD_DIR}/${TARGET_NAME} $(addprefix ${BUILD_DIR}/,${OBJECT_FILES} ${TEST_OBJECT_FILES})
	$(info Running Unit Tests)
	@./${BUILD_DIR}/${TARGET_NAME}