
#include "Substation.h"
#include "btree.h"
#include <stdio.h>
#include <string.h>

void import_csv(root_t* root_p) {
    char filename[MAX_STRING_LEN];
    printf("Please enter the name of the csv file: ");
    fgets(filename, MAX_STRING_LEN, stdin);
    sscanf(filename, "%[^\n]", filename);
    /*fgets(filename, MAX_STRING_LEN, stdin);*/
    FILE * substation_database;
    substation_database = fopen(filename, "r");
    printf("%s\n", filename);
    if (substation_database) {
        #ifdef DEBUG
            printf("File opened\n");
        #endif

        int loopVar = 0;
        char* eof = filename;
        while (eof!=NULL) {
            char buffer[MAX_STRING_LEN];
            eof = fgets(buffer,MAX_STRING_LEN,substation_database);
            loopVar++;
        }
        #ifdef DETAILEDDEBUG
            printf("Loop var: %d\n", loopVar);
        #endif
        rewind(substation_database);

        while (loopVar > 0) {
            char buffer[MAX_STRING_LEN*10];
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
            fgets(buffer, sizeof(buffer),substation_database);
            sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
            location, desig, plant, network, quantity, protocol, number,
            address, moduletype, failed, online, faulty, oos);
            telemetry_point_t* t_p_p = create_telemetry_point(location, desig, plant, network, quantity,
            protocol, number, address, moduletype, failed, online, faulty,
            oos);
            add_telemetry_point(((*root_p).number_of_entries), root_p, t_p_p);
            loopVar--;
        }
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