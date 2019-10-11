/*******************************************************************************
 * BLOCK.H
 * BLOCK DATATYPE 
 * Name: Owen Dowley
 * Student ID: 13234505
 * 
 * Description:
 * This file defines the datatype block. It is used to transfer from the 
 * btree datastructure to unsigned long long ints which can be handled by
 * encryption and decryption.
*******************************************************************************/
#ifndef BLOCKHEADER
#define BLOCKHEADER

#include "debug.h" /* for DEBUG */

struct block {
    /* The number of data blocks currently stored */
    int number_of_blocks;
    /* The number of characters in the current block */
    unsigned int chars_added_to_block;
    /* The unsigned long long int storing data */
    unsigned long long number;
    /* The character outputed by the block */
    char outputc;
    /* A pointer to the previous block */
    struct block* previous;
}; typedef struct block block_t;

/* Creates a block_t */
block_t* create_block(block_t* previous);

/* Adds a character to the current block. Sneakily handles obtaining new blocks
when required */
block_t* add_char_to_block(block_t* block_p, char c);

/* Adds an unsigned long long to the current block, and fetches a longer one */
block_t* add_ull_to_block(block_t* block_p, unsigned long long number);

/* Gets a character for a new block, and handles getting more blocks where
needed */
block_t* get_char_from_block(block_t* block_p);

#endif