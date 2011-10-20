ICC=icc
GCC=gcc
CC=$(GCC)

OPT=
O2=$(OPT) -O2
O3=$(OPT) -O3

TARGETS=linkedthrash linkedthrash-gcc-o2 linkedthrash-icc-o2

all: $(TARGETS)

linkedthrash: linkedthrash.c
	$(CC) $@.c -g -o $@

linkedthrash-gcc-o2: linkedthrash.c
	$(GCC) $(O2) linkedthrash.c -o $@

linkedthrash-icc-o2: linkedthrash.c
	$(ICC) $(O2) linkedthrash.c -o $@

clean:
	rm -f *.o core.* *~ $(TARGETS)
