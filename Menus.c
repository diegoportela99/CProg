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
void start_menu(void) {
	printf("\n"
    "\033[1;31m"
	"START MENU\n"
    "\033[0m"
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
int start_menu_handler(void) {
	int selection;
	/* Using scanf right now for convinience, will change for consistency */
	scanf("%d", &selection); 
	switch(selection) {
		case 1 :
			
			break;
		case 2 :
			
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
	"\033[1;31m"
	"FILE MENU\n"
	"\033[0m"
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
			
			break;
		case 3 :
			
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
 * This functions prints the search menu 
 * Developer: 
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_menu(void) {
	printf("\n"
	"\033[1;31m"
	"SEARCH MENU\n"
	"\033[0m"
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
int search_menu_handler(void) {
	int selection;
	/* Using scanf right now for convinience, will change for consistency */
	scanf("%d", &selection); 
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
	return selection;

}