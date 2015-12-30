#------------------------------------------------------------------------------
# Build Configuration
#------------------------------------------------------------------------------
# Update these variables according to your requirements.

# version
VERSION = 0.0.1

# tools
CC = c99
LD = ${CC}
AR = ar

# flags
INCS      = -Isource/ -Itests/
CPPFLAGS  = -D_XOPEN_SOURCE=700
CFLAGS   += ${INCS} ${CPPFLAGS}
LDFLAGS  += ${LIBS}
ARFLAGS   = rcs

# commands
COMPILE = @echo CC $@; ${CC} ${CFLAGS} -c -o $@ $<
LINK    = @echo LD $@; ${LD} -o $@ $^ ${LDFLAGS}
ARCHIVE = @echo AR $@; ${AR} ${ARFLAGS} $@ $^
CLEAN   = @rm -f

#------------------------------------------------------------------------------
# Build-Specific Macros
#------------------------------------------------------------------------------
# Library macros and rules
LIBNAME = opts
LIB  = lib${LIBNAME}.a
DEPS = ${OBJS:.o=.d}
OBJS = source/opts.o

# Test binary macros
TEST_BIN  = test${LIBNAME}
TEST_DEPS = ${TEST_OBJS:.o=.d}
TEST_OBJS = tests/atf.o       \
            tests/main.o      \
            tests/test_opts.o \
            tests/test_opt.o

# Distribution dir and tarball settings
DISTDIR   = ${LIBNAME}-${VERSION}
DISTTAR   = ${DISTDIR}.tar
DISTGZ    = ${DISTTAR}.gz
DISTFILES = config.mk LICENSE.md Makefile README.md source tests

# load user-specific settings if they exist
-include config.mk

#------------------------------------------------------------------------------
# Phony Targets
#------------------------------------------------------------------------------
.PHONY: all options tests dist

all: options ${LIB} tests

options:
	@echo "Toolchain Configuration:"
	@echo "  CC       = ${CC}"
	@echo "  CFLAGS   = ${CFLAGS}"
	@echo "  LD       = ${LD}"
	@echo "  LDFLAGS  = ${LDFLAGS}"
	@echo "  AR       = ${AR}"
	@echo "  ARFLAGS  = ${ARFLAGS}"

tests: ${TEST_BIN}
	@./${TEST_BIN}

dist: clean
	@echo DIST ${DISTGZ}
	@mkdir -p ${DISTDIR}
	@cp -R ${DISTFILES} ${DISTDIR}
	@tar -cf ${DISTTAR} ${DISTDIR}
	@gzip ${DISTTAR}
	@rm -rf ${DISTDIR}

clean:
	${CLEAN} ${LIB} ${TEST_BIN} ${OBJS} ${TEST_OBJS}
	${CLEAN} ${OBJS:.o=.gcno} ${OBJS:.o=.gcda}
	${CLEAN} ${TEST_OBJS:.o=.gcno} ${TEST_OBJS:.o=.gcda}
	${CLEAN} ${DEPS} ${TEST_DEPS}
	${CLEAN} ${DISTTAR} ${DISTGZ}

#------------------------------------------------------------------------------
# Target-Specific Rules
#------------------------------------------------------------------------------
.c.o:
	${COMPILE}

${LIB}: ${OBJS}
	${ARCHIVE}

${TEST_BIN}: ${TEST_OBJS} ${LIB}
	${LINK}

# load dependency files if they exist
-include ${DEPS}
-include ${TEST_DEPS}

