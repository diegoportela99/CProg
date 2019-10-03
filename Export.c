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

void export_csv(btree_t* root_p) {
    int loopVar;
    int treesize = get_size();
    char filename[MAX_STRING_LEN];
    filename = strcat(filename, ".csv");
    printf("Please enter a name for the csv file: ");
    fgets(filename, MAX_STRING_LEN, stdin);
    FILE * textp;
    textp = fopen(filename, "w");
    for (loopVar = 0; loopVar <= treesize; loopVar++) {
        telemetry_point_t current = get_telemetry_point(loopVar, root_p);
        fprintf(textp, "%s,%s,%s,%s,%s,%s,%d,%d,%s,%d,%d,%d,%d,\n", 
        current.location, current.desig, current.plant, current.network, 
        current.quantity, current.protocol, current.number, current.address, 
        current.moduletype, current.failed, current.online, current.faulty, 
        current.oos);
    }
}

void save_data(void) {

}
