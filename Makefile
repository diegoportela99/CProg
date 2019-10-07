#this is the make file for the Substation Systems Database, producing SSD.out

CC = gcc
CCFLAGS = -Wall -Werror -ansi -lm

SSD.out : Main.o Menus.o Import.o Export.o btree.o
	$(CC) $(CCFLAGS) -o SSD.out Main.o Menus.o Import.o Export.o btree.o

Main.o : Main.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Main.o Main.c

Menus.o : Menus.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Menus.o Menus.c

Import.o : Import.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Import.o Import.c

Export.o : Export.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Export.o Export.c

btree.o : btree.c btree.h debug.h
	$(CC) $(CCFLAGS) -c -o btree.o btree.c

clean:
	rm SSD.out Main.o Menus.o Import.o Export.o btree.o
