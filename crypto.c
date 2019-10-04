/*******************************************************************************
 * WOLFRAM'S RULE-30 BASED BLOCK CIPHER
 * Name: Owen Dowley
 * Student ID: 13234505
 * 
 * Description:
 * This section applies the block cipher defined in crypto.h for multiple blocks
 * of plaintext.It does this in "Electronic Code Book" mode, except changes the 
 * key after every block, reducing the issues generally associated with this.
 * 
 * Note: it is worst practice to use your own cryptographic algorithms. Always
 * use standard algorithms - I recommend TwoFish, which is freely available.
 * Standard well-known algoritms have shown they can withstand modern 
 * cryptanalysis techniques - this code hasn't. This code is probably 
 * cryptographically secure, but achieves this by merit of shuffling plaintext 
 * much more repeatedly than a better designed cipher would need to. This 
 * results in very slow performance.
 * Additionally, this cipher is probably vulnerable to sidechannel attacks.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "crypto.h"

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

/* Encrypts 64 bit blocks of plaintext, given an array of plaintext blocks,
   the number of blocks of plaintext, and a key. Writes ciphertext to file. */
int encrypt(unsigned long long plaintext[], int number_of_blocks);

/* Decrypts a file into 64 bit blocks of plaintext, given a key. */
int decrypt(unsigned long long* plaintext_p);

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void) {
    unsigned long long plaintext[4] = {0x2d1cd84c6567854c, 
                                       0x3853166d394825d4,
                                       0xc325242519821d33,
                                       0x2690fc4452ed668e};
    
    unsigned long long decrypted[4] = {0,0,0,0};

    encrypt(plaintext,4);
    decrypt(decrypted);
    print_title("FINAL PLAINTEXT");
    printf("%llx\n%llx\n%llx\n%llx\n",
        decrypted[0],decrypted[1],decrypted[2],decrypted[3]);
    
    return(0);
}

/*******************************************************************************
 * This function encrypts an array of blocks of plaintext, and saves the
 * resulting ciphertext to a file. This file has an integer header containing
 * the number of 64 bit blocks in the file.
 * inputs:
 * - plaintext[] | An array of plaintext to be encrypted.
 * - number_of_blocks | The number of blocks of plaintext, i.e., number of 
 *                    | elements in the plaintext array
 * - key[] | An array of key material used to encrypt the plaintext.
 * outputs:
 * - 1 or 0 | Has encrytion succeeded ? 1 : 0
*******************************************************************************/
int encrypt(unsigned long long plaintext[], int number_of_blocks) {
    
    #ifdef DEBUG
        print_title("ENCRYTPING");
    #endif

    int i; /* iterator */
    unsigned long long key[NUMBER_OF_KEY_BLOCKS];
    
    for(i=0;i<NUMBER_OF_KEY_BLOCKS;i++) {
        key[i] = csprng();
    }

    /* Save the key to file */
    file_handle_key(key,1);

    /* Initiate the key_cycle, storing the pointer to it */
    key_cycle_t* key_cycle_p = init_key_cycle(key);

    /* ciphertext_p is a pointer to memory to contain the ciphertext.
       MALLOC! Associated free() is later in encrypt */
    unsigned long long* ciphertext_p  = (unsigned long long*)malloc(sizeof
        (unsigned long long)*number_of_blocks);
    
    /* Check if malloc succeeded */
    if(ciphertext_p == NULL) {
        printf("Malloc failed for ciphertext_p\n");
        return(0);
    }

    /* Generate key cycles for each block of plaintext, and then encrypt them.*/
    for(i=0; i<number_of_blocks;i++) {
        key_cycle_p = key_schedule(key_cycle_p);
        #ifdef DETAILEDDEBUG
            print_header("Scheduling key:\n");
            printf("key_cycle_p = %p\n",key_cycle_p);
        #endif
        (*(ciphertext_p+i)) = feistel_network(plaintext[i],*key_cycle_p,1);
        
    }

    save_ciphertext(ciphertext_p, number_of_blocks);
    free(ciphertext_p);
    ciphertext_p = NULL;

    free_key_cycles(key_cycle_p);

    return(1);
}

/*******************************************************************************
 * Decrypt
*******************************************************************************/
int decrypt(unsigned long long* plaintext_p) {
    
    #ifdef DEBUG
        print_title("DECRYTPING");
    #endif

    unsigned long long key[NUMBER_OF_KEY_BLOCKS];

    file_handle_key(key,0);

    /* We need to hang onto the last generated key_cycle_p so we can hand it
    to free_key_cycles */
    key_cycle_t* last_key_cycle_p;

    int cipherBlocks,i;
    unsigned long long* ciphertext_p = NULL;
    if(plaintext_p == NULL) {
        printf("AAHH NULL POINTER DEBUG");
    }

    cipherBlocks = read_header();
    if(!cipherBlocks) {
        printf("Loading ciphertext failed\n");
        return(0);
    }

    ciphertext_p = load_ciphertext(cipherBlocks);
    
    key_cycle_t* key_cycle_p = init_key_cycle(key);
    
    for(i=0;i<cipherBlocks; i++) {
        key_cycle_p = key_schedule(key_cycle_p);
        #ifdef DETAILEDDEBUG
            print_header("Scheduling key:\n");
            printf("key_cycle_p = %p\n",key_cycle_p);
        #endif
    }

    last_key_cycle_p = key_cycle_p;

    for(i=cipherBlocks-1;i>=0;i--) {
        *(plaintext_p+i) = feistel_network(
            *(ciphertext_p+i),*key_cycle_p,0);  
        key_cycle_p = (*key_cycle_p).previous_key_cycle_p;
    }

    #ifdef DEBUG
        print_header("Returning plaintext:\n");
        for(i=0;i<cipherBlocks;i++) {
            printf("%llx\n",*(plaintext_p+i));
        }
    #endif

    free(ciphertext_p);
    ciphertext_p = NULL;

    free_key_cycles(last_key_cycle_p);
    
    return(1);
}