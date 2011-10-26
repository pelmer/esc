# 
include ../make.def

EXES=hello_par$(EXE) pi_spmd_simple$(EXE) pi_spmd_final$(EXE) \
     pi_mc_par_array$(EXE) \
     pi_loop$(EXE) pi_mc$(EXE) pi_mc_par$(EXE) \
     matmul_par$(EXE) prod_cons_par$(EXE) linked_omp3_tasks$(EXE) \
     linked_omp25$(EXE) linked_cpp$(EXE) mandel_par$(EXE)
all: $(EXES)

pi_mc_par_array$(EXE): pi_mc_par_array.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_mc_par_array pi_mc_par_array.$(OBJ) $(LIBS)

pi_mc_par$(EXE): pi_mc_par.$(OBJ) random_par.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_mc_par pi_mc_par.$(OBJ) random_par.$(OBJ) $(LIBS)

hello_par$(EXE): hello_par.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o hello_par hello_par.$(OBJ) $(LIBS)

pi_spmd_simple$(EXE): pi_spmd_simple.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_spmd_simple pi_spmd_simple.$(OBJ) $(LIBS)

pi_spmd_final$(EXE): pi_spmd_final.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_spmd_final pi_spmd_final.$(OBJ) $(LIBS)

pi_loop$(EXE): pi_loop.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o pi_loop pi_loop.$(OBJ) $(LIBS)

pi_mc$(EXE): pi_mc.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o pi_mc pi_mc.$(OBJ) $(LIBS)

matmul_par$(EXE): matmul_par.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o matmul_par matmul_par.$(OBJ) $(LIBS)

prod_cons_par$(EXE): prod_cons_par.$(OBJ)
	$(CLINKER) $(OPTFLAGS) -o prod_cons_par prod_cons_par.$(OBJ) $(LIBS)

linked_omp3_tasks$(EXE): linked_omp3_tasks.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o linked_omp3_tasks linked_omp3_tasks.$(OBJ) $(LIBS)

linked_cpp$(EXE): linked_cpp.$(OBJ) 
	$(CPP) $(OPTFLAGS) -o linked_cpp linked_cpp.$(OBJ) $(LIBS)

linked_omp25$(EXE): linked_omp25.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o linked_omp25 linked_omp25.$(OBJ) $(LIBS)

mandel_par$(EXE): mandel_par.$(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o mandel_par mandel_par.$(OBJ) $(LIBS)

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp  .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CPP) $(CFLAGS) -c $<
