#include "block.h"
#include "debug.h"
#include <stdlib.h>

block_t* create_block(block_t* previous) {
    #ifdef DEBUG
        print_header("New Block\n");
    #endif
    block_t* block_p = (block_t*)malloc(sizeof(block_t*));
    if(previous == NULL) {
        block_p->number_of_blocks = 1;
    } else {
        block_p->number_of_blocks = previous->number_of_blocks + 1;
    }
    block_p->chars_added_to_block = 0;
    block_p->number = 0;
    block_p->previous = previous;
    #ifdef DEBUG
        printf("Chars: %d Number: %llx Previous: %p New: %p Number of blocks: "
        "%d\n",
            block_p->chars_added_to_block,block_p->number,
            block_p->previous = previous, block_p, block_p->number_of_blocks);
    #endif
    return(block_p);
}

block_t* add_char_to_block(block_t* block_p, char c) {
    /* If there isn't enough room for another char in this block,
    make us a new block. */
    if(block_p->chars_added_to_block==8) {
    #ifdef DEBUG
        printf("Block full, value: %llx\n",block_p->number);
    #endif
    block_p = create_block(block_p);
    }
    block_p->number = block_p->number<<8;
    block_p->number |= (int)c;
    #ifdef DEBUG
        printf("Block value: %llx\n",block_p->number);
    #endif
    block_p->chars_added_to_block++; 

    return(block_p);
}