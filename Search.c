/*******************************************************************************
 * Search Functionality
*******************************************************************************/
#include "btree.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void search(root_t* root, int search_criteria);

void print_telemetry_point(telemetry_point_t* telemetry_point);

void print_search_header();

void search_btree(root_t* root, int search_criteria, char search_string[]);

/*******************************************************************************
 * Function definitions
*******************************************************************************/
void search(root_t* root_p, int search_criteria) {
    char search_type[3][7] = {"plant","desig","module"};
    char user_input[BUFSIZ];
    char search_string[BUFSIZ];

    printf("Please enter %s: ", search_type[search_criteria-1]);
    fgets(user_input, BUFSIZ, stdin);
    sscanf(user_input,"%[^\n]",search_string);
    search_btree(root_p,search_criteria-1,search_string);
}

void print_telemetry_point(telemetry_point_t* telemetry_point) {
	printf (
    "%9s|%9s|%20s|%7s|%28s|%4s|%2s|%5s|%7s|%1s|%1s|%1s|%1s\n",
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

void print_search_header() {
    printf(TEXTRED(0));
    printf("                                                                  "
    "                                  F|O|F|O\n");
    printf("                                                                  "
        "                                  A|N|A|O\n");
    printf(" Location|    Desig|               Plant|Network|"
    "                    Quantity|Prot| #|Addrs| Module|I|L|U|S\n");
    printf(TEXTDEFAULT);
}

void search_btree(root_t* root, int search_criteria, char search_string[]) {
    int i,j=0; /* Iterators */

    for(i=0; i<(*root).number_of_entries; i++) {
        telemetry_point_t* tel_p = get_telemetry_point(i,root);
        switch (search_criteria)
        {
        case 0:
            if(strcmp((*tel_p).plant,search_string)==0) {
                if(j==0) {print_search_header();}
                print_telemetry_point(tel_p);
                j++;
            }
            break;
        case 1:
            if(strcmp((*tel_p).desig,search_string)==0) {
                if(j==0) {print_search_header();}
                print_telemetry_point(tel_p);
                j++;
            }
            break;
        case 2:
            if(strcmp((*tel_p).moduletype,search_string)==0) {
                if(j==0) {print_search_header();}
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
