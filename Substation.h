/*******************************************************************************
* Substation header file, this file holds the function definitions and structure
* prototype of the substation data. 
* Substation Systems Database 
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: ...
*******************************************************************************/
#include "btree.h"
#include "block.h"

/*******************************************************************************
 * Constants
*******************************************************************************/

/* Maximum length for strings */
#define MAX_STRING_LEN 256

/*******************************************************************************
 * Structs
*******************************************************************************/



/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

/* NOTE: Functions are subject to change */
/* All of these functions use void as we still need to work out the design */
/* Would we have a function to print all the data entries? - Michael */

/* Prints the start menu */
void start_menu(void);
/* Handles user input for start_menu */
int start_menu_handler(root_t* root_p);
/* Prints the file menu */
void file_menu(void);
/* Handles user input for file_menu */
int file_menu_handler(root_t* root_p);
/* Imports the data from csv and writes to database */
void import_csv(root_t* root_p);
/* Loads the data from csv */
void load_data(void);
/* Prints the search menu */
void search_menu(void);
/* Handles user input for search_menu */
int search_menu_handler(root_t* root_p);
/* Searches the csv */
void search(root_t* root, int search_criteria);
/* Exports the data as a csv */
void export_csv(root_t* root_p);
/* Exports the database file */
void save_data(void);
/* Breaks data from structure into 64 bit chunks for encryption */
block_t* chunk (root_t* root_p);
/* Encrypts 64 bit blocks of plaintext, given an array of plaintext blocks,
   the number of blocks of plaintext, and a key. Writes ciphertext to file. */
int encrypt(block_t* block_p);

