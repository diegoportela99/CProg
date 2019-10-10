#include "block.h"
#include "debug.h"
#include <stdlib.h>

block_t* create_block(block_t* previous) {
    block_t* block_p = (block_t*)malloc(sizeof(block_t));
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

block_t* add_ull_to_block(block_t* block_p, unsigned long long number) {
    if(block_p->chars_added_to_block==8) {
        block_p = create_block(block_p);
    }
    block_p->number = number;
    block_p->chars_added_to_block+=8;
    return(block_p);
}

block_t* add_char_to_block(block_t* block_p, char c) {
    /* If there isn't enough room for another char in this block,
    make us a new block. */
    if(block_p->chars_added_to_block==8) {
    block_p = create_block(block_p);
    }
    block_p->number = block_p->number<<8;
    block_p->number |= (int)c;    
    block_p->chars_added_to_block++; 

    return(block_p);
}

block_t* get_char_from_block(block_t* block_p) {
    int c;
    if(block_p!=NULL) {
        if(block_p->chars_added_to_block==0) {

            block_t* newblock_p = block_p->previous;
            free(block_p);
            block_p = newblock_p;
        }
    }
    if(block_p!=NULL) {
        c = ((block_p->number)>>56);
        block_p->outputc = (char)c;
        block_p->number = block_p->number << 8;
        block_p->chars_added_to_block --;
    }
    return(block_p);
}