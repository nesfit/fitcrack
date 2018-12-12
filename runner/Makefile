## Makefile for Linux and Windows

SRCDIR=./src/
OBJDIR=./obj/
BINDIR=./bin/
DOCDIR=./doc/
TESTDIR=./testground/
TAB=$(shell echo -e '\t')

CXXFLAGS=--static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -pedantic -lboinc_api -lboinc -lzip -lz -static-libgcc -static-libstdc++ 
CXXPATHS=-I'./include/' -I'./include/boinc/' -I'./include/libzip/'  
## Release
LDXXFLAGS=-O3 -std=c++98 #-D_GLIBCXX_USE_CXX11_ABI=0
## Debug
# LDXXFLAGS=-pg -std=c++98 #-D_GLIBCXX_USE_CXX11_ABI=0

SRCS=$(shell find $(SRCDIR) -name *.cpp)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,.o,$(SRCS)))

linux: TARGET=runner.bin
linux: CXX=g++-4.8
linux: CXXPATHS+=-L'./lib/x86_64_linux'
linux: ARCH=-m64
linux: $(OBJDIR) $(BINDIR) runner

windows: TARGET=runner.exe
windows: CXX=x86_64-w64-mingw32-g++
windows: ARCH=
windows: CXXPATHS+=-L'./lib/windows_x86_64/'
windows: $(OBJDIR) $(BINDIR) runner

$(OBJDIR): 
	mkdir $@

$(BINDIR): 
	mkdir $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp  
	$(CXX) $(CXXPATHS) $(LDXXFLAGS) -c $< -o $@  

.PHONY: runner
runner: $(OBJS)  
	$(CXX) -o $(BINDIR)$(TARGET) $(ARCH) $(OBJS) $(CXXPATHS) $(LDXXFLAGS) $(CXXFLAGS)  
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
