#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "btree.h"
#include "block.h"
#include "debug.h"

#define NUMBER_OF_ROWS 13

char* get_row_item(telemetry_point_t* tp_p, int row);

/*******************************************************************************
 * FUNCTION DEFINITIONS
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
        for(j=0;j<(NUMBER_OF_ROWS);j++) {
            temp = get_row_item(tp_p, j);
            #ifdef DEBUG
                printf("Data item: %s\n",temp);
            #endif
            /* Iterate through characters in an item */
            for(k=0;k<strlen(temp);k++) {
                block_p = add_char_to_block(block_p,*(temp+k));
            }
            block_p = add_char_to_block(block_p,',');
        } 
    }
return(block_p);
}

/*void dechunk(root_t* root_p, block_t* block_t) {
}*/

char* get_row_item(telemetry_point_t* tp_p, int row) {
    char* char_p;
    switch (row) {
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