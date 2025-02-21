# Makefile to compile UMIX Programming Assignment 3 (pa3) [updated 12/31/21]

LIBDIR = $(UMIXPUBDIR)/lib
LIB = $(LIBDIR)/libumix3.a

CC 	= cc 
FLAGS 	= -g -m32 -L$(LIBDIR) -lumix3

PA3 =	pa3a pa3b pa3c test_fifo test_lifo test_rr test_prop

pa3:	$(PA3)

pa3a:	pa3a.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o pa3a pa3a.c mycode3.o

pa3b:	pa3b.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o pa3b pa3b.c mycode3.o

pa3c:	pa3c.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o pa3c pa3c.c mycode3.o

test_fifo:	test_fifo.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o test_fifo test_fifo.c mycode3.o

test_lifo:	test_lifo.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o test_lifo test_lifo.c mycode3.o

test_rr:	test_rr.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o test_rr test_rr.c mycode3.o

test_prop:	test_prop.c aux.h umix.h mycode3.o $(LIB)
	$(CC) $(FLAGS) -o test_prop test_prop.c mycode3.o

mycode3.o:	mycode3.c aux.h sys.h mycode3.h
	$(CC) $(FLAGS) -c mycode3.c

clean:
	rm -f *.o $(PA3)
