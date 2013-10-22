# 
#    makefile to build solutions to the OpenMP exercises
#
#       make           .... build the programs for each solution
#       make test      .... test the solutions
#       make clean     .... remove object files and executables

include ../make.def

EXES=hello_par$(EXE) pi_spmd_simple$(EXE) pi_spmd_final$(EXE) \
     pi_loop$(EXE) pi_loop_min_change$(EXE) pi_task$(EXE) matmul_par$(EXE)

all: $(EXES)

hello_par$(EXE): hello_par.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello_par hello_par.$(OBJ) $(LIBS)

pi_spmd_simple$(EXE): pi_spmd_simple.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_spmd_simple pi_spmd_simple.$(OBJ) $(LIBS)

pi_spmd_final$(EXE): pi_spmd_final.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_spmd_final pi_spmd_final.$(OBJ) $(LIBS)

pi_loop$(EXE): pi_loop.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_loop pi_loop.$(OBJ) $(LIBS)

pi_loop_min_change$(EXE): pi_loop_min_change.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_loop_min_change pi_loop_min_change.$(OBJ) $(LIBS)

pi_task$(EXE): pi_task.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_task pi_task.$(OBJ) $(LIBS)

matmul_par$(EXE): matmul_par.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o matmul_par matmul_par.$(OBJ) $(LIBS)

test: $(EXES)
	$(PRE)hello_par$(EXE)
	$(PRE)pi_spmd_simple$(EXE)
	$(PRE)pi_spmd_final$(EXE)
	$(PRE)pi_loop$(EXE)
	$(PRE)pi_loop_min_change$(EXE)
	$(PRE)pi_task$(EXE)
	$(PRE)matmul_par$(EXE)

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp  .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<

#test: $(EXES)
#	for i in $(EXES); do \
#            $(PRE)$$i; \
#        done
