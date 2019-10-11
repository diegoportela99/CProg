/*******************************************************************************
* Substation header file, this file holds the function definitions and structure
* prototype of the substation data. 
* Substation Systems Database 
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: Contains the function prototypes for all the primary functions
* used in the program. Also contains widely used constants.
*******************************************************************************/

#ifndef SUBSTATIONHEADER
#define SUBSTATIONHEADER

#include "btree.h"

/*******************************************************************************
 * Constants
*******************************************************************************/

/* Maximum length for strings */
#define MAX_STRING_LEN 256

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

/* Prints the start menu */
void start_menu(int argc);
/* Handles user input for start_menu */
int start_menu_handler(root_t* root_p, int argc, char* argv[]);
/* Prints the file menu */
void file_menu(int argc);
/* Handles user input for file_menu */
int file_menu_handler(root_t* root_p, int argc);
/* Prints the compression debug menu */
void compression_debug_menu(void);
/* Handles user input for the compression debug menu */
int compression_debug_menu_handler(root_t* root_p, int argc, char* argv[]);
/* Prints the colour menu */
void colour_menu(void);
/* Handles user input for the colour menu */
int colour_menu_handler(void);
/* Prints the import menu */
void import_menu(int argc, char* argv[]);
/* Handles user input for the import menu */
int import_menu_handler(root_t* root_p, char* argv[]);
/* Imports the data from csv and writes to database */
void import_csv(root_t* root_p, int argc, char* argv[]);
/* Loads the data from csv */
void load_from_db(root_t* root_p, int argc);
/* Prints the search menu */
void search_menu(int argc);
/* Handles user input for search_menu */
int search_menu_handler(root_t* root_p, int argc);
/* Searches the csv by plant */
void search(root_t* root_p, int search_criteria, int argc);
/* Exports the data as a csv */
void export_csv(root_t* root_p);
/* Exports the database file */
void save_to_db(root_t* root_p, int argc);
/* Prints a telemetry point with formatting. Used for debug in loadsave.c,
as well as in Search.c */
void print_telemetry_point(telemetry_point_t* telemetry_point);
#endif

