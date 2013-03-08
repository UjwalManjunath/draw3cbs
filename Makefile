CC = gcc 
DEBUG = -O


CFLAGS = $(DEBUG) 


LIBS =  -lXm -lXt -lX11 -lm

TARGETS=  draw3cbs

all: $(TARGETS)

clean:
	rm -f $(TARGETS) a.out core *.o \#*

draw3cbs: draw3cbs.o
	$(CC) $(CFLAGS) -o $@ $@.o $(LIBS)



clobber:
	rm -f *.o *~* *.a 
	rm -f $(TARGETS)

