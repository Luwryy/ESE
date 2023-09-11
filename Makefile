#enseash : main.o REPL.o signal.o
#	gcc -o enseash main.o REPL.o signal.o
#main.o : main.c
#	gcc -c -Wall main.c -g
#REPL.o : REPL.c
#	gcc -c -Wall REPL.c -g
#signal.o : signal.c
#	gcc -c -Wall signal.c -g

rappel : rappel.o
	gcc -o rappel rappel.o

rappel.o : rappel.c
	gcc -c -Wall rappel.c -g


