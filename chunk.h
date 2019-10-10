#include "block.h"
#include "btree.h"

block_t* chunk (root_t* root_p);
void dechunk(root_t* root_p, block_t* block_p, int size);