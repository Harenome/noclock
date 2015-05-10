# This Makefile is derived from my "template" Makefile : although it has been
# edited with love and care, some unnecessary rules may remain.
#
# When starting a new project, the following sections will most likely need to
# be edited (and should not need to be edited again) :
# - Program
# - Paths
# - Files
# - Flags
#
# Of course, some projects will need special care and others sections may need
# to be edited as well.
#
# The section 'Actual building' is the section that will constantly be edited
# as the project grows and new files are added. The most naïve way is to merely
# add a rule for each new `.o` object file and add it to the `PROGRAM_OBJECTS`
# variable.
#
# For instance:
#     PROGRAM_OBJECTS: main.o
#
#     main.o: main.c version.h

################################################################################
# Various variables
################################################################################

NORMAL = \x1B[0m
BOLD = \x1B[1m
UL = \x1B[4m
BOLD_UL = $(BOLD)$(UL)

CYAN = \x1B[38;5;45m
BLUE = \x1B[38;5;33m
GREEN = \x1B[38;5;106m
LIGHT_YELLOW = \x1B[38;5;227m
YELLOW = \x1B[38;5;226m
LIGHT_ORANGE = \x1B[38;5;220m
ORANGE = \x1B[38;5;214m
DARK_ORANGE = \x1B[38;5;208m

################################################################################
# Completion monitoring utility
################################################################################

ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = release
%:
	@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef PROGRESS
	T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
		-nrRf $(firstword $(MAKEFILE_LIST)) \
		PROGRESS="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
PROGRESS = echo -e "$(BOLD)" \
		$(shell printf '[%5.5s %%]' \
			$(shell LC_ALL=C printf '%.2f' \
				$(shell LC_ALL=C echo "$(C) * 100 / $(T)" | LC_ALL=C bc -l) \
			) \
		) "$(NORMAL)"
endif

################################################################################
# Special targets
################################################################################

# Some variables are Target-specific (they have a value only when a specific
# target is used).
#
# Examples:
#
#     $ make release
#     $ make debug
#

release: all
debug: all

################################################################################
# .PHONY targets
################################################################################

.PHONY: all release debug \
	install uninstall \
	clean cleanall cleanlex cleanyacc cleandoc distclean \
	clean_all clean_lex clean_yacc clean_doc distclean \
	version.o

################################################################################
# Program
################################################################################

PROGRAM_NAME = noclock

################################################################################
# Paths
################################################################################

PATH_SRC = src
PATH_INCLUDE = include

PATH_BUILD = build
PATH_OBJ = $(PATH_BUILD)/obj
PATH_DOC = $(PATH_BUILD)/doc
PATH_LIB = $(PATH_BUILD)/lib
PATH_BIN = $(PATH_BUILD)/bin

PATH_MAN = man

PATH_YACC = yacc
PATH_LEX = lex

PATH_INCLUDE_YACC = $(PATH_BUILD)/autogen/$(PATH_INCLUDE)/$(PROGRAM_NAME)/yacc
PATH_SRC_YACC = $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/yacc

PATH_INCLUDE_LEX = $(PATH_BUILD)/autogen/$(PATH_INCLUDE)/$(PROGRAM_NAME)/lex
PATH_SRC_LEX = $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/lex

################################################################################
# Files
################################################################################

# Header files.
vpath %.h $(PATH_INCLUDE) $(PATH_INCLUDE)/$(PROGRAM_NAME)
vpath %.h build/autogen/$(PATH_INCLUDE)
vpath %.h build/autogen/$(PATH_INCLUDE)/$(PROGRAM_NAME)

# Source code files.
vpath %.c $(PATH_SRC) $(PATH_SRC)/$(PROGRAM_NAME)
vpath %.c $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/yacc
vpath %.c $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/lex

# Object files.
vpath %.o $(PATH_OBJ)

# Libraries.
vpath %.a $(PATH_LIB)
vpath %.so $(PATH_LIB)

# Lex/Yacc.
vpath %.l $(PATH_LEX)
vpath %.y $(PATH_YACC)

# Binary.
vpath $(PROGRAM_NAME) $(PATH_BIN)

################################################################################
# Flags, first pass.
################################################################################

debug: FLAGS_CC_DEBUG = -g -DDEBUG
debug: FLAGS_CC_WARNINGS = -pedantic -W -Wall -Wextra -Wfloat-equal \
	-Wswitch-default -Winit-self -Wshadow -Wbad-function-cast -Wcast-qual \
	-Wcast-align -Wconversion -Wlogical-op -Wstrict-prototypes -Wnested-externs

release: FLAGS_CC_OPTIMIZATIONS = -O3 -march=native

FLAGS_CC_INCLUDE = -I. -I$(PATH_INCLUDE) -Ibuild/autogen/$(PATH_INCLUDE) \
					-I$(PATH_INCLUDE_LEX) -I$(PATH_INCLUDE_YACC)
FLAGS_CC_LIB = -L$(PATH_LIB)
FLAGS_CC_MINIMAL = -std=gnu99 $(FLAGS_CC_INCLUDE)

################################################################################
# Conventionnal (mostly) variables
################################################################################

# Users usually expect Makefiles to provide support for various variables in
# order to ease configuration.
# For instance:
#     $ make CC=clang
#     $ make CFLAGS="-O2"
#     $ make install PREFIX=/opt/noclock
#     $ make LDFLAGS="-L/opt/mylib/lib"

# These are the only variables the user is *allowed* (nothing prevents the user
# from overriding other variables...) to override.

## Shell
SHELL = /bin/bash

## Programs
# These variables usually are implicitly defined.
# Ensure they are defined.
CC = gcc
AR = ar
RM = rm -rf
LEX = lex
YACC = yacc

# Extras (not really conventionnal)
DOC = doxygen
MKDIR = mkdir -p
CP = cp
ECHO = echo -e

# Install
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

## Flags
ARFLAGS = crvs
CFLAGS = $(FLAGS_CC_DEBUG) $(FLAGS_CC_WARNINGS) $(FLAGS_CC_OPTIMIZATIONS)
LDFLAGS =
LDLIBS =

## Directories
# In most situations, changing only PREFIX is enough.
#
#     $ make install PREFIX=/opt/noclock
PREFIX = /usr/local
EXEC_PREFIX = $(PREFIX)
BINDIR = $(EXEC_PREFIX)/bin
SBINDIR = $(EXEC_PREFIX)/sbin
LIBEXECDIR = $(EXEC_PREFIX)/libexec
DATAROOTDIR = $(PREFIX)/share
DATADIR = $(DATAROOTDIR)
SYSCONFDIR = $(PREFIX)/etc
SHAREDSTATEDIR = $(PREFIX)/com
LOCALSTATEDIR = $(PREFIX)/var
RUNSTATEDIR = $(LOCALSTATEDIR)/run
INCLUDEDIR = $(PREFIX)/include
DOCDIR = $(DATAROOTDIR)/doc/$(PROGRAM_NAME)
LIBDIR = $(EXEC_PREFIX)/lib
MANDIR = $(DATAROOTDIR)/man

################################################################################
# Flags, second pass
################################################################################

# The user is allowed to override some flags. But there are minimal requirements.
# Ensure these requirements are set even if the flags are empty.
override CFLAGS += $(FLAGS_CC_MINIMAL)
override LDLIBS += $(FLAGS_CC_LIB)
override LDFLAGS += -ly -lfl -lisl
override YFLAGS += -d

################################################################################
# Actual building
################################################################################

PROGRAM_OBJECTS = $(notdir $(patsubst %.c,%.o, $(wildcard $(PATH_SRC)/*/*.c)))

all: version.o $(PROGRAM_NAME) | bin_dir

## Executable

$(PROGRAM_NAME): $(PROGRAM_OBJECTS) y.tab.o lex.yy.o version.o | bin_dir
	@$(PROGRESS) "$(GREEN)Linking C executable $(BOLD_UL)$@$(NORMAL)"
	@$(CC) -o $(PATH_BIN)/$@ \
		$(patsubst %.o,$(PATH_OBJ)/%.o, $(patsubst $(PATH_OBJ)/%,%, $^)) \
		$(LDFLAGS) $(LDLIBS)

## Object files

# Generate .o object files.
%.o: %.c | obj_dir
	@$(PROGRESS) "$(BLUE)Building C object $(BOLD_UL)$@$(NORMAL)"
	@$(CC) $(CFLAGS) -o $(PATH_OBJ)/$@ -c $<

y.tab.c: $(PROGRAM_NAME).y | yacc_dir
	@$(PROGRESS) "$(CYAN)Generating yacc files" \
		"$(BOLD_UL)$@$(NORMAL)$(CYAN) and $(BOLD_UL)y.tab.h$(NORMAL)"
	@$(YACC) $(YFLAGS) -o $@ $<
	@mv y.tab.c $(PATH_SRC_YACC)/
	@mv y.tab.h $(PATH_INCLUDE_YACC)/

lex.yy.c: $(PROGRAM_NAME).l | lex_dir
	@$(PROGRESS) "$(CYAN)Generating lex file $(BOLD_UL)$@$(NORMAL)"
	@$(LEX) $(LFLAGS) -o $(PATH_SRC_LEX)/$@ $<

y.tab.o: y.tab.c | obj_dir
	@$(PROGRESS) "$(BLUE)Building C object $(BOLD_UL)$@$(NORMAL)"
	@$(CC) $(FLAGS_CC_MINIMAL) -o $(PATH_OBJ)/$@ -c $(PATH_SRC_YACC)/y.tab.c

lex.yy.o: lex.yy.c | obj_dir
	@$(PROGRESS) "$(BLUE)Building C object $(BOLD_UL)$@$(NORMAL)"
	@$(CC) $(FLAGS_CC_MINIMAL) -o $(PATH_OBJ)/$@ -c $(PATH_SRC_LEX)/lex.yy.c

## Compile time specific actions.

version.o: clean_version | build_dir obj_dir
	@$(RM) $(PATH_OBJ)/version.o
	@cp config/version.c.in config/version.c
	@sed -i "s/__NOCLOCK_USER__/$(shell whoami)/" config/version.c
	@sed -i "s/__NOCLOCK_HOST__/$(shell hostname)/" config/version.c
	@sed -i "s/__NOCLOCK_LEX__/$(shell $(LEX) --version)/" config/version.c
	@sed -i "s/__NOCLOCK_YACC__/$(shell $(YACC) -V | head -n 1)/" config/version.c
	@mv config/version.c $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/
	@$(PROGRESS) "$(BLUE)Building C object $(BOLD_UL)$@$(NORMAL)"
	@$(CC) $(CFLAGS) -o $(PATH_OBJ)/$@ \
		-c $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/version.c


################################################################################
# Documentation
################################################################################

doc: | doc_dir
	@$(DOC) Doxyfile

################################################################################
# Directories
################################################################################

obj_dir:
	@$(MKDIR) $(PATH_OBJ)

lib_dir:
	@$(MKDIR) $(PATH_LIB)

bin_dir:
	@$(MKDIR) $(PATH_BIN)

yacc_dir:
	@$(MKDIR) $(PATH_INCLUDE_YACC) $(PATH_SRC_YACC)

lex_dir:
	@$(MKDIR) $(PATH_INCLUDE_LEX) $(PATH_SRC_LEX)

doc_dir:
	@$(MKDIR) $(PATH_DOC)

build_dir:
	@$(MKDIR) $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)
	@$(MKDIR) $(PATH_BUILD)/autogen/$(PATH_INCLUDE)/$(PROGRAM_NAME)

################################################################################
# (Un)Installing
################################################################################

# DESTDIR: Support staged (un)installs.
# DESTDIR should be defined as a command line argument (thus it is empty by
# default).
#
#     $ make install DESTDIR=/tmp/stage
#     $ make uninstall DESTDIR=/tmp/stage
#
# Note: DESTDIR is different from PREFIX!
#
# Warning: PREFIX should have a leading '/' (the default does) when overriding
#          DESTDIR.

install:
	@$(PROGRESS) "install: $(BOLD)$(DESTDIR)$(BINDIR)/$(PROGRAM_NAME)$(NORMAL)" \
		&& $(INSTALL_PROGRAM) -D $(PATH_BIN)/$(PROGRAM_NAME) \
			$(DESTDIR)$(BINDIR)/$(PROGRAM_NAME)
	@$(PROGRESS) "install:" \
		"$(BOLD)$(DESTDIR)$(MANDIR)/man1/$(PROGRAM_NAME).1$(NORMAL)" \
		&& $(INSTALL_DATA) -D $(PATH_MAN)/man1/$(PROGRAM_NAME).1 \
			$(DESTDIR)$(MANDIR)/man1/$(PROGRAM_NAME).1

uninstall:
	@$(PROGRESS) "uninstall: $(BOLD)$(DESTDIR)$(BINDIR)/$(PROGRAM_NAME)$(NORMAL)" \
		&& $(RM) $(DESTDIR)$(BINDIR)/$(PROGRAM_NAME)
	@$(PROGRESS) "uninstall:" \
		"$(BOLD)$(DESTDIR)$(MANDIR)/man1/$(PROGRAM_NAME).1$(NORMAL)" \
		&& $(RM) $(DESTDIR)$(MANDIR)/man1/$(PROGRAM_NAME).1

################################################################################
# Cleaning
################################################################################

clean_version:
	@$(RM) $(PATH_OBJ)/version.o
	@$(RM) config/version.c
	@$(RM) $(PATH_BUILD)/autogen/$(PATH_SRC)/$(PROGRAM_NAME)/version.c

clean: clean_yacc clean_lex
	@$(RM) $(PATH_BIN) $(PATH_OBJ) $(PATH_LIB)
	@$(PROGRESS) "$(BOLD)$(YELLOW)Clean.$(NORMAL)"

cleanyacc: clean_yacc
clean_yacc:
	@$(RM) $(PATH_INCLUDE_YACC) $(PATH_SRC_YACC)
	@$(PROGRESS) "$(BOLD)$(LIGHT_YELLOW)Yacc files cleansed.$(NORMAL)"

cleanlex: clean_lex
clean_lex:
	@$(RM) $(PATH_INCLUDE_LEX) $(PATH_SRC_LEX)
	@$(PROGRESS) "$(BOLD)$(LIGHT_YELLOW)Lex files cleansed.$(NORMAL)"

cleandoc: clean_doc
clean_doc:
	@$(RM) $(PATH_DOC)
	@$(PROGRESS) "$(BOLD)$(LIGHT_ORANGE)Doc cleansed.$(NORMAL)"

cleanall: clean_all
clean_all: clean clean_doc
	@$(PROGRESS) "$(BOLD)$(ORANGE)Very clean.$(NORMAL)"

distclean: dist_clean
dist_clean: clean_all
	@$(RM) $(PATH_BUILD)
	@$(PROGRESS) "$(BOLD)$(DARK_ORANGE)Super clean.$(NORMAL)"

endif
