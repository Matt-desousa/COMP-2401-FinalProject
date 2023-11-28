TARGETS = main house logger utils

all: 	defs.h main.c house.c logger.c utils.c room.c evidence.c hunter.c ghost.c
		gcc -o fp main.c house.c logger.c utils.c room.c evidence.c hunter.c ghost.c -lpthread

# main:	main.c defs.h
# 		gcc -c main.c

# house:	house.c defs.h
# 		gcc -c house.c

# logger: logger.c defs.h
# 		gcc -c logger.c

# utils: utils.c defs.h
# 		gcc -c utils.c