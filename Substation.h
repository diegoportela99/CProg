/*******************************************************************************
* Substation header file, this file holds the function definitions and structure
* prototype of the substation data. 
* Substation Systems Database 
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: ...
*******************************************************************************/

/*******************************************************************************
 * Header Files
 * 
 * I think we can remove these - Ray **UPDATE**
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*******************************************************************************
 * Constants
*******************************************************************************/

/*******************************************************************************
 * Structs
*******************************************************************************/

struct telemetry_point {
	char[] location; /*the location of the substation*/
	char[] desig;
	char[] plant;
	char[] network;
	char[] quantity;
	char[] protocol;
	int number;
	int address;
	char[] moduletype;
	int failed;
	int online;
	int faulty;
	int oos; /* Out of service */
} typedef struct telemetry_point telemetry_point_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

/* NOTE: Functions are subject to change */
/* All of these functions use void as we still need to work out the design */
/* Would we have a function to print all the data entries? - Michael */

/* Prints the start menu */
void start_menu(void);
/* Prints the file menu */
void file_menu(void);
/* Imports the data from csv and writes to database */
void import_data(void);
/* Loads the data from csv */
void load_data(void);
/* Prints the search menu */
void search_data(void);
/* Searches the csv by plant */
void search_plant(void);
/* Searches the csv by desig */
void search_desig(void);
/* Searches the csv by module */
void search_module(void);
/* Exports the data as a csv */
void export_csv(void);
/* Exports the database file */
void save_data(void);

