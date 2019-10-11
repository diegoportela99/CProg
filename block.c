/*******************************************************************************
 * BLOCK.C
 * BLOCK HANDLER FUNCTIONS 
 * Name: Owen Dowley
 * Student ID: 13234505
 * 
 * Description:
 * This file contains the functions used to interact with the data block defined
 * in block.h
*******************************************************************************/
#include "block.h"
#include "debug.h"
#include <stdlib.h>

/******************************************************************************* 
 * This function creates a block, given a pointer to the previous block. It
 * initializes all the values of the new block.
 * inputs:
 * - previous | The pointer to the previous block_t
 * outputs:
 * - block_t* | The pointer to the new block
*******************************************************************************/
block_t* create_block(block_t* previous) {
    
    /* Allocate memory for a new block */
    block_t* block_p = (block_t*)malloc(sizeof(block_t));
    if(!block_p) {
        printf("Malloc failed\n");
    }

    /* Initialize values in the block */
    if(previous == NULL) {
        block_p->number_of_blocks = 1;
    } else {
        block_p->number_of_blocks = previous->number_of_blocks + 1;
    }
    block_p->chars_added_to_block = 0;
    block_p->number = 0;
    block_p->outputc = 0;
    block_p->previous = previous;

    return(block_p);
}

/******************************************************************************* 
 * This function adds an unsigned long long int to a block. It automagically 
 * handles making new blocks as required.
 * inputs:
 * - block_p | The block being written
 * - number | The unsigned long long int to be stored
 * outputs:
 * - block_t* | The pointer to the new block
*******************************************************************************/
block_t* add_ull_to_block(block_t* block_p, unsigned long long number) {
    /* If the block is currently full, make a new one */
    if(block_p->chars_added_to_block==8) {
        block_p = create_block(block_p);
    }
    /* Add the ull to the block */
    block_p->number = number;
    block_p->chars_added_to_block+=8;
    return(block_p);
}

/******************************************************************************* 
 * This function adds chars to a block. It automagically handles making new
 * blocks as required.
 * inputs:
 * - block_p | The block being written
 * - c | The char to be stored
 * outputs:
 * - block_t* | The pointer to the new block (or previous block)
*******************************************************************************/
block_t* add_char_to_block(block_t* block_p, char c) {
    /* If there isn't enough room for another char in this block,
    make us a new block. */
    if(block_p->chars_added_to_block==8) {
    block_p = create_block(block_p);
    }
    /* Add the char to the block (as a byte in the ull number) */
    block_p->number = block_p->number<<8;
    block_p->number |= (int)c;    
    block_p->chars_added_to_block++; 
    return(block_p);
}

/******************************************************************************* 
 * This function gets a char from a block. It automagically handles retrieving
 * older blocks and deallocating blocks as required.
 * inputs:
 * - block_p | The block being read
 * outputs:
 * - block_t* | The pointer to the new block (or previous block)
*******************************************************************************/
block_t* get_char_from_block(block_t* block_p) {
    int c;
    /* I'm very careful not to dereference null pointers in this function */
    if(block_p!=NULL) {
        /* If a block is empty get the previous block */
        if(block_p->chars_added_to_block==0) {

            block_t* newblock_p = block_p->previous;
            free(block_p);
            block_p = newblock_p;
        }
    }
    if(block_p!=NULL) {
        /* Get a character from the block */
        c = ((block_p->number)>>56);
        block_p->outputc = (char)c;
        block_p->number = block_p->number << 8;
        block_p->chars_added_to_block --;
    }
    return(block_p);
}