#include "btree.h"
#include "btree.h"
#include "crypto.h"
#include "chunk.h"
#include "Substation.h"

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
    encrypt(plaintext_p,size);
    free(plaintext_p);
    delete_datastructure(root_p);
}

void load_from_db(root_t* root_p, int argc) {
    int i; /* Iterator */
    printf("\nReading database file header... \n");
    int size = read_header();
    printf("%d encrypted blocks found\n",size);
    unsigned long long* plaintextout_p = (unsigned long long*)malloc(sizeof(
        unsigned long long)*size);
    
    print_title("DECRYTPING DATA", argc);
    decrypt(plaintextout_p);

    block_t* blockout_p = create_block(NULL);

        for(i=0;i<size;i++) {
        blockout_p = add_ull_to_block(blockout_p,*(plaintextout_p+i));
        #ifdef DEBUG
            printf("%016llx\n", blockout_p->number);
        #endif
    }

    dechunk(root_p,blockout_p,size);

    #ifdef DEBUG
        printf("There are %d database entries.\n",root_p->number_of_entries);
        for(i=0; i<root_p->number_of_entries;i++) {
            
            telemetry_point_t* tp_p = get_telemetry_point(i,root_p);
            print_telemetry_point(tp_p);
        }
    #endif
}