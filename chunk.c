/*******************************************************************************
* Chunk and Dechunk functionality
* Developer: Owen Dowley
* Student ID: 13234505
* Description: This file contains functions for chunking data from the 
* datastructure into blocks of unsigned long long integers for use in crypto.c,
* and for converting from blocks of ulls from crypto.c to the datastructure.
* Functions required elsewhere from this file are prototyped in chunk.h
*******************************************************************************/
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "block.h"
#include "debug.h"

#define NUMBER_OF_COLUMNS 13 /* The number of columns in the CSV */

/*******************************************************************************
 * FUNCTION PROTOTYPES
*******************************************************************************/

/* Gets the item in a given 'column' - that is, its initial place in the
CSV before importing to the datastructure. */
char* get_column_item(telemetry_point_t* tp_p, int column);

/*******************************************************************************
 * FUNCTION DEFINITIONS
*******************************************************************************/

/*******************************************************************************
 * This function chunks the current datastructure into unsigned long long int 
 * blocks.
 * inputs:
 * - root_p | A pointer to the root of the binary tree datastructure
 * outputs:
 * - block_t* | A pointer to the last block data was stored in
*******************************************************************************/
block_t* chunk (root_t* root_p) {
    int i,j,k; /* Iterators */
    char* temp;
    /* Create an instance of the automagical type block_t */
    block_t* block_p = create_block(NULL);

    /* Iterate through rows */
    for(i=0;i<(*root_p).number_of_entries;i++) {
        telemetry_point_t* tp_p = get_telemetry_point(i,root_p);
        /* Iterate through items in a row */
        for(j=1;j<(NUMBER_OF_COLUMNS+1);j++) {
            temp = get_column_item(tp_p, j);
            #ifdef DEBUG
                printf("Data item: %s\n",temp);
            #endif
            /* Iterate through characters in an item */
            for(k=0;k<strlen(temp);k++) {
                block_p = add_char_to_block(block_p,*(temp+k));
                #ifdef DETAILEDDEBUG
                    printf("%x ",*(temp+k));
                #endif

            }
            block_p = add_char_to_block(block_p,',');
            #ifdef DETAILEDDEBUG
                printf("%x ",',');
            #endif
        }
        #ifdef DETAILEDDEBUG
            printf("\n");
        #endif
    }
    /*while(block_p->number>>56 == 0 || block_p->number>>0x2c) {
        printf("%llx\n",block_p->number);
        block_p = get_char_from_block(block_p);
    }*/

return(block_p);
}

/*******************************************************************************
 * This function dechunks unsigned long long int blocks into the datastructure.
 * inputs:
 * - root_p | The root of the binary tree datastructure to store data in
 * - block_p | The last block data was stored in by decryption
 * - size | The number of blocks decrypted
 * outputs:
 * - none
*******************************************************************************/
void dechunk(root_t* root_p, block_t* block_p) {
    int i; /* Iterator */
    
    #ifdef DETAILEDDEBUG
        printf("DECHUNKING:\n");
        int j=0; 
    #endif
    while(block_p!=NULL) {
        #ifdef DETAILEDDEBUG
            j++;
            printf("Block %d\n",j);
        #endif
        char string[][255] = {"","","","","","","","","","","","",""};
        for (i=0;i<NUMBER_OF_COLUMNS;i++) {
            
            char c = 0;
            int cfound = 0;
            
            while(c!=','||!cfound) {
                block_p = get_char_from_block(block_p);
                if(block_p!= NULL) {
                    c = block_p->outputc;
                    if(c!=0&&c!=0x2c) {
                        strncat(string[i],&c,1);
                    }
                    if(c==0x2c) {
                        cfound = 1;
                    }
                
                }
                if(block_p==NULL) {
                    break;
                }
            }
   
            #ifdef DETAILEDDEBUG
                printf("%s\n",string[i]);
            #endif

        }
        if(block_p!=NULL) {
            telemetry_point_t* tel_p = create_telemetry_point(string[0],string[1],
                string[2],string[3],string[4],string[5],string[6],string[7],
                string[8],string[9],string[10],string[11],string[12]);
            add_telemetry_point(root_p->number_of_entries,root_p,tel_p);
        }
    }
}


/*******************************************************************************
 * This gets the item in a given "column" of a telemetry point - that is,
 * the column from the initial csv it would sit in.
 * inputs:
 * - tp_p | A pointer to the telemetry point
 * - column | The number of the column
 * outputs:
 * - char* | The appropriate member of tp_p
*******************************************************************************/
char* get_column_item(telemetry_point_t* tp_p, int column) {
    char* char_p;
    switch (column) {
        case 1:
            char_p = (*tp_p).location;
            break;
        case 2:
            char_p= (*tp_p).desig;
            break;
        case 3:
            char_p= (*tp_p).plant;
            break;
        case 4:
            char_p= (*tp_p).network;
            break;
        case 5:
            char_p= (*tp_p).quantity;
            break;
        case 6:
            char_p= (*tp_p).protocol;
            break;
        case 7:
            char_p= (*tp_p).number;
            break;
        case 8:
            char_p= (*tp_p).address;
            break;
        case 9:
            char_p= (*tp_p).moduletype;
            break;
        case 10:
            char_p= (*tp_p).failed;
            break;
        case 11:
            char_p= (*tp_p).online;
            break;
        case 12:
            char_p= (*tp_p).faulty;
            break;
        case 13:
            char_p= (*tp_p).oos;
            break;
    }
    return(char_p);
}