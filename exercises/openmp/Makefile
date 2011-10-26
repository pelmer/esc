# 

include make.def

EXES=hello$(EXE) pi$(EXE) matmul$(EXE) pi_mc$(EXE) prod_cons$(EXE) linked$(EXE) mandel$(EXE)

all: $(EXES)

hello$(EXE): hello.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello hello.$(OBJ) $(LIBS)

pi$(EXE): pi.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi pi.$(OBJ) $(LIBS)

pi_mc$(EXE): pi_mc.$(OBJ) random.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o pi_mc random.$(OBJ) pi_mc.$(OBJ) $(LIBS)

matmul$(EXE): matmul.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o matmul matmul.$(OBJ) $(LIBS)

prod_cons$(EXE): prod_cons.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o prod_cons prod_cons.$(OBJ) $(LIBS)

linked$(EXE): linked.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o linked linked.$(OBJ) $(LIBS)

mandel$(EXE): mandel.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o mandel mandel.$(OBJ) $(LIBS)

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .$(OBJ)

.c.$(OBJ):
	$(CC) $(OPTFLAGS) $(CFLAGS) -c $<
