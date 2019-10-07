/*******************************************************************************
 * THE PURPOSE OF THIS DOCUMENT IS TO ARCHIVE SPECIFIC BLOCKS OF CODE WHICH
 * NEEDED TO BE REMOVED FOR SOME REASON OR ANOTHER. ARCHIVED HERE IN CASE
 * IT IS REQUIRED AND FOR DOCUMENTATION PURPOSES. ARCHIVED CODE SHOULD BE
 * PLACED IN BLOCKS WITH COMMENTS OUTLINING WHY IT WAS REMOVED AND WHO BY.
*******************************************************************************/

/*******************************************************************************
 * Archived main function by Owen
 * REASON FOR REMOVAL: To refine the main function.
 * REMOVED BY: Michael Lardner
 * Developer: Owen Dowley
 * inputs:
 * - none
 * outputs:
 * - none
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
}; typedef struct telemetry_point telemetry_point_t;