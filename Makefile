CC = gcc
CFLAGS = -Wall -Werror -pedantic -ansi
MAIN = smake
OBJ = main.o func.o rule.o list.o
all: $(MAIN)

$(MAIN): $(OBJ)
	$(CC) $(CLFAGS) -g -o $(MAIN) $(OBJ)

main.o: main.c main.h func.h rule.h list.h
	$(CC) $(CFLAGS) -g -c main.c

func.o: func.c func.h
	$(CC) $(CFLAGS) -g -c func.c

rule.o: rule.c rule.h
	$(CC) $(CFLAGS) -g -c rule.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -g -c list.c

debug:
	gcc -g main.c func.c rule.c list.c

clean:
	rm *.o $(MAIN)
