/*******************************************************************************
* Substation Systems Database Main
* Main
* Developer: Michael Lardner
* Student ID: 12893602
* Description: This c file contains the code for the substation systems
* database programs main function.
*******************************************************************************/

#include "Substation.h"
#include "btree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*******************************************************************************
 * Main
*******************************************************************************/
int main(int argc, char* argv[]) {
	printf("\033[0m");
	int selection;
	root_t* root_p = create_root();
	/* Checks if the user has inputted any runtime commands */
	if (argc > 1) {
		/* Checks if the user has entered the runtime command for
		yellow mode, and if so, changes the text colour to yellow */

		if (strcmp(argv[1], "-colour") == 0) {
			/*
			printf("\033[01;32m");
			printf("GREEN MODE ACTIVATED\n");
			*/
			do {
				colour_menu();
				selection = colour_menu_handler();
			}
			while (selection != 1 && selection != 2 && selection != 3 &&
			selection != 4);
		}
		if (strcmp(argv[1], "-c") == 0) {
			do {
				compression_debug_menu();
				selection = compression_debug_menu_handler(root_p, argc, argv);
			}
			while (selection != 3);
		}
		else if (strcmp(argv[1], "-colour") != 0) {
			/* If a non valid runtime command is entered, the program
			is terminated */
			printf("Please enter a valid run mode\n");
			return (0);
		}
	}
	printf("\n");
	/* The following printf's are used to create the ascii art heading for the
	program */
	printf("******************************************************************"
	"\n");
	printf("   _____ _    _ ____   _____ _______    _______ _____ ____  _   _ "
	"\n");
	printf("  / ____| |  | |  _ \\ / ____|__   __|/\\|__   __|_   _/ __ \\| \\"
	" | |\n");
	printf(" | (___ | |  | | |_) | (___    | |  /  \\  | |    | || |  | |  \\|"
	" |\n");
	printf("  \\___ \\| |  | |  _ < \\___ \\   | | / /\\ \\ | |    | || |  | |"
	" . ` |\n");
	printf("  ____) | |__| | |_) |____) |  | |/ ____ \\| |   _| || |__| | |\\ "
	" |\n");
	printf(" |_____/ \\____/|____/|_____/   |_/_/    \\_\\_|  |_____\\____/|_|"
	" \\_|\n");
	printf("  _____       _______       ____           _____ ______ \n");
	printf(" |  __ \\   /\\|__   __|/\\   |  _ \\   /\\    / ____|  ____|\n");
	printf(" | |  | | /  \\  | |  /  \\  | |_) | /  \\  | (___ | |__   \n");
	printf(" | |  | |/ /\\ \\ | | / /\\ \\ |  _ < / /\\ \\  \\___ \\|  __|  \n");
	printf(" | |__| / ____ \\| |/ ____ \\| |_) / ____ \\ ____) | |____ \n");
	printf(" |_____/_/    \\_\\_/_/    \\_\\____/_/    \\_\\_____/|______|\n");
	printf("\n****************************************************************"
	"**\n");
	
	/* Do while loop loops infinitely as the exit condition is the user entering
	a 4 which in the switch case calls the program to exit */
	do {
    	start_menu(argc);
		selection = start_menu_handler(root_p, argc, argv);
	}
	while (selection != 4 && selection != 5);
	if (selection == 5) {

	}
	/* Calls the binary tree destructor to ensure no trace is left of it once
	the program is terminated */
	delete_datastructure(root_p);
	/* Free the root itself too */
	free(root_p);
	return(0);
}
