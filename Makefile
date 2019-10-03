#this is the make file for the Substation Systems Database, producing SSD.out

CC = gcc
CCFLAGS = -Wall -Werror -ansi -lm

SSD.out : Main.o Menus.o 
	$(CC) $(CCFLAGS) -o SSD.out Main.o Menus.o 

Main.o : Main.c Substation.h
	$(CC) $(CCFLAGS) -c -o Main.o Main.c

Menus.o : Menus.c Substation.h
	$(CC) $(CCFLAGS) -c -o Menus.o Menus.c

clean:
	rm SSD.out Main.o Menus.o

