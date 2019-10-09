/*******************************************************************************
* Substation Systems Database menu functions
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

/*******************************************************************************
 * Main
*******************************************************************************/
int main(int argc, char* argv[]) {
	if (argc > 1) {
		/* Checks if the user has entered the runtime command for
		yellow mode, and if so, changes the text colour to yellow */
		if (strcmp(argv[1], "-y") == 0) {
			printf("\033[01;33m");
			printf("YELLOW MODE ACTIVATED\n");
		}
		else {
			/* If a non valid runtime command is entered, the program
			is terminated */
			printf("Please enter a valid run mode\n");
			return (0);
		}
	}
	/* Creates the root for the binary tree data structure */
	root_t* root_p = create_root();
	int selection;
	printf("\n");
	/*
	printf("********************************************************************************\n");
	printf("SUBSTATION DATABASE\n");
	printf("********************************************************************************\n");
	*/
	/* The following printf's are used to create the ascii art heading for the
	program */
	printf("   _____ _    _ ____   _____ _______    _______ _____ ____  _   _ \n");
	printf("  / ____| |  | |  _ \\ / ____|__   __|/\\|__   __|_   _/ __ \\| \\ | |\n");
	printf(" | (___ | |  | | |_) | (___    | |  /  \\  | |    | || |  | |  \\| |\n");
	printf("  \\___ \\| |  | |  _ < \\___ \\   | | / /\\ \\ | |    | || |  | | . ` |\n");
	printf("  ____) | |__| | |_) |____) |  | |/ ____ \\| |   _| || |__| | |\\  |\n");
	printf(" |_____/ \\____/|____/|_____/   |_/_/    \\_\\_|  |_____\\____/|_| \\_|\n");
	printf("  _____       _______       ____           _____ ______ \n");
	printf(" |  __ \\   /\\|__   __|/\\   |  _ \\   /\\    / ____|  ____|\n");
	printf(" | |  | | /  \\  | |  /  \\  | |_) | /  \\  | (___ | |__   \n");
	printf(" | |  | |/ /\\ \\ | | / /\\ \\ |  _ < / /\\ \\  \\___ \\|  __|  \n");
	printf(" | |__| / ____ \\| |/ ____ \\| |_) / ____ \\ ____) | |____ \n");
	printf(" |_____/_/    \\_\\_/_/    \\_\\____/_/    \\_\\_____/|______|\n");
	
	/* Do while loop loops infinitely as the exit condition is the user entering
	a 4 which in the switch case calls the program to exit */
	do {
    	start_menu(argc);
		selection = start_menu_handler(root_p, argc);
	}
	while (selection != 4);
	return(0);
}