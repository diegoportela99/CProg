/*******************************************************************************
* Substation Systems Database menu functions
* Main
* Developer: Michael Lardner
* Student ID: 12893602
* Description: This c file contains the code for the substation systems
* database programs main function.
*******************************************************************************/

#include "Substation.h"
#include <stdio.h>

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void) {
	int selection;
	printf("\n");
	printf("********************************************************************************\n");
	printf("SUBSTATION DATABASE\n");
	printf("********************************************************************************\n");
	/* Do while loop loops infinitely as the exit condition is the user entering
	a 4 which in the switch case calls the program to exit */
	do {
    	start_menu();
		selection = start_menu_handler();
	}
	while (selection != 4);
	return(0);
}