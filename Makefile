program :main.o
	cc main.o -o program
main.o	:main.c
	cc  -c main.c -lm
