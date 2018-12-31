all:main readfile writefile
.PHONY:clean

main:main.o sem.o
	gcc -Wall -g main.o sem.o -o main
readfile:readfile.o sem.o
	gcc -Wall -g readfile.o sem.o -o readfile
writefile:writefile.o sem.o
	gcc -Wall -g writefile.o sem.o -o writefile	
main.o:main.c
	gcc -Wall -g -c main.c -o main.o
sem.o:sem.c
	gcc -Wall -g -c sem.c -o sem.o
clean:
	rm -f *.o main readfile writefile