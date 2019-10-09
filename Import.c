/*******************************************************************************
* Substation Systems Database export functions
* Export functions
* Developers: Michael Lardner
* Student ID: 12893602
* Description: This c file contains all of the required functions for handling
* exporting the database file, as well as exporting the current database in
* memory as a csv file.
*******************************************************************************/


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
        /*Always DNP in this case, comms protocol that talks to master
		station*/
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
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],"
		"%[^,],%[^,],%[^,],%[^,],%[^,]",
        location, desig, plant, network, quantity, protocol, number,
        address, moduletype, failed, online, faulty, oos);
        if (strcmp(location, "Location") == 0 && strcmp(desig, "Desig") == 0 &&
        strcmp(plant, "Plant") == 0 && strcmp(network, "Network") == 0 &&
        strcmp(quantity, "Quantity") == 0 && strcmp(protocol, "Protocol") == 0 &&
        strcmp(number, "Number") == 0 && strcmp(address, "Address") == 0 &&
        strcmp(moduletype, "ModuleType") == 0 && strcmp(failed, "Failed") == 0
        && strcmp(online, "Online") == 0 && strcmp(faulty, "Faulty") == 0
        && strcmp(oos, "Oos") == 0) {
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
                /*Always DNP in this case, comms protocol that talks to master
                station*/
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
                sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],"
                "%[^,],%[^,],%[^,],%[^,],%[^,]",
                location, desig, plant, network, quantity, protocol, number,
                address, moduletype, failed, online, faulty, oos);
                telemetry_point_t* t_p_p = create_telemetry_point(location, desig,
                plant, network, quantity, protocol, number, address, moduletype,
                failed, online, faulty, oos);
                add_telemetry_point(((*root_p).number_of_entries), root_p, t_p_p);
                loopVar--;
            }
        printf("Read Success\n");
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
            printf("Invalid File. Please use a file a CSV file with the" 
            "following column headings: \n"
            "Location,Desig,Plant,Network,Quantity,Protocol,Number,Address,"
            "ModuleType,Failed,Online,Faulty,Oos,\n");
        }

    }

    else {
        printf("Read Error\n");
    }

}

/*
void load_data(void) {


}
*/
