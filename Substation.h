/*******************************************************************************
* Substation header file, this file holds the function definitions and structure
* prototype of the substation data. 
* Substation Systems Database 
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: ...
*******************************************************************************/


/*******************************************************************************
 * Constants
*******************************************************************************/

/* Maximum length for strings */
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
void start_menu(void);
/* Handles user input for start_menu */
int start_menu_handler(void);
/* Prints the file menu */
void file_menu(void);
/* Handles user input for file_menu */
int file_menu_handler(void);
/* Imports the data from csv and writes to database */
void import_data(void);
/* Loads the data from csv */
void load_data(void);
/* Prints the search menu */
void search_menu(void);
/* Handles user input for search_menu */
int search_menu_handler(void);
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

