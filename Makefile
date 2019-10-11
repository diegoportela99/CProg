#This is the make file for the Substation Systems Database, producing SSD.out

CC = gcc
CCFLAGS = -Wall  -ansi -lm

SSD.out : Main.o Menus.o Import.o Export.o Search.o btree.o Compression.o debug.o loadsave.o chunk.o block.o crypto.o
	$(CC) $(CCFLAGS) -o SSD.out Main.o Menus.o Import.o Export.o Search.o btree.o Compression.o debug.o loadsave.o chunk.o block.o crypto.o

Main.o : Main.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Main.o Main.c

Menus.o : Menus.c Substation.h Compression.h loadsave.o btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Menus.o Menus.c
	
Search.o : Search.c btree.h btree.o Substation.h
	$(CC) $(CCFLAGS) -c -o Search.o Search.c
    
Import.o : Import.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Import.o Import.c

Export.o : Export.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Export.o Export.c

btree.o : btree.c btree.h debug.h
	$(CC) $(CCFLAGS) -c -o btree.o btree.c

debug.o : debug.c debug.h
	$(CC) $(CCFLAGS) -c -o debug.o debug.c

Compression.o : Compression.c Substation.h btree.h btree.o Compression.h debug.o
	$(CC) $(CCFLAGS) -c -o Compression.o Compression.c

loadsave.o : loadsave.c crypto.o chunk.o block.o debug.o btree.o Substation.h Search.o
	$(CC) $(CCFLAGS) -c -o loadsave.o loadsave.c

crypto.o : crypto.c debug.o
	$(CC) $(CCFLAGS) -c -o crypto.o crypto.c

chunk.o : chunk.c block.o debug.o btree.o
	$(CC) $(CCFLAGS) -c -o chunk.o chunk.c

block.o : block.c block.h btree.o debug.o 
	$(CC) $(CCFLAGS) -c -o block.o block.c

clean:
	rm SSD.out Main.o Menus.o Import.o Export.o Search.o btree.o Compression.o debug.o loadsave.o crypto.o chunk.o block.o
	
