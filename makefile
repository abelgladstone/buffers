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
	${CONFIG_DIR}  \
	${TEST_DIR}

SOURCE_DIRS = fifo \
	lifo \
	test \

# CAUTION - Do not edit anything below if you are not sure what you are doing.
# STAY AWAY
# Add -I for include paths for GCC include
INCLUDE_PARAMS := $(addprefix -I ,$(INCLUDE_PATHS))

# Find all the *.c files in the root directory and the source directories.
# Note sub folders are not automatically found
SOURCE_DIRS := ${SOURCE_DIRS} .
SOURCE_FILES := $(shell find ${SOURCE_DIRS} -maxdepth 1 -name '*.c')

# Make all the *.c files to *.o files
OBJECT_FILES := $(patsubst %.c,%.o,$(notdir $(SOURCE_FILES)))

OPTS = -g3 -Wall -c -Wno-unused-function

.PHONY: run
.PHONY: all
.PHONY: link
.PHONY: build
.PHONY: clean
.PHONY: pristine

# Compile and link default
all: build
	$(info Linking)
	$(info The target built is ${BUILD_DIR}/${TARGET_NAME})
	${CC} -o ${BUILD_DIR}/${TARGET_NAME} $(addprefix ${BUILD_DIR}/,${OBJECT_FILES})
	$(info Build Complete)

# Compile only
build: clean ${OBJECT_FILES}
	$(info Building)

# Makes a new directory for the build after cleaning
clean: pristine
	$(info Starting build)
	mkdir ${BUILD_DIR}

# Cleans the build directories
pristine:
	$(info Cleaning all the files)
	rm -rf ${BUILD_DIR}

# For each object file find the corresponding c file to build from the root directory
${OBJECT_FILES}:
	${CC} ${OPTS} ${INCLUDE_PARAMS} $(shell find . -name ${subst .o,.c,${notdir $@}} -type f) -o ${BUILD_DIR}/$@

# Builds and runs the program too. I don't have clue as to why I need this
run: all
	$(info Running the program)
	@./${BUILD_DIR}/${TARGET_NAME}