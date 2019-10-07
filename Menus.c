/*******************************************************************************
* Substation Systems Database menu functions
* Menus
* Developer: Michael Lardner
* Student ID: 12893602
* Description: This c file contains all of the required functions for handling
* the substation systems database programs menus. Including printing and
* user navigation.
*******************************************************************************/

#include "Substation.h"
#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
 * This functions prints the start menu
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void start_menu(int argc) {
	/* Checks if a different colour mode is activated at runtime
	if not, the heading is printed in red */
	if (argc <= 1) {
		printf("\n"
		"\033[1;31m"
		"START MENU\n"
    	"\033[0m");
	}
	else {
		printf("\n"
		"START MENU\n");
	}
	printf(
    "1. Import CSV data\n"
    "2. Load database file\n"
    "3. File submenu\n"
    "4. Exit program\n"
    "Enter choice (number between 1-4)>\n");
}

/*******************************************************************************
 * This functions handles user input for the start menu
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
int start_menu_handler(root_t* root_p, int argc) {
	char input[MAX_STRING_LEN];
	fgets(input, MAX_STRING_LEN, stdin);
	int selection;
	/* The atoi function turns a string input into a int. If unsuccessful
	it returns a 0. This is used for error detection. */
	selection = atoi(input);
	if (selection != 0) {
		switch(selection) {
			case 1 :
				import_csv(root_p);
				break;
			case 2 :
			
				break;
			case 3 :
				do {
					file_menu(argc);
					selection = file_menu_handler(root_p, argc);
				}
				while (selection != 4);
				/* Changes selection to 0 so that when selection is returned
				it does not trigger the while clause in the main. Enabling
				the loop to continue. */
				selection = 0;
				break;	
			case 4 :
				/* Reverts the text colour to default and terminates
				the program */
				printf("\033[0m");
				exit(0);
				break;
			case 5 :
				break;
			default :
				printf("Invalid choice\n");
				break;
		}
	}
	else {
		printf("Invalid choice\n");
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
void file_menu(int argc) {
	/* Checks if a different colour mode is activated at runtime
	if not, the heading is printed in red */
	if (argc <= 1) {
		printf("\n"
		"\033[1;31m"
		"FILE MENU\n"
    	"\033[0m");
	}
	else {
		printf("\n"
		"FILE MENU\n");
	}
	printf(
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
int file_menu_handler(root_t* root_p, int argc) {
	char input[MAX_STRING_LEN];
	fgets(input, MAX_STRING_LEN, stdin);
	int selection;
	selection = atoi(input);
	if (selection != 0) {
		switch(selection) {
			case 1 :
				do {
					search_menu(argc);
					selection = search_menu_handler(argc);
				}
				while (selection != 4);
				selection = 0;
				break;
			case 2 :
				export_csv(root_p);
				break;
			case 3 :
				
				break;	
			case 4 :			
				break;
			default :
				printf("Invalid choice\n");
				break;
		}
	}
	else {
		printf("Invalid choice\n");
	}
	return selection;
}

/*******************************************************************************
 * This functions prints the search menu 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_menu(int argc) {
	/* Checks if a different colour mode is activated at runtime
	if not, the heading is printed in red */
	if (argc <= 1) {
		printf("\n"
		"\033[1;31m"
		"SEARCH MENU\n"
    	"\033[0m");
	}
	else {
		printf("\n"
		"SEARCH MENU\n");
	}
	printf(
    "1. Search by plant\n"
    "2. Search by desig\n"
    "3. Search by module\n"
    "4. Return to file menu\n"
    "Enter choice (number between 1-4)>\n");
}

/*******************************************************************************
 * This functions handles user input for the search menu
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
int search_menu_handler(int argc) {
	char input[MAX_STRING_LEN];
	fgets(input, MAX_STRING_LEN, stdin);
	int selection;
	selection = atoi(input);
	if (selection != 0) {
		switch(selection) {
			case 1 :
				
				break;
			case 2 :
				
				break;
			case 3 :
				
				break;	
			case 4 :			
				break;
			default :
				printf("Invalid choice\n");
				break;
		}
	}
	else {
		printf("Invalid choice\n");
	}
	return selection;
}