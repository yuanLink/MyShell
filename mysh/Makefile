# all: mysh

mysh: sh.o mysys.o
	gcc -Wall sh.o mysys.o -o mysh

sh.o: sh.c
	gcc -c -Wall sh.c

sh1.o: sh1.c
	gcc -c -Wall sh1.c

mysys.o: mysys.c
	gcc -c -Wall mysys.c

clean:
	rm -rf *.o mysh
