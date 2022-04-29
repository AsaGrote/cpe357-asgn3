CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=gnu99
MAIN = smake
OBJ = main.o func.o rule.o list.o
all: $(MAIN)

$(MAIN): $(OBJ)
	$(CC) $(CLFAGS) -o $(MAIN) $(OBJ)

main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c

func.o: func.c func.h
	$(CC) $(CFLAGS) -c func.c

rule.o: rule.c rule.h
	$(CC) $(CFLAGS) -c rule.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

debug:
	gcc -g main.c func.c rule.c list.c

clean:
	rm *.o $(MAIN)
