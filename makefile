CC := gcc
CONFIG_DIR := config
BUILD_DIR := debug
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

INCLUDE_PARAMS := $(addprefix -I,$(INCLUDE_PATHS))

SOURCE_FILES := $(shell find . -name '*.c' -type f)

OBJECT_FILES := $(patsubst %.c,%.o,$(notdir $(SOURCE_FILES)))

OPTS = -g3 -Wall -c -Wno-unused-function

.PHONY: run
.PHONY: all
.PHONY: link
.PHONY: build
.PHONY: clean
.PHONY: pristine

all: link
	$(info Build Complete)

link: build
	$(info Linking)
	$(info The target built will be ${TARGET_NAME})
	${CC} -o ${BUILD_DIR}/${TARGET_NAME} $(addprefix ${BUILD_DIR}/,${OBJECT_FILES})

build: clean ${OBJECT_FILES}
	$(info Building)

clean: pristine
	$(info Starting build)
	mkdir ${BUILD_DIR}

pristine:
	$(info Cleaning all the files)
	rm -rf ${BUILD_DIR}

${OBJECT_FILES}:
	${CC} ${OPTS} ${INCLUDE_PARAMS} $(shell find . -name ${subst .o,.c,${notdir $@}} -type f) -o ${BUILD_DIR}/$@

run: all
	$(info Running the program)
	@./${BUILD_DIR}/${TARGET_NAME}