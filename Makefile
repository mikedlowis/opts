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
CFLAGS   += -g ${INCS} ${CPPFLAGS}
LDFLAGS  += ${LIBS}
ARFLAGS   = rcs

#------------------------------------------------------------------------------
# Build Targets and Rules
#------------------------------------------------------------------------------
SRCS = source/opts.c
OBJS = ${SRCS:.c=.o}
TEST_SRCS = tests/atf.c tests/main.c tests/test_opts.c tests/test_opt.c
TEST_OBJS = ${TEST_SRCS:.c=.o}

all: options libopts.a testopts

options:
	@echo "Toolchain Configuration:"
	@echo "  CC       = ${CC}"
	@echo "  CFLAGS   = ${CFLAGS}"
	@echo "  LD       = ${LD}"
	@echo "  LDFLAGS  = ${LDFLAGS}"
	@echo "  AR       = ${AR}"
	@echo "  ARFLAGS  = ${ARFLAGS}"

libopts.a: ${OBJS}
	@echo AR $@
	@${AR} ${ARFLAGS} $@ ${OBJS}

testopts: ${TEST_OBJS} libopts.a
	@echo LD $@
	@${LD} -o $@ ${TEST_OBJS} libopts.a ${LDFLAGS}
	-./$@

.c.o:
	@echo CC $<
	@${CC} ${CFLAGS} -c -o $@ $<

clean:
	@rm -f libopts.a testopts ${OBJS} ${TEST_OBJS}

.PHONY: all options

