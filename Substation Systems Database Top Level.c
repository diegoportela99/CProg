/*******************************************************************************
* Substation Systems Database 
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: This program will allow a user to import data from either a CSV
* or an already encrypted and compressed database file. Once imported they will
* be able to search for equipment within the electrical substation, convert a 
* CSV file to a compressed and encryped database file which can be stored and 
* shared in a secure manner.
*******************************************************************************/

/*******************************************************************************
 * Header Files
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************
 * Constants
*******************************************************************************/

/*******************************************************************************
 * Structs
*******************************************************************************/

struct telemetry_point {
	/*the location of the substation*/
	char[] location; 
	/*A unique code given to each piece of equipment*/
	char[] desig;
	/*the type of equipment, eg. Cb = circuit breaker*/
	char[] plant;
	/*eg. voltage level 11kV*/
	char[] network;
	/*The name of the actual point eg. oil temperature*/
	char[] quantity;
	/*Always DNP in this case, comms protocol that talks to master station*/
	char[] protocol;
	/*channel number of the module */
	int number;
	/*the module address for master station communications*/
	int address;
	/*what type of signal, eg. analog, digital etc*/
	char[] moduletype;
	/*has the telemetry failed at the time of CSV save*/
	int failed;
	/*Is the telemetry online when CSV saved*/
	int online;
	/*same as failed*/
	int faulty;
	/* Out of service */
	int oos; 
} typedef struct telemetry_point telemetry_point_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

/* NOTE: Functions are subject to change */
/* All of these functions use void as we still need to work out the design */
/* Would we have a function to print all the data entries? - Michael */

/*NOTE- The large gaps are left here to make it clear who is doing what for 
the checkpoint 1, it will be cleaned up later on*/

/* Prints the start menu */
/*Michael*/
void start_menu(void);


/* Prints the file menu */
/*Michael*/
void file_menu(void);


/* Imports the data from csv and writes to database */
/*Diego*/
void import_data(void);


/* Loads the data from csv */
/*Diego*/
void load_data(void);


/* Prints the search menu */
/*Michael*/
void search_menu(void);


/* Searches the csv by plant */
/*Jacob*/
void search_plant(void);


/* Searches the csv by desig */
/*Jacob*/
void search_desig(void);


/* Searches the csv by module */
/*Jacob*/
void search_module(void);


/* Exports the data as a csv */
/*Michael*/
void export_csv(void);


/* Exports the database file */
/*Michael*/
void save_data(void);

/* Encyrption of the file using a feistel algorithm */
/*Owen*/
void encryption(void);

/* Compression of the file using a huffman algorithm */
/*Ray*/
void compression(void);

/*******************************************************************************
 * Main
*******************************************************************************/
int exit = 0; /* Flags exit state */
int state[] = {0,0,0,0}; /* State of program 0) overall 
                                             1) in start menu 
                                             2) in file menu, and
                                             3) in search menu */

/* NOTE: This current outline ignores error handling. Any function called
         from the switch cases which could possibly fail should return a 1 or 
         0 for error handling.
         Please use 1 for success - it just makes for more intuitive if 
         phrasing. (Eg. if(!load_data()) {...} )
         
         Also, this current main-loop is very ugly. It is functionally how I
         think the program will probably run, but I think we should really
         break out all the menu switch cases (and their relative functions) 
         into separate files. 
         --Owen
         UPDATE*/

int main(void) { /* <- Coding standard here. UPDATE */
	while(!exit){
	    switch(state[0]) {
	        
	        /* Overall state 0: START MENU */
	        case 0: 
	            switch(state[1]) {
	                /* Print start menu, get user input */
	                case 0: state[1] = start_menu(); 
	                        break;
	                
	                /* Load data from encrypted file */
	                case 1: load_data(); 
	                        state[0] = 1; /* Go to file menu */
	                        break;
	        
	                /* Import data from CSV */
		            case 2: import_data(); 
		                    state[0] = 1; /* Go to file menu */
		                    break;
		    
		            /* Use existing file in memory */
		            case 3: state[0] = 1; 
                            break;
                    
                    /* Exit the program */
		            case 4: exit = 1; 
		                    break;
		    
		            /* Invalid state reached */
		            default: 
		        }
		        break;
		    
		    case 1: /* Overall state 1: FILE MENU */ 
		        switch(state[2]) {
		        
		            /* Print file menu, get user input */
		            case 0: state[2] = file_menu(); 
		                    break;
		                    
		            /* Substate: SEARCH MENU */
		            case 1:  
		                switch(state[3]) {
		                
		                    /* Print search menu, get user input*/
		                    case 0: state[3] = search_menu();
		                            break;
		                
		                    /* Search by plant */
		                    case 1: search_plants(); 
		                            break;
		                            
                            /* Search by designation */
                            case 2: search_desig(); 
	                                break;
		                    
                            /* Search by module */
                            case 3: search_module(); 
                                    break;
	                    }
	                    break;
		                    
		            case 2: export_csv(); /* Export to CSV */
		                    break;
		            
		            case 3: save_data(); /* Export to encrypted file */
		                    break;
		                    
		            case 4: state[0] = 0; /* Exits to start menu */
		            
		            default: /* Invalid state reached  */
		        }
		        break;
		        
		    default: /* Invalid state reached */
		}
	}
	return(0);
}

/*******************************************************************************
 * Functions
*******************************************************************************/

/*******************************************************************************
 * This functions prints the start menu
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void start_menu(void) {

}

/*******************************************************************************
 * This functions prints the file menu
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void file_menu(void) {

}

/*******************************************************************************
 * This functions imports the data from a csv file of the users selection 
 * Developer: Diego Portela
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void import_data(void) {

}

/*******************************************************************************
 * This functions loads a database file by user selection 
 * Developer: Diego Portela
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void load_data(void) {

}

/*******************************************************************************
 * This functions prints the search menu 
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_menu(void) {

}

/*******************************************************************************
 * This functions searches the database by the plant type inputted by the user 
 * Developer: Jacob Elali
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_plant(void) {

}

/*******************************************************************************
 * This functions searches the database by the desig type inputted by the user 
 * Developer: Jacob Elali
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_desig(void) {

}

/*******************************************************************************
 * This functions searches the database by the module type inputter by the user 
 * Developer: Jacob Elali
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void search_module(void) {

}

/*******************************************************************************
 * This functions exports to csv 
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void export_csv(void) {

}

/*******************************************************************************
 * This functions exports the database file 
 * Developer: Michael Lardner
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void save_data(void) {

}

/*******************************************************************************
 * This functions encrypts the data which can then be exported to a database  
 * Developer: Owen Dowley
 * inputs:
 * - TBA
 * outputs:
 * - TBA
*******************************************************************************/
void encryption(void) {

}

/*******************************************************************************
 * This functions compresses the data which can then be exported to a database  
 * Developer: Ray Robinson
 * inputs:
 * - TBA
 * outputs:
 * - TBA
*******************************************************************************/
void compression(void) {

}
