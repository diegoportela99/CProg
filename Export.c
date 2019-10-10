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

/*******************************************************************************
 * This functions exports the current database as a csv file
 * Developer: Michael Lardner
 * inputs:
 * - root_t* root_p - The root of the binary tree datastructure
 * outputs:
 * - int selection - Integer of the users selection
*******************************************************************************/
void export_csv(root_t* root_p) {
    if (root_p->number_of_entries > 0) {
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
        fprintf(substation_database, "Location,Desig,Plant,Network,Quantity,"
            "Protocol,Number,Address,ModuleType,Failed,Online,Faulty,Oos,\n");

        /* Loops over all the entries in the database */
        for (loopVar = 0; loopVar < (*root_p).number_of_entries-1; loopVar++) {
            /* Gets the data point at the specified index */
            telemetry_point_t* current = get_telemetry_point(loopVar, root_p);
            /* Prints the data point to the new file */
            fprintf(substation_database, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"
            ",%s,\n", 
            (*current).location, (*current).desig, (*current).plant,
            (*current).network, (*current).quantity, (*current).protocol,
            (*current).number, (*current).address, (*current).moduletype,
            (*current).failed, (*current).online, (*current).faulty, 
            (*current).oos);
            #ifdef DEBUG
            printf("Loopvar: %d ", loopVar);
            fprintf(textp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
            (*current).location, (*current).desig, (*current).plant,
            (*current).network, (*current).quantity, (*current).protocol,
            (*current).number, (*current).address, (*current).moduletype, 
            (*current).failed, (*current).online, (*current).faulty, 
            (*current).oos);
            #endif
        }
        printf("Export Success\n");
        fclose(substation_database);
    }
    else {
        printf("No data in memory. Please use the import function first.\n");
    }
}

