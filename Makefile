program :main.o
	cc main.o -lm -o program
main.o	:main.c
	cc  -c main.c -lm
