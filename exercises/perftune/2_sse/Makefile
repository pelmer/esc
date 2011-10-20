
####### Compiler, tools and options

CC            = gcc
CXX           = g++
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -O2  -I/opt/intel/tbb/2.0/include
LINK          = g++
LFLAGS        = -Wl,--as-needed
LIBS          = -L/usr/lib 
TBB_LIBS      = -ltbb -L/usr/lib -I/usr/include -L/opt/intel/tbb/2.0/em64t/cc4.1.0_libc2.4_kernel2.6.16.21/lib
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = Fit.cxx Fit.h FitClasses.h
BINARIES      = single tbb scalar double
DESTDIR       = 

default: single

####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

# The original
single: P4_F32vec4.h $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LIBS) -o "$@" Fit.cxx

tbb: $(SOURCES)
	$(CXX) -DTBB $(CXXFLAGS) $(LIBS) $(TBB_LIBS) -o "$@" Fit.cxx

scalar: $(SOURCES)
	$(CXX) -DX87 $(CXXFLAGS) $(LIBS) $(TBB_LIBS) -o "$@" Fit.cxx

double: openlab_mod/P4_F64vec2.h $(SOURCES)
	$(CXX) -DDOUBLE_PRECISION $(CXXFLAGS) $(LIBS) -o "$@" Fit.cxx

double_debug: openlab_mod/P4_F64vec2.h Fit.cxx FitClasses.h Fit.h
	$(CXX) -DDOUBLE_PRECISION -g -o "$@" Fit.cxx

debug: openlab_mod/P4_F64vec2.h Fit.cxx FitClasses.h Fit.h
	$(CXX) -g -o "$@" Fit.cxx

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

all: $(BINARIES)

.tar.bz2: clean
	tar cj --exclude=hltsse_openlab"$@" --exclude=.*.swp -C .. hltsse > hltsse_openlab"$@"

clean:
	-$(DEL_FILE) $(BINARIES) hltsse_openlab.tar.bz2


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile



compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 



install:  install_target install_sources  FORCE

uninstall: uninstall_target uninstall_sources   FORCE

FORCE:

