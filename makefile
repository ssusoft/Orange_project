orange : main.o writelist.o actor.o director.o movie.o command.o start.o 
	gcc -o orange movie.o actor.o director.o command.o writelist.o start.o main.o
movie.o : header.h movie.c
	gcc -c movie.c
actor.o : header.h actor.c
	gcc -c actor.c
director.o : header.h director.c
	gcc -c director.c
command.o : header.h command.c
	gcc -c command.c
writelist.o : header.h writelist.c
	gcc -c writelist.c
start.o : header.h start.c
	gcc -c start.c
main.o : header.h main.c
	gcc -c main.c
