#This is the make file for the Substation Systems Database, producing SSD.out

CC = gcc
CCFLAGS = -Wall -Werror -ansi -lm

SSD.out : Main.o Menus.o Import.o Export.o Search.o btree.o chunk.o block.o crypto.o cryptohelper.o debug.o
	$(CC) $(CCFLAGS) -o SSD.out Main.o Menus.o Import.o Export.o Search.o btree.o chunk.o block.o crypto.o cryptohelper.o debug.o

Main.o : Main.c Substation.h btree.o
	$(CC) $(CCFLAGS) -c -o Main.o Main.c

Menus.o : Menus.c Substation.h chunk.o btree.o  block.o
	$(CC) $(CCFLAGS) -c -o Menus.o Menus.c 
	
Search.o : Search.c btree.o
	$(CC) $(CCFLAGS) -c -o Search.o Search.c
    
Import.o : Import.c Substation.h btree.o
	$(CC) $(CCFLAGS) -c -o Import.o Import.c

Export.o : Export.c Substation.h btree.o
	$(CC) $(CCFLAGS) -c -o Export.o Export.c

crypto.o : crypto.c cryptohelper.o debug.o block.o
	$(CC) $(CCFLAGS) -c -o crypto.o crypto.c

cryptohelper.o: cryptohelper.c cryptohelper.h debug.o
	$(CC) $(CCFLAGS) -c -o cryptohelper.o cryptohelper.c

chunk.o : chunk.c block.o debug.o
	$(CC) $(CCFLAGS) -c -o chunk.o chunk.c

block.o : block.c block.h btree.o debug.o 
	$(CC) $(CCFLAGS) -c -o block.o block.c

btree.o : btree.c btree.h debug.o
	$(CC) $(CCFLAGS) -c -o btree.o btree.c

debug.o : debug.c debug.h
	$(CC) $(CCFLAGS) -c -o debug.o debug.c

clean:
	rm SSD.out Main.o Menus.o Import.o Export.o Search.o btree.o block.o chunk.o crypto.o cryptohelper.o debug.o
