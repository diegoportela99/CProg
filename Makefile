#this is the make file for the Substation Systems Database, producing SSD.out

CC = gcc
CCFLAGS = -Wall -Werror -ansi -lm

SSD.out : start_menu.o file_menu.o import_data.o load_data.o LinkedList.o search_data.o search_plant.o search_desig.o search_module.o export_csv.o save_data.o
	$(CC) $(CCFLAGS) -o SSD.out start_menu.o file_menu.o import_data.o load_data.o search_data.o search_plant.o search_desig.o search_module.o export_csv.o save_data.o

start_menu.out : 

file_menu.out :

import_data.out : 

load_data.out :

LinkedList :

search_data.out :

search_plant.out :

search_desig.out :

search_module.out :

export_csv.out :

save_data.out :
