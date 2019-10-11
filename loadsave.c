/*******************************************************************************
* Encrypted database load and save functions
* Developer: Owen Dowley
* Student ID: 13234505
* Description: This file contains functions for loading from and saving to the
* encrypted database.
* Functions required in top level from this file are prototyped in Substation.h
*******************************************************************************/
#include "debug.h" /* Used for print_title, DEBUG mode */
#include "btree.h" /* Used for root_t */
#include "crypto.h"/* Used for encrypt, decrypt */
#include "chunk.h" /* Used for chunk, dechunk */
#include "Substation.h" /* Used for print_telemetry_point */
#include <stdlib.h> /* Used for malloc */

/*******************************************************************************
 * Function Definitions
*******************************************************************************/

/*******************************************************************************
 * This function saves the current datastructure as an encrypted database.
 * inputs:
 * - root_p | The root of the binary tree datastructure
 * - argc | The number of arguments the program was run with
 * outputs:
 * - none
*******************************************************************************/
void save_to_db(root_t* root_p, int argc) {
    if(root_p->number_of_entries<=0) {
        printf("Load data before trying to export.\n");
        return;
    }
    int i; /* Iterator */

    #ifdef DEBUG
        printf("There are %d database entries.\n",root_p->number_of_entries);
        for(i=0; i<root_p->number_of_entries-1;i++) {
            
            telemetry_point_t* tp_p = get_telemetry_point(i,root_p);
            print_telemetry_point(tp_p);
        }
    #endif

    block_t* block_p = chunk(root_p);
    int size = block_p->number_of_blocks;
    unsigned long long* plaintext_p = (unsigned long long*)malloc(sizeof
        (unsigned long long )* size);

    for(i=0;i<size;i++) {
        plaintext_p[i] = block_p->number;
        #ifdef DEBUG
            printf("%016llx\n", plaintext_p[i]);
        #endif
        block_p = block_p->previous;
    }
    print_title("ENCRYTPING  DATA",argc);
    if(!encrypt(plaintext_p,size)) {
        printf("Encrypt failed.\n");
        return;
    }
    free(plaintext_p);
    delete_datastructure(root_p);
}

/*******************************************************************************
 * This function loads an encrypted database into the datastructure.
 * inputs:
 * - root_p | The root of the binary tree datastructure
 * - argc | The number of arguments the program was run with
 * outputs:
 * - none
*******************************************************************************/
void load_from_db(root_t* root_p, int argc) {
    int i; /* Iterator */
    
    int size = read_header();
    if(!size) {
        printf("\nNo database file found.\nSave an encrypted database "
            "file first, via file menu option \"Export database file.\"\n");
        return;
    }
    printf("\nReading database file header... \n");
    printf("%d encrypted blocks found\n",size);
    unsigned long long* plaintextout_p = (unsigned long long*)malloc(sizeof(
        unsigned long long)*size);
    
    print_title("DECRYTPING DATA", argc);
    if(!decrypt(plaintextout_p)) {
        printf("Decrypt failed.\n");
        return;
    }

    block_t* blockout_p = create_block(NULL);

        for(i=0;i<size;i++) {
        blockout_p = add_ull_to_block(blockout_p,*(plaintextout_p+i));
        #ifdef DEBUG
            printf("%016llx\n", blockout_p->number);
        #endif
    }

    dechunk(root_p,blockout_p);

    #ifdef DEBUG
        printf("There are %d database entries.\n",root_p->number_of_entries);
        for(i=0; i<root_p->number_of_entries;i++) {
            
            telemetry_point_t* tp_p = get_telemetry_point(i,root_p);
            print_telemetry_point(tp_p);
        }
    #endif
}