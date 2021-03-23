all: fe3 ex2c count

fe3: fe3.c
	gcc -Wall -lpthread -o fe3 fe3.c 

ex2c: ex2c.c
	gcc -Wall -lpthread -o ex2c ex2c.c

count: count.c
	 gcc -Wall count.c -o count -lpthread
	

clean:
	rm -f *.o fe3
	rm -f *.o ex2c
	rm -f *.o count