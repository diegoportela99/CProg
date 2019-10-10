#ifndef BLOCKHEADER
#define BLOCKHEADER

#include "debug.h"
#include "btree.h"

struct block {
    int number_of_blocks;
    unsigned int chars_added_to_block;
    unsigned long long number;
    char outputc;
    struct block* previous;
}; typedef struct block block_t;

block_t* create_block(block_t* previous);

block_t* add_char_to_block(block_t* block_p, char c);

block_t* add_ull_to_block(block_t* block_p, unsigned long long number);

block_t* get_char_from_block(block_t* block_p);

#endif