#------------------------------------------------------------------------------
# Build Configuration
#------------------------------------------------------------------------------
# Update these variables according to your requirements.

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
# Build Targets and Rules
#------------------------------------------------------------------------------
LIBNAME = opts
LIB  = lib${LIBNAME}.a

DEPS = ${OBJS:.o=.d}
OBJS = source/opts.o

TEST_BIN  = test${LIBNAME}
TEST_DEPS = ${TEST_OBJS:.o=.d}
TEST_OBJS = tests/atf.o       \
            tests/main.o      \
            tests/test_opts.o \
            tests/test_opt.o

# load user-specific settings
-include config.mk

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

${LIB}: ${OBJS}
	${ARCHIVE}

${TEST_BIN}: ${TEST_OBJS} ${LIB}
	${LINK}
	@./$@

.c.o:
	${COMPILE}

clean:
	${CLEAN} ${LIB} ${TEST_BIN} ${OBJS} ${TEST_OBJS}
	${CLEAN} ${OBJS:.o=.gcno} ${OBJS:.o=.gcda}
	${CLEAN} ${TEST_OBJS:.o=.gcno} ${TEST_OBJS:.o=.gcda}
	${CLEAN} ${DEPS} ${TEST_DEPS}

# load dependency files
-include ${DEPS}
-include ${TEST_DEPS}

.PHONY: all options tests

