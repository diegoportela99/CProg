#This is the make file for the Substation Systems Database, producing SSD.out

CC = gcc
CCFLAGS = -Wall  -ansi -lm

SSD.out : Main.o Menus.o Import.o Export.o Search.o btree.o Compression.o debug.o
	$(CC) $(CCFLAGS) -o SSD.out Main.o Menus.o Import.o Export.o Search.o btree.o Compression.o debug.o

Main.o : Main.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Main.o Main.c

Menus.o : Menus.c Substation.h Compression.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Menus.o Menus.c
	
Search.o : Search.c btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Search.o Search.c
    
Import.o : Import.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Import.o Import.c

Export.o : Export.c Substation.h btree.h btree.o
	$(CC) $(CCFLAGS) -c -o Export.o Export.c

btree.o : btree.c btree.h debug.h
	$(CC) $(CCFLAGS) -c -o btree.o btree.c

debug.o : debug.c debug.h
	$(CC) $(CCFLAGS) -c -o debug.o debug.c

Compression.o : Compression.c Substation.h btree.h btree.o Compression.h
	$(CC) $(CCFLAGS) -c -o Compression.o Compression.c

clean:
	rm SSD.out Main.o Menus.o Import.o Export.o Search.o btree.o Compression.o debug.o
	
