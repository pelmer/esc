# 
#  makefile to build OpenMP exercises
#
#      make         ... build the programs for the exercises
#      make test    ... test all the exercises
#      make clean   ... remove object files and executables

include make.def

EXES=hello$(EXE) pi$(EXE) matmul$(EXE) 

all: $(EXES)

hello$(EXE): hello.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello hello.$(OBJ) $(LIBS)

pi$(EXE): pi.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi pi.$(OBJ) $(LIBS)

pi_mc$(EXE): pi_mc.$(OBJ) random.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o pi_mc random.$(OBJ) pi_mc.$(OBJ) $(LIBS)

matmul$(EXE): matmul.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o matmul matmul.$(OBJ) $(LIBS)

test: $(EXES)
	$(PRE)hello$(EXE)
	$(PRE)pi$(ESE)
	$(PRE)matmul$(EXE)

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<

#test: $(EXES)
#	for i in $(EXES); do \
#            $(PRE)$$i; \
#        done
