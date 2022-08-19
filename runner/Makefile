## Makefile for Linux and Windows

SRCDIR=./src/
OBJDIR=./obj/
BINDIR=./bin/
DOCDIR=./doc/
TESTDIR=./testground/
TAB=$(shell echo -e '\t')

GIT_REV=$(shell git rev-parse HEAD 2> /dev/null)

$(shell echo 'void ehframe_dummy (void) { }' > $(BINDIR)/ehframe_dummy.C)

CXXFLAGS=-DGIT_REV="\"$(GIT_REV)\"" -Werror -Wall -Wextra -O3 -g3 -std=c++11
CXXPATHS=-I'./include/' -I'./include/boinc/' -I'./include/libzip/'
LDXXFLAGS=-lboinc_api -lboinc -lzip -lz

SRCS=$(shell find $(SRCDIR) -name *.cpp)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,.o,$(SRCS)))

linux: TARGET=runner.bin
linux: CXX=g++
linux: LDXXFLAGS+=-Wl,--whole-archive -lpthread -Wl,--no-whole-archive --static -L'./lib/x86_64_linux'
#this is necessary because of https://bugzilla.redhat.com/show_bug.cgi?id=199788#c2 (15 yrs old and still not fixed -.-')
linux: LDXXFLAGS+=-lc $(BINDIR)/ehframe_dummy.C
linux: $(OBJDIR) $(BINDIR) runner

windows: TARGET=runner.exe
windows: CXX=x86_64-w64-mingw32-g++
windows: LDXXFLAGS+=-lpthread --static -L'./lib/windows_x86_64/'
windows: $(OBJDIR) $(BINDIR) runner

mac: TARGET=runner.app
mac: CXX=clang++
mac: LDXXFLAGS+=-lpthread -L'./lib/darwin_arm64/'
mac: $(OBJDIR) $(BINDIR) runner

$(OBJDIR): 
	mkdir -p $@

$(BINDIR): 
	mkdir -p $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp  
	$(CXX) $(CXXPATHS) $(CXXFLAGS) -c $< -o $@  

.PHONY: runner
runner: $(OBJS)  
	$(CXX) -o $(BINDIR)$(TARGET) $(OBJS) $(CXXPATHS) $(LDXXFLAGS) $(CXXFLAGS)  
	cp $(BINDIR)$(TARGET) $(TESTDIR)$(TARGET)

.PHONY: doc
doc: 
	mkdir -p $@
	doxygen Doxyfile


.PHONY: clean
clean: cleanobj cleanbin cleandoc

.PHONY: cleanobj cleanbin cleandoc
cleanobj:
	rm -rf $(OBJDIR)*.o

cleanbin:
	rm -f $(TESTDIR)runner.*
	rm -f $(BINDIR)*.exe $(BINDIR)*.bin 

cleandoc:
	rm -rf $(DOCDIR)*

test_prince:
	cd testground; python3 prince_tester.py; cd --
