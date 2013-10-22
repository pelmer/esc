# 
#  makefile to build OpenACC exercises
#
#      make         ... build the programs for the exercises
#      make test    ... test all the exercises
#      make clean   ... remove object files and executables

include make.def

COMMON      = Common

EXES=vadd$(EXE) jac_solv$(EXE)

COMM_OBJS = $(COMMON)/wtime.$(OBJ) 

all: $(EXES)

vadd$(EXE): vadd.$(OBJ) $(COMM_OBJS)
	$(CLINKER) $(OPTFLAGS) -o vadd vadd.$(OBJ) $(COMM_OBJS) $(LIBS)

jac_solv$(EXE): jac_solv.$(OBJ) frandom.$(OBJ) $(COMM_OBJS)
	$(CLINKER) $(OPTFLAGS) -o jac_solv jac_solv.$(OBJ) frandom.$(OBJ) \
                   $(COMM_OBJS) $(LIBS)

$(COMMON)/wtime.$(OBJ): $(COMMON)/wtime.c
	$(CC) -c $(CFLAGS) -o $(COMMON)/wtime.$(OBJ)  $(COMMON)/wtime.c

test: $(EXES)
	$(PRE)vadd$(EXE)
	$(PRE)jac_solv$(EXE)

clean:
	$(RM) $(EXES) *.$(OBJ) $(COMMON)/*.$(OBJ)

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
#
