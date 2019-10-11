/*******************************************************************************
* Substation Systems Database export functions
* Import functions
* Developers: Michael Lardner
* Student ID: 12893602
* Description: This c file contains all of the required functions for handling
* importing CSV files into the database.
*******************************************************************************/

#include "Substation.h"
#include "btree.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * This function handles the importing of csv files into the database
 * Developer: Michael Lardner
 * inputs:
 * - root_t* root_p - The root of the binary tree datastructure
 * - int argc - The amount of runtime arguments
 * - char* argv[] - String containing the runtime mode entered by the user
 * outputs:
 * - none
*******************************************************************************/
void import_csv(root_t* root_p, int argc, char* argv[]) {
    int selection;
    /* Checks if the user has already imported before */
    if (root_p->number_of_entries > 0) {
        do {
			import_menu(argc, argv);
			selection = import_menu_handler(root_p, argv);
            /* If the user selects to reset the database and start again
            a new binary tree datastructure is built */
            if (selection == 1) {
                root_t* root_p = create_root();
                /* Assigns root_p to itself to ignore the warning in the
                compiler of the variable being unused, even if it is */
                root_p = root_p;
            }
		}
		while (selection !=3 && selection !=2 && selection !=1);
    }
    if (selection != 3) {
        char filename[MAX_STRING_LEN];
        printf("Please enter the name of the csv file: ");
        fgets(filename, MAX_STRING_LEN, stdin);
        sscanf(filename, "%[^\n]", filename);
        /*fgets(filename, MAX_STRING_LEN, stdin);*/
        FILE * substation_database;
        substation_database = fopen(filename, "r");
        printf("%s\n", filename);
        if (substation_database) {
            /* The purpose of this first section of code is to check if the file
            is in the correct format. It does this by performing the steps to
            store the first line of the selected file. Then it checks if
            the first line features the correct column heading names. */
            char buffer[MAX_STRING_LEN*10];
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
            /*Always DNP in this case, comms protocol that talks to master
            station*/
            char protocol[MAX_STRING_LEN];
            /*channel number of the module */
            char number[MAX_STRING_LEN];
            /*the module address for master station communications*/
            char address[MAX_STRING_LEN];
            /*what type of signal, eg. analog, digital etc*/
            char moduletype[MAX_STRING_LEN];
            /*has the telemetry failed at the time of CSV save*/
            char failed[MAX_STRING_LEN];
            /*Is the telemetry online when CSV saved*/
            char online[MAX_STRING_LEN];
            /*same as failed*/
            char faulty[MAX_STRING_LEN];
            /* Out of service */
            char oos[MAX_STRING_LEN]; 
            fgets(buffer, sizeof(buffer),substation_database);
            sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],"
            "%[^,],%[^,],%[^,],%[^,],%[^,]",
            location, desig, plant, network, quantity, protocol, number,
            address, moduletype, failed, online, faulty, oos);
            /* This if statement checks if the first line of the file is in the
            correct format */
            if (strcmp(location, "Location") == 0 &&
            strcmp(desig, "Desig") == 0 &&
            strcmp(plant, "Plant") == 0 && strcmp(network, "Network") == 0 &&
            strcmp(quantity, "Quantity") == 0 &&
            strcmp(protocol, "Protocol") == 0 &&
            strcmp(number, "Number") == 0 && strcmp(address, "Address") == 0 &&
            strcmp(moduletype, "ModuleType") == 0 &&
            strcmp(failed, "Failed") == 0
            && strcmp(online, "Online") == 0 && strcmp(faulty, "Faulty") == 0
            && strcmp(oos, "Oos") == 0) {
                #ifdef DEBUG
                    printf("File opened\n");
                #endif

                int loopVar = 0;
                char* eof = filename;
                while (eof!=NULL) {
                    char buffer[MAX_STRING_LEN];
                    eof = fgets(buffer,MAX_STRING_LEN,substation_database);
                    loopVar++;
                }
                #ifdef DETAILEDDEBUG
                    printf("Loop var: %d\n", loopVar);
                #endif
                rewind(substation_database);

                /* Removes the header line of the csv because we don't want
                it now we've checked it's right. That mean's the file is
                shorter so loopVar is decrimented. */
                fgets(buffer,MAX_STRING_LEN,substation_database);
                loopVar--;

                while (loopVar > 0) {
                    char buffer[MAX_STRING_LEN*10];
                    /* Strings of all the variable types in a valid csv file
                    used to store the read data which is then used to
                    build the database */
                    char location[MAX_STRING_LEN]; 
                    char desig[MAX_STRING_LEN];
                    char plant[MAX_STRING_LEN];
                    char network[MAX_STRING_LEN];
                    char quantity[MAX_STRING_LEN];
                    char protocol[MAX_STRING_LEN];
                    char number[MAX_STRING_LEN];
                    char address[MAX_STRING_LEN];
                    char moduletype[MAX_STRING_LEN];
                    char failed[MAX_STRING_LEN];
                    char online[MAX_STRING_LEN];
                    char faulty[MAX_STRING_LEN];
                    char oos[MAX_STRING_LEN]; 
                    fgets(buffer, sizeof(buffer),substation_database);
                    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],"
                    "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
                    location, desig, plant, network, quantity, protocol, number,
                    address, moduletype, failed, online, faulty, oos);
                    telemetry_point_t* t_p_p = create_telemetry_point(location,
                    desig, plant, network, quantity, protocol, number, address,
                    moduletype, failed, online, faulty, oos);
                    add_telemetry_point(((*root_p).number_of_entries),
                    root_p, t_p_p);
                    loopVar--;
                }
            #ifdef DEBUG
                printf("Number of entries: %d\n", root_p->number_of_entries);
            #endif
            printf("Read Success\n");
                #ifdef DETAILEDDEBUG
                    int i;
                    printf("Reading memory");
                    for(i=0; i<((*root_p).number_of_entries);i++) {
                        telemetry_point_t* t_pr_p =
                        get_telemetry_point(i,root_p);
                        printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                         (*t_pr_p).location, (*t_pr_p).desig, (*t_pr_p).plant,
                            (*t_pr_p).network, (*t_pr_p).quantity,
                            (*t_pr_p).protocol, (*t_pr_p).number,
                            (*t_pr_p).address, (*t_pr_p).moduletype,
                            (*t_pr_p).failed, (*t_pr_p).online,
                            (*t_pr_p).faulty, (*t_pr_p).oos);
                
                    }
                #endif
            }
            else {
                printf("Invalid File. Please use a file a CSV file with the" 
                "following column headings: \n"
                "Location,Desig,Plant,Network,Quantity,Protocol,Number,Address,"
                "ModuleType,Failed,Online,Faulty,Oos,\n");
            }

        }

        else {
            printf("Read Error\n");
        }
    }
    

}

/*******************************************************************************
 * This function prints the import menu Which is only called when the user has
 * already used the import function in the session.
 * Developer: Michael Lardner
 * inputs:
 * - int argc - The amount of runtime arguments
 * - char* argv[] - String containing the runtime mode entered by the user
 * outputs:
 * - none
*******************************************************************************/
void import_menu(int argc, char* argv[]) {
    if (argc > 1) {
        /* Checks if the user is in the compression debug runtime mode */
        if (strcmp(argv[1], "-c") == 0) {
            printf("\n"
		    "\033[1;31m"
		    "IMPORT MENU\n"
    	    "\033[0m");
        }
        /* If the user is in the colour runtime mode */
        else {
            printf("\n"
		    "IMPORT MENU\n");
        }
	}
    /* If the user isn't in a runtime mode, the menu heading is printed
    normally */
    else if (argc <= 1) {
		printf("\n"
		"\033[1;31m"
		"IMPORT MENU\n"
    	"\033[0m");
	}
    printf(
        "A file has already been imported\n"
        "1. Replace the current loaded file\n"
        "2. Add a new files contents to the current database\n"
        "3. Exit to previous menu\n"
        "Enter choice (number between 1-3)>\n");
}

/*******************************************************************************
 * This function handles user input for the import menu. Which is only called
 * when the user has already used the import function in the session.
 * Developer: Michael Lardner
 * inputs:
 * - root_t* root_p - The root of the binary tree datastructure
 * - int argc - The amount of runtime arguments
 * - char* argv[] - String containing the runtime mode entered by the user
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
int import_menu_handler(root_t* root_p, char* argv[]) {
    char input[MAX_STRING_LEN];
	fgets(input, MAX_STRING_LEN, stdin);
	int selection;
	/* The atoi function turns a string input into a int. If unsuccessful
	it returns a 0. This is used for error detection. */
	selection = atoi(input);
	if (selection != 0) {
		switch(selection) {
			case 1 :
                delete_datastructure(root_p);
				break;
			case 2 :
				break;
			case 3 :
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
