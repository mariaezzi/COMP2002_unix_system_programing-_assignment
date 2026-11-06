CC = gcc
#CFLAGS = -Wall -Wextra -ansi -pedantic -g
CFLAGS = -g
OBJ = escape.o fileHandling.o game.o newSleep.o random.o terminal.o
EXEC = escape

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

escape.o : escape.c 
	$(CC) $(CFLAGS) -c escape.c

fileHandling.o : fileHandling.c fileHandling.h
	$(CC) $(CFLAGS) -c fileHandling.c

game.o : game.c game.h
	$(CC) $(CFLAGS) -c game.c

newSleep.o : newSleep.c newSleep.h
	$(CC) $(CFLAGS) -c newSleep.c

random.o : random.c random.h
	$(CC) $(CFLAGS) -c random.c

terminal.o : terminal.c terminal.h
	$(CC) $(CFLAGS) -c terminal.c

clean :
	rm -f $(OBJ) $(EXEC) state.txt