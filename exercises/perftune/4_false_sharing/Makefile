ICC=icc
GCC=gcc
CC=$(GCC)

OPT=
O0=$(OPT) -O0
O2=$(OPT) -O2
O3=$(OPT) -O3

LINK=-lpthread

TARGETS=falseshare falseshare-gcc-o2 falseshare-icc-o2

all: $(TARGETS)

falseshare: falseshare.c
	$(CC) $(LINK) $(O0) $@.c -o $@

falseshare-gcc-o2: falseshare.c
	$(GCC) $(O2) $(LINK) falseshare.c -o $@

falseshare-icc-o2: falseshare.c
	$(ICC) $(O2) $(LINK) falseshare.c -o $@


clean:
	rm -f *.o core.* *~ $(TARGETS)
