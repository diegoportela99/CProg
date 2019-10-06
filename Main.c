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
int main(int argc, char* argv[]) {
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
    	start_menu();
		selection = start_menu_handler();
	}
	while (selection != 4 && selection != 5);
	if (selection == 5) {

	}
	return(0);
}