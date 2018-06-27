CC=gcc

CONFIGDIR = config
BUILDDIR = debug
TESTDIR = test
TARGETNAME = buffer.out

INCS = fifo \
	lifo \
	${CONFIGDIR}  \
	${TESTDIR}

SRCS = main.c fifo/fifo.c lifo/lifo.c test/buffer_test.c

INCLUDEPARAMS = $(addprefix -I,$(INCS))

DEPS = ${CONFIGDIR}/lifo_config.h \
	${CONFIGDIR}/fifo_config.h \
	${TESTDIR}/buffer_test.h \
	lifo/lifo.h \
	fifo/fifo.h

OPTS = -g3 -Wall -c

OBJECTS = ${BUILDDIR}/main.o ${BUILDDIR}/fifo.o ${BUILDDIR}/lifo.o ${BUILDDIR}/buffer_test.o

run: all
	@echo Running the program
	@./${BUILDDIR}/${TARGETNAME}

all: link

clean: 
	@echo Cleaning all the files
	rm -rf ${BUILDDIR}

makedir:
	@echo Starting build
	mkdir ${BUILDDIR}

build: clean makedir ${OBJECTS}
	@echo Building
	
link: build
	@echo Linking
	${CC} -o ${BUILDDIR}/${TARGETNAME} ${OBJECTS}

${BUILDDIR}/main.o:
	${CC} ${OPTS} ${INCLUDEPARAMS} main.c -o ${BUILDDIR}/main.o

${BUILDDIR}/fifo.o:
	${CC} ${OPTS} ${INCLUDEPARAMS} fifo/fifo.c -o ${BUILDDIR}/fifo.o

${BUILDDIR}/lifo.o:
	${CC} ${OPTS} ${INCLUDEPARAMS} lifo/lifo.c -o ${BUILDDIR}/lifo.o

${BUILDDIR}/buffer_test.o:
	${CC} ${OPTS} -Wno-unused-function ${INCLUDEPARAMS} ${TESTDIR}/buffer_test.c -o ${BUILDDIR}/buffer_test.o