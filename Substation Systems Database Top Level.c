/*******************************************************************************
* Substation Systems Database 
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: ...
*******************************************************************************/

/*******************************************************************************
 * Header Files
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************
 * Constants
*******************************************************************************/

/* Temporary length for strings, so the code can compile */
#define MAX_STRING_LEN 256

/*******************************************************************************
 * Structs
*******************************************************************************/

struct telemetry_point {
	/*the location of the substation*/
	char location[MAX_STRING_LEN]; 
	/*A unique code given to each piece of equipment*/
	char desig[MAX_STRING_LEN];
	/*the type of equipment, eg. Cb = circuit breaker*/
	char plant[MAX_STRING_LEN];
	/*eg. voltage level 11kV*/
	char network[MAX_STRING_LEN];
	/*The name of the actual point eg. oil temperature*/
	char quantity[MAX_STRING_LEN];
	/*Always DNP in this case, comms protocol that talks to master station*/
	char protocol[MAX_STRING_LEN];
	/*channel number of the module */
	int number;
	/*the module address for master station communications*/
	int address;
	/*what type of signal, eg. analog, digital etc*/
	char moduletype[MAX_STRING_LEN];
	/*has the telemetry failed at the time of CSV save*/
	int failed;
	/*Is the telemetry online when CSV saved*/
	int online;
	/*same as failed*/
	int faulty;
	/* Out of service */
	int oos; 
}; typedef struct telemetry_point telemetry_point_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

/* NOTE: Functions are subject to change */
/* All of these functions use void as we still need to work out the design */
/* Would we have a function to print all the data entries? - Michael */

/* Prints the start menu */
/*Michael*/
void start_menu(void);
/* Handles user input for start_menu */
/*Michael*/
int start_menu_handler(void);
/* Prints the file menu */
/*Michael*/
void file_menu(void);
/* Handles user input for file_menu */
/*Michael*/
int file_menu_handler(void);
/* Imports the data from csv and writes to database */
/*Diego*/
void import_data(void);
/* Loads the data from csv */
/*Diego*/
void load_data(void);
/* Prints the search menu */
/*Michael*/
void search_menu(void);
/* Handles user input for search_menu */
/*Michael*/
int search_menu_handler(void);
/* Searches the csv by plant */
/*Jacob*/
void search_plant(void);
/* Searches the csv by desig */
/*Jacob*/
void search_desig(void);
/* Searches the csv by module */
/*Jacob*/
void search_module(void);
/* Exports the data as a csv */
/*Michael*/
void export_csv(void);
/* Exports the database file */
/*Michael*/
void save_data(void);

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void) { /* <- Coding standard here. UPDATE */
	int selection;
	printf("\n");
	printf("SUBSTATION DATABASE\n");
	/* Do while loop loops infinitely as the exit condition is the user entering
	a 4 which in the switch case calls the program to exit */
	/* This is just an initial format for the menu, it may need to be changed
	later if any issues are encountered */
	do {
		if (selection != 3)
    	start_menu();
		selection = start_menu_handler();
	}
	while (selection != 4);
	return(0);
}

/*******************************************************************************
 * Functions
*******************************************************************************/

/*******************************************************************************
 * This functions prints the start menu
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void start_menu(void) {
	printf("\n"
	"START MENU\n"
    "1. Import CSV data\n"
    "2. Load database file\n"
    "3. File submenu\n"
    "4. Exit program\n"
    "Enter choice (number between 1-4)>\n");
}

/*******************************************************************************
 * This functions handles user input for the start menu
 * Developer: Michael Lardner 12893602
 * inputs:
 * - none
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
int start_menu_handler(void) {
	int selection;
	/* Using scanf right now for convinience, will change for consistency */
	scanf("%d", &selection); 
	switch(selection) {
		case 1 :
			import_data();
			break;
		case 2 :
			load_data();
			break;
		case 3 :
			do {
				file_menu();
				selection = file_menu_handler();
			}
			while (selection != 4);
			/* Change selection to 0 so that when selection is returned it does
			not trigger the while clause in the main. Enabling the loop to 
			continue. */
			selection = 0;
			break;	
		case 4 :
			exit(0);
			break;
		default :
			printf("Invalid choice\n");
			break;
	}
	return selection;

}

/*******************************************************************************
 * This functions prints the file menu
 * Developer: Michael Lardner 12893602
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void file_menu(void) {
	printf("\n"
	"FILE MENU\n"
    "1. Search data\n"
    "2. Export CSV\n"
    "3. Export database file\n"
    "4. Exit to start menu\n"
    "Enter choice (number between 1-4)>\n");
}

/*******************************************************************************
 * This functions handles user input for the file menu
 * Developer: Michael Lardner 12893602
 * inputs:
 * - none
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
int file_menu_handler(void) {
	int selection;
	/* Using scanf right now for convinience, will change for consistency */
	scanf("%d", &selection); 
	switch(selection) {
		case 1 :
			do {
				search_menu();
				selection = search_menu_handler();
			}
			while (selection != 4);
			selection = 0;
			break;
		case 2 :
			export_csv();
			break;
		case 3 :
			save_data();
			break;	
		case 4 :			
			break;
		default :
			printf("Invalid choice\n");
			break;
	}
	return selection;
}

/*******************************************************************************
 * This functions imports the data from a csv file of the users selection 
 * Developer: Michael Lardner 12893602
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void import_data(void) {

}

/*******************************************************************************
 * This functions loads a database file by user selection 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void load_data(void) {

}

/*******************************************************************************
 * This functions prints the search menu 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_menu(void) {
	printf("\n"
	"SEARCH MENU\n"
    "1. Search by plant\n"
    "2. Search by desig\n"
    "3. Search by module\n"
    "4. Return to file menu\n"
    "Enter choice (number between 1-4)>\n");
}

/*******************************************************************************
 * This functions handles user input for the search menu
 * Developer: Michael Lardner 12893602
 * inputs:
 * - none
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
int search_menu_handler(void) {
	int selection;
	/* Using scanf right now for convinience, will change for consistency */
	scanf("%d", &selection); 
	switch(selection) {
		case 1 :
			search_plant();
			break;
		case 2 :
			search_desig();
			break;
		case 3 :
			search_module();
			break;	
		case 4 :			
			break;
		default :
			printf("Invalid choice\n");
			break;
	}
	return selection;

}

/*******************************************************************************
 * This functions searches the database by the plant type inputted by the user 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_plant(void) {

}

/*******************************************************************************
 * This functions searches the database by the desig type inputted by the user 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_desig(void) {

}

/*******************************************************************************
 * This functions searches the database by the module type inputter by the user 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_module(void) {

}

/*******************************************************************************
 * This functions exports to csv 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void export_csv(void) {

}

/*******************************************************************************
 * This functions exports the database file 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void save_data(void) {

}
