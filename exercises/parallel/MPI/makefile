# 
#  make         .... to build all exercises
#  make test    .... to run tests
#  make clean   .... to remove object files and executables 

include make.def

EXES= hello$(EXE) pi$(EXE) ring$(EXE)

RINGOBJS = ring.$(OBJ) ring_naive.$(OBJ)

all: $(EXES) $(MPIEXES)

pi$(EXE): pi.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi pi.$(OBJ) $(LIBS)

hello$(EXE): hello.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello hello.$(OBJ) $(LIBS)

ring$(EXE): $(RINGOBJS)
	$(CLINKER) $(OPTFLAGS) -o ring $(RINGOBJS) $(LIBS)

test: $(EXES)
	$(PRE)hello$(EXE);\
	$(PRE)pi$(EXE);\
	mpiexec -n 4 $(PRE)ring$(EXE)

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<
# 
#
#  I prefer this way of running the tests, but I couldn't figure
#  out how to cleaning mix regular executables with MPI execs.
#test: $(EXES)
#       for i in $(EXES); do \
#           $(PRE)$$i; \
#       done

