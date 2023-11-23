TARGETS=main house logger utils defs

all:		${TARGETS}



main:	main.c
		gcc -pthread -o p1 p1-prime.c -lm

house:	house.c
		gcc -g -pthread -o p2 p2-fruits.c

logger:

utils:

defs:

clean:
			rm -f ${TARGETS}