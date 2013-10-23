import os,sys

build_dirs = {
	'CUDA/Solutions'   : ['vadd','matmul'],
	'OpenCL/Solutions' : ['vadd','matmul'],
	'MPI/Solutions'    : ['vadd','matmul'],
	'OpenMP/Solutions' : ['vadd','matmul'],

	'CUDA'   : ['vadd','matmul'],
        'OpenCL' : ['vadd','matmul'],
        'MPI'    : ['vadd','matmul'],
        'OpenMP' : ['vadd','matmul'],

        'OpenACC'          : ['vadd','matmul'],
}

keys = build_dirs.keys()
if len(sys.argv) > 1:
	keys = sys.argv[1:]

for dir in keys:

        exes = build_dirs[dir]
	os.system('cd %s && make clean > /dev/null'%dir)
        retval = os.system('cd %s && make  2> /dev/null > /dev/null'  % dir)

        print('compiled %s: ret=%d'%(dir,retval))

print '\n\n\running make test for all dirs..\n\n'

for dir in keys:
	
	exes = build_dirs[dir]
	retval = os.system('cd %s && make clean > /dev/null && make  2> /dev/null > /dev/null'  % dir)
	
	print('compiled %s: ret=%d'%(dir,retval))
	
	if retval == 0:
		retval = os.system('cd %s && make test 2> /dev/null > /dev/null'  % (dir))
		print('testing %s: ret=%d'%(dir,retval))
		
print '\n\ncleaning..\n\n'

for dir in keys:
        retval = os.system('cd %s && make clean'%dir)
