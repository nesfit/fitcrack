#Dependency generation and general outline taken from
#http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#compiler to use. Assumes flag compatibility with g++
#COMPILER=~/builds/llvm-3.4.2-build/bin/clang
COMPILER=$(CXX)
#directory to store dependency files
DEPDIR=dep
#source file directory
SRCDIR=..
#directory for object files
OBJDIR=obj
#include directory, mostly for header files
INCDIR=../headers
#static lib name
OBJLIB=$(OBJDIR)/objlib.a
#source file extension
SRCEXT=cpp
#library directories
LIBDIRFLAGS=
#linker flags
LDFLAGS=-pthread
#flags to include necessary directories
INCFLAGS=-iquote $(INCDIR)/Utils
#flags for compilation
CXXFLAGS=-std=c++11 -Wall -Wextra -fPIC -pthread

TESTDATADIR=$(shell pwd)/testdata
TESTDATADIR_QUOTED=\"$(TESTDATADIR)\"

#configuration ends here, from here it's necessary
#for this Makefile to work as intended

#list of all source files
TEST_SRCFILES=$(shell find ../tests -name '*.$(SRCEXT)')
SRCFILES=$(TEST_SRCFILES) ../source/Utils/MaskSplitter.cpp
#list of all subdirectories of SRCDIR without duplications
SRC_DIR_TREE=$(shell echo $(dir $(SRCFILES)) | sort -u)
#duplications of source directory tree in OBJDIR and DEPDIR
OBJ_DIR_TREE=$(patsubst $(SRCDIR)%,$(OBJDIR)%,$(SRC_DIR_TREE))
DEP_DIR_TREE=$(patsubst $(SRCDIR)%,$(DEPDIR)%,$(SRC_DIR_TREE))
#makes sure that DEPDIR and OBJDIR exist
$(shell mkdir -p $(DEP_DIR_TREE))
$(shell mkdir -p $(OBJ_DIR_TREE))
#compiler flags to automatically generate dependencies during compilation
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
#command to run after compilation. Moves generated dep files in case the compilation succeeded
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $(OBJDIR)/$*.o
#list of all object files
OFILES = $(patsubst $(SRCDIR)/%.$(SRCEXT),$(OBJDIR)/%.o,$(SRCFILES))
TEST_OFILES = $(patsubst $(SRCDIR)/%.$(SRCEXT),$(OBJDIR)/%.o,$(TEST_SRCFILES))

#the default target
default: debug #tests
	
debug: CXXFLAGS+=-g
debug: tests

test: tests
	./tests

#this is suitable for one executable build
#in case you wish to build multiple executables,
#just create a rule for each executable,
#each the same as this one, but adding the name
#of the object file with the corresponding main
#function before $(ARCHNAME).
#For example if main is in file app.cpp, the rule
#may look like this:
#app: $(OBJDIR)/app.o $(ARCHNAME)

#linking of the application
tests: CXXFLAGS+=-g
tests: LIBDIRFLAGS+=
tests: LDFLAGS+=-lgtest
tests: $(TEST_OFILES) $(OBJLIB)
	$(COMPILER) $^ -o $@ $(LIBDIRFLAGS) $(LDFLAGS)

$(OBJLIB): $(OFILES)
	ar rsT $@ $^

#define test data directory for unit test sources
#$(OBJDIR)/tests/%.o: INCFLAGS+= \
	gtest
$(OBJDIR)/tests/%.o: CXXFLAGS+= -DTESTDATA_DIR=$(TESTDATADIR_QUOTED)
	
#compilation of object files along with dependency generation
#the .d dependency is here so that dependencies are rebuilt
#in case they are deleted
$(OBJDIR)/%.o : $(SRCDIR)/%.$(SRCEXT) $(DEPDIR)/%.d
	$(COMPILER) $(CXXFLAGS) $(INCFLAGS) $(DEPFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

#this is here so that make doesn't freak out when a .d file
#does not exist
$(DEPDIR)/%.d: ;

.PHONY: clean install

clean:
	@rm -rf $(OBJDIR) $(DEPDIR) $(LIBNAME) tests

#this includes the .d files generated in compilation
#they contain rules in format name.o: srcfile headers
#also header:
#which makes sure that make doesn't throw a fit about not knowing
#how to generate header files that were deleted
-include $(patsubst $(OBJDIR)/%,$(DEPDIR)/%.d,$(basename $(OFILES)))
