CC = gcc
#CFLAGS = -Wall -Wextra -ansi -pedantic -g
CFLAGS = -g
OBJ = escape.o fileHandling.o
EXEC = escape

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

escape.o : escape.c 
	$(CC) $(CFLAGS) -c escape.c

fileHandling.o : fileHandling.c fileHandling.h
	$(CC) $(CFLAGS) -c fileHandling.c

clean :
	rm -f $(OBJ) $(EXEC) state.txt