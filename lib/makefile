all: lib

clean:
	rm -rf *.o
	rm -rf *.so

lib:
	gcc -fPIC -c NOMDEVOTREIA.c -Wall
	gcc -shared NOMDEVOTREIA.o -o NOMDEVOTREIA.so -Wall