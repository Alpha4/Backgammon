all: lib

clean:
	rm -rf *.o
	rm -rf *.so

lib:
	gcc -fPIC -c bot.c ../src/liste.c -Wall
	gcc -shared bot.o liste.o -o libalta.so -Wall