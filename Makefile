CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu99
MAIN = smake
OBJ = main.o rule.o list.o
all: $(MAIN)

$(MAIN): $(OBJ)
	$(CC) $(CLFAGS) -g -o $(MAIN) $(OBJ)

main.o: main.c main.h rule.h list.h
	$(CC) $(CFLAGS) -g -c main.c

rule.o: rule.c rule.h
	$(CC) $(CFLAGS) -g -c rule.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -g -c list.c

debug:
	gcc -g main.c rule.c list.c

clean:
	rm *.o $(MAIN)
