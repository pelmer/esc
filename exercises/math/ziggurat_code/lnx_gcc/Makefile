all : compile link
  
CC       = gcc
CFLAGS   = -m32 -O2 -Wall -I..
CPATH    = ..

%.o : $(CPATH)/%.c  
	$(CC) -c $(CFLAGS) $<

OBJECTS0 = zigrandom.o zigtimer.o zignor.o rnorrexpx.o rnor_mwc8222.o rnal.o FastNorm3x.o
OBJECTS1 = timings.o

compile : $(OBJECTS0) $(OBJECTS1)

link :
	$(CC) -m32 -o timings $(OBJECTS0) $(OBJECTS1) -lm
