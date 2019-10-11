/*******************************************************************************
* Chunk and Dechunk functionality
* Developer: Owen Dowley
* Student ID: 13234505
* Description: This file contains functions prototypes for chunking data from 
* the datastructure into blocks of unsigned long long integers for use in 
* crypto.c, and for converting from blocks of ulls from crypto.c to the 
* datastructure. Functions prototyped here are defined in chunk.c
*******************************************************************************/
#include "block.h" /* for block_t*/
#include "btree.h" /* for root_t */

block_t* chunk (root_t* root_p);
void dechunk(root_t* root_p, block_t* block_p);