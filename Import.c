
#include "Substation.h"
#include "btree.h"
#include <stdio.h>
#include <string.h>

void import_csv(root_t* root_p) {
    char filename[MAX_STRING_LEN];
    printf("Please enter the name of the csv file: ");
    fgets(filename, MAX_STRING_LEN, stdin);
    /* Removes the newline from the filename */
    sscanf(filename, "%[^\n]", filename);
    FILE * substation_database;
    substation_database = fopen(filename, "r");
    printf("%s\n", filename);
    if (substation_database) {
        #ifdef DEBUG
            printf("File opened\n");
        #endif

        int loopVar = 0;
        char* eof = filename;
        /* Counts the amount of data points in the file */
        while (eof!=NULL) {
            char buffer[MAX_STRING_LEN];
            eof = fgets(buffer,MAX_STRING_LEN,substation_database);
            loopVar++;
        }
        #ifdef DETAILEDDEBUG
            printf("Loop var: %d\n", loopVar);
        #endif
        /* Returns to the start of the file */
        rewind(substation_database);

        while (loopVar > 0) {
            /* Recreates the telemetry_point struct to store the data */
            char buffer[MAX_STRING_LEN*10];
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
            /* Reads in the data point */
            sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
            location, desig, plant, network, quantity, protocol, number,
            address, moduletype, failed, online, faulty, oos);
            /* Creates a new data point with the collected data */
            telemetry_point_t* t_p_p = create_telemetry_point(location, desig, plant, network, quantity,
            protocol, number, address, moduletype, failed, online, faulty,
            oos);
            add_telemetry_point(((*root_p).number_of_entries), root_p, t_p_p);
            loopVar--;
        }
        printf("SUCCESS\n");
        #ifdef DETAILEDDEBUG
            int i;
            printf("Reading memory");
            for(i=0; i<((*root_p).number_of_entries);i++) {
                telemetry_point_t* t_pr_p = get_telemetry_point(i,root_p);
                printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", (*t_pr_p).location, (*t_pr_p).desig, (*t_pr_p).plant,
                    (*t_pr_p).network, (*t_pr_p).quantity, (*t_pr_p).protocol, (*t_pr_p).number, (*t_pr_p).address, (*t_pr_p).moduletype, (*t_pr_p).failed,
                    (*t_pr_p).online, (*t_pr_p).faulty, (*t_pr_p).oos);
        
            }
        #endif
    }

    else {
        printf("Read error\n");
    }

}

/*
void load_data(void) {


}
*/