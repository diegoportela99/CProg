/*******************************************************************************
* Substation Systems Database export functions
* Export functions
* Developer: Michael Lardner
* Student ID: 12893602
* Description: This c file contains all of the required functions for handling
* exporting the database file, as well as exporting the current database in
* memory as a csv file.
*******************************************************************************/

#include "Substation.h"
#include "btree.h"
#include <stdio.h>
#include <string.h>

void export_csv(root_t* root_p) {
    int loopVar;
    char filename[MAX_STRING_LEN];
    printf("Please enter a name for the csv file (Exclude .csv): ");
    fgets(filename, MAX_STRING_LEN, stdin);
    /* Removes the newline from the filename */
    sscanf(filename, "%[^\n]", filename);
    /* Makes the file a csv file */
    strcat(filename, ".csv");
    FILE * substation_database;
    substation_database = fopen(filename, "w");
    for (loopVar = 0; loopVar < (*root_p).number_of_entries-1; loopVar++) {
        /* Gets the data point at the specified index */
        telemetry_point_t* current = get_telemetry_point(loopVar, root_p);
        /* Prints the data point to the new file */
        fprintf(substation_database, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
        (*current).location, (*current).desig, (*current).plant, (*current).network, 
        (*current).quantity, (*current).protocol, (*current).number, (*current).address, 
        (*current).moduletype, (*current).failed, (*current).online, (*current).faulty, 
        (*current).oos);
        #ifdef DEBUG
        printf("Loopvar: %d ", loopVar);
        fprintf(textp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
        (*current).location, (*current).desig, (*current).plant, (*current).network, 
        (*current).quantity, (*current).protocol, (*current).number, (*current).address, 
        (*current).moduletype, (*current).failed, (*current).online, (*current).faulty, 
        (*current).oos);
        #endif
    }
    fclose(substation_database);
}

/*
void save_data(void) {

}
*/