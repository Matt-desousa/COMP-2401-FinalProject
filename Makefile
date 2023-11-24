TARGETS=main house logger utils defs

all: 	${TARGETS}
		gcc -o finalProkject main.o house.o logger.o utils.o

main:	main.c defs.h
		gcc -c main.c

house:	house.c defs.h
		gcc -c house.c

logger: logger.c defs.h
		gcc -c logger.c

utils: utils.c defs.h
		gcc -c utils.c

clean:
		rm -f ${TARGETS}