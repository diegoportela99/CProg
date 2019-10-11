/*******************************************************************************
* Search functions
* Developer: Owen Dowley, Michael Lardner
* Student ID: 13234505, 12893602
* Description: This file contains for searching the btree memory structure.
* Functions required in top level from this file are prototyped in Substation.h
*******************************************************************************/

#include "btree.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Substation.h"

/*******************************************************************************
 * Function prototypes
*******************************************************************************/

void search_btree(root_t* root, int search_criteria, char search_string[],
int argc);

void print_search_header(int argc);

/*******************************************************************************
 * Function definitions
*******************************************************************************/

/******************************************************************************* 
 * This function searches a root_p by a search criteria and an a
 * inputs:
 * - root_p | The root of the current memory structure
 * - search criteria | which sort of search we're performing, int
 * - argc | Argument, passed down from main
 * outputs:
 * - none
*******************************************************************************/
void search(root_t* root_p, int search_criteria, int argc) {
    if(root_p->number_of_entries == 0) {
        printf("Please load data before attempting to search.\n");
        return;
    }

    char search_type[3][7] = {"plant","desig","module"};
    char user_input[BUFSIZ];
    char search_string[BUFSIZ];

    printf("Please enter %s: ", search_type[search_criteria-1]);
    fgets(user_input, BUFSIZ, stdin);
    sscanf(user_input," %s ",search_string);
    search_btree(root_p,search_criteria-1,search_string, argc);
}

void print_telemetry_point(telemetry_point_t* telemetry_point) {
	printf (
    "%9s|%18s|%22s|%14s|%28s|%4s|%2s|%5s|%7s|%1s|%1s|%1s|%1s\n",
    (*telemetry_point).location,
	(*telemetry_point).desig,
	(*telemetry_point).plant,
	(*telemetry_point).network,
	(*telemetry_point).quantity,
	(*telemetry_point).protocol,
	(*telemetry_point).number,
	(*telemetry_point).address,
	(*telemetry_point).moduletype,
	(*telemetry_point).failed,
	(*telemetry_point).online,
	(*telemetry_point).faulty,
	(*telemetry_point).oos
    );
}

void print_search_header(int argc) {
    /* Checks if the user is in a colour mode */
    if (argc <= 1) {
        printf(TEXTRED(0));
    }
    printf("                                                                  "
    "                                                    F|O|F|O\n");
    printf("                                                                  "
        "                                                    A|N|A|O\n");
    printf(" Location|             Desig|                 Plant|       Network|"
    "                    Quantity|Prot| #|Addrs| Module|I|L|U|S\n");
    if (argc <= 1) {
       printf(TEXTDEFAULT); 
    }
}

void search_btree(root_t* root, int search_criteria, char search_string[],
int argc) {
    int i,j=0; /* Iterators */

    for(i=0; i<(*root).number_of_entries; i++) {
        telemetry_point_t* tel_p = get_telemetry_point(i,root);
        switch (search_criteria)
        {
        case 0:
            if(strcmp((*tel_p).plant,search_string)==0) {
                if(j==0) {print_search_header(argc);}
                print_telemetry_point(tel_p);
                j++;
            }
            break;
        case 1:
            if(strcmp((*tel_p).desig,search_string)==0) {
                if(j==0) {print_search_header(argc);}
                print_telemetry_point(tel_p);
                j++;
            }
            break;
        case 2:
            if(strcmp((*tel_p).moduletype,search_string)==0) {
                if(j==0) {print_search_header(argc);}
                print_telemetry_point(tel_p);
                j++;
            }
            break;
        }
    }
    if(j==0) {
        printf("No data found.\n");
    }
}
