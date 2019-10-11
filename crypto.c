/*******************************************************************************
 * CRYPTO.C
 * WOLFRAM'S RULE-30 BASED BLOCK CIPHER
 * Name: Owen Dowley
 * Student ID: 13234505
 * 
 * Description:
 * This is an AES equivalent cipher, which uses 256 bit keys and encrypts 64
 * bit plaintext blocks. It operates on the Feistel network structure, 
 * repeatedly using a highly non-linear F function to fufill Shannon's principle
 * of confusion, and repeated XORing input with key material (known as "key 
 * whitening") to fufill Shannon's principle of diffusion.
 * 
 * The F-function in this cipher impliments a round of Wolfram's Rule 30 - a 
 * cellular automaton which has seen usage in pseudo-random number generation
 * because of the emergant semi-random pattern which occurs.
 * 
 * Note: it is worst practice to use your own cryptographic algorithms. Always
 * use standard algorithms - I recommend TwoFish, which is freely available.
 * Standard well-known algoritms have shown they can withstand modern 
 * cryptanalysis techniques - this code hasn't, and probably wouldn't.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "crypto.h"
#include "debug.h"

/* Database name */
#define DBNAME "database.ssdb"

/* Key file name*/
#define KEYNAME "key"

/* Source random numbers */
#define URANDOM "/dev/urandom"

/* Defines a modulus which behaves correctly for negative values (unlike  "%").
Mathematically, negative inputs to mod should wrap round to positive. 
NOTE: Does not work for x < -y. */
#define MOD(x,y) ((((x)%(y))+(y))%(y)) 

/* Selects the rightmost 4 bytes of an unsigned long long int */
#define EIGHTLOWMASK 0xFFFFFFFF

/* The number of blocks of key material. */
#define NUMBER_OF_KEY_BLOCKS 4

/* The number of sections of key used for key whitening */
#define NUMBER_OF_WKEYS 4

/* The number of sections of key used in the F function */
#define NUMBER_OF_KEYS 32

/* The number of cycles of Wolfram's Rule 30 applied in the F function. Could
be as low as 1, but higher numbers cause greater non-linearity, which is a 
cryptographically desirable property. */
#define WOLFRAMCYCLES 5

/*******************************************************************************
 * Structure Definitions
*******************************************************************************/

/* This structure holds a 'cycle' of keys - all the keys needed to encrypt a
single block of ciphertext. In addition, it holds a pointer to the previous key 
cycle - needed in decryption to iterate backwards through key cycles, but also 
needed for the deallocation of memory. */
struct key_cycle {
    /* The sections of key used for key whitening */
    unsigned long long wkey[NUMBER_OF_WKEYS]; 

    /* The sections of key used in the F function */
    unsigned long long key[NUMBER_OF_KEYS]; 
    
    /* Pointer to previous key cycle. Needed to iterate backwards through key
    cycles for decryption */
    struct key_cycle* previous_key_cycle_p; 
};
typedef struct key_cycle key_cycle_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/
/* Initializes the first key cycle */
key_cycle_t* init_key_cycle(unsigned long long key[NUMBER_OF_KEY_BLOCKS]);

/* Schedules a new key cycle given the previous one */
key_cycle_t* key_schedule(key_cycle_t* previous_key_cycle_p);

/* Frees a key cycle and all previous key cycles */
void free_key_cycles(key_cycle_t* key_cycle_p);

/* Saves the encrypted text to a file, with an integer header which is the
number of blocks of ciphertext saved. */
int save_ciphertext(unsigned long long* ciphertext_p, int number_of_blocks);

/* Loads an encrypted file into memory as ciphertext */
unsigned long long* load_ciphertext(int number_of_blocks);

/* The core encryption scheme is a feistel network. It takes a block and key 
material, and either encryts or decrypts the block */
unsigned long long feistel_network(unsigned long long plaintext,key_cycle_t key,
    int encrypt);

/* Also known as a "round function", the heart of a feistel cipher. This 
specific F function takes a block of plaintext, XORs in the key material, and
then performs pseudo-random number generation with it as the seed.
Is strongly non-linear. */
unsigned long f_function(unsigned long plaintext, unsigned long key);

/* Performs the pseudo-random number generation used in the F-function, using
the cellular automata defined by Wolfram's Rule 30 */
unsigned long wolframs_rule_30(unsigned long plaintext);

/* Cryptographically Secure Pseudo-Random Number Generator. Generates a key 
from system randomness provided in /dev/urandom. Is Linux system dependent. */
unsigned long long csprng();

/* Saves the key to a file, or loads it from one. */
int file_handle_key(unsigned long long key[NUMBER_OF_KEY_BLOCKS],int save);

#ifdef GRAPHICALDEBUG
    /* Graphs Wolfram's rule 30 for debug purposes. If functioning correctly,
    triangular patterns should be visable. */
    void graph_rule_30(unsigned long row);
#endif

/******************************************************************************* 
 * This function creates a new key cycle struct from 256 bits of key material.
 * This should only be used to create the very first key cycle, from then on use
 * key_schedule().
 * inputs:
 * - key[] | The array of key material used to initialize the key cycle
 * outputs:
 * - key_cycle_t* | A pointer to the initialized key_cycle_p. 
 * /////////////////////////// MEMORY WARNING //////////////////////////////////
 * This function calls malloc, and returns a pointer to the allocated memory! 
 * Functions which use init_key_cycle() are responsible for calling the
 * destructor free_key_cycles() on the LAST key cycle created.
*******************************************************************************/
key_cycle_t* init_key_cycle(unsigned long long key[]) {
        int i;
        key_cycle_t* key_cycle_p = (key_cycle_t*)malloc(sizeof(key_cycle_t));
        if(key_cycle_p == NULL) {printf("AAHH NULL POINTER DEBUG");}
        

        #ifdef DEBUG
            printf(TEXTRED(0));
            printf("Initializing key cycle\n");
            printf(TEXTDEFAULT);
        #endif

        (*key_cycle_p).previous_key_cycle_p = NULL;

        unsigned long temp[4] = {f_function(key[0]>>32,key[1]&EIGHTLOWMASK),
                                 f_function(key[1]>>32,key[2]&EIGHTLOWMASK),
                                 f_function(key[2]>>32,key[3]&EIGHTLOWMASK),
                                 f_function(key[3]>>32,key[0]&EIGHTLOWMASK)};
        
        for(i = 0; i<4; i++) {
            temp[0] = f_function(temp[2],temp[1]);
            temp[1] = f_function(temp[3],temp[2]);
            temp[2] = f_function(temp[0],temp[3]);
            temp[3] = f_function(temp[1],temp[0]);
            (*key_cycle_p).wkey[i] = f_function(temp[0],temp[1]);
            
            #ifdef DETAILEDDEBUG
                printf("WKey %d: %llx ", i, (*key_cycle_p).wkey[i]);
            #endif
        }
        
        for(i = 0; i<32; i++) {
            temp[0] = f_function(temp[2],temp[1]);
            temp[1] = f_function(temp[3],temp[2]);
            temp[2] = f_function(temp[0],temp[3]);
            temp[3] = f_function(temp[1],temp[0]);
            (*key_cycle_p).key[i] = f_function(temp[0],temp[1]);
            #ifdef DETAILEDDEBUG
                printf("Key %d: %llx ", i, (*key_cycle_p).key[i]);
            #endif
        }
        #ifdef DETAILEDDEBUG
            printf("\n");
        #endif
    return key_cycle_p;
}

/******************************************************************************* 
 * This function creates a new key cycle struct from an earlier key cycle. The 
 * new key cycle contains a pointer to the old one.
 * inputs:
 * - previous_key_cycle_p | A pointer to the previous key cycle
 * outputs:
 * - key_cycle_t* | A pointer to the new key cycle 
 * /////////////////////////// MEMORY WARNING //////////////////////////////////
 * This function calls malloc, and returns a pointer to the allocated memory! 
 * Functions which use init_key_cycle() are responsible for calling the 
 * destructor free_key_cycles() on the LAST key cycle created.
*******************************************************************************/
key_cycle_t* key_schedule(key_cycle_t* previous_key_cycle_p) {
    
    int i;
    
    key_cycle_t* key_cycle_p = (key_cycle_t*)malloc(sizeof(key_cycle_t));
    if(key_cycle_p == NULL) {printf("AAHH NULL POINTER DEBUG");}
    (*key_cycle_p).previous_key_cycle_p = previous_key_cycle_p;

    for(i=0; i<4; i++) {
        (*key_cycle_p).wkey[i]= f_function(
            (*previous_key_cycle_p).wkey[MOD(i+1,4)],
            (*previous_key_cycle_p).wkey[i]);
    }
    for(i=0; i<32; i++) {
        (*key_cycle_p).key[i]= f_function(
            (*previous_key_cycle_p).key[MOD(i+7,32)],
            (*previous_key_cycle_p).key[MOD(i-5,32)]);
    }
    return(key_cycle_p);
}

/******************************************************************************* 
 * This function frees a keycycle and all previous key cycles from it. Call when
 * done with key material.
 * inputs:
 * - key_cycle_p | A pointer to the last key cycle of the key cycles to be freed
 * outputs:
 * - none
*******************************************************************************/
void free_key_cycles(key_cycle_t* key_cycle_p) {
    
    /* Used to keep us from losing the pointer to the previous key cycle when
       freeing a key cycle */ 
    key_cycle_t* previous_key_cycle_p;
    
    #ifdef DEBUG
        int free_n = 0;
        printf(TEXTRED(0));
        printf("Freeing key cycles\n");
        printf(TEXTDEFAULT);
    #endif

    /* Frees key_cycles while there are more key cycles to free */
    while(key_cycle_p != NULL) {
        #ifdef DEBUG
            free_n++;
            printf("%d: %p\n",free_n,key_cycle_p);
        #endif
        previous_key_cycle_p = (*key_cycle_p).previous_key_cycle_p;
        free(key_cycle_p);
        key_cycle_p = previous_key_cycle_p;
    }
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
            printf(TEXTRED(0));
            printf("Scheduling key %d:\n",i);
            printf(TEXTDEFAULT);
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
 * This function decrypts an array of blocks of ciphertext from a file, and 
 * outputs the resulting plaintext.
 * inputs:
 * - plaintext_p | A pointer to a location where plaintext will be stored.
 * outputs:
 * - 1 or 0 | Has encrytion succeeded ? 1 : 0
 * - *(plaintext_p) is filled with the plaintext
*******************************************************************************/
int decrypt(unsigned long long* plaintext_p) {

    unsigned long long key[NUMBER_OF_KEY_BLOCKS];

    /* Get key material from file */
    file_handle_key(key,0);

    /* We need to hang onto the last generated key_cycle_p so we can hand it
    to free_key_cycles */
    key_cycle_t* last_key_cycle_p;

    int cipherBlocks,i;
    unsigned long long* ciphertext_p = NULL;

    /* Read the number of blocks of ciphertext. */
    cipherBlocks = read_header();
    if(!cipherBlocks) {
        printf("Loading ciphertext failed\n");
        return(0);
    }

    /* Load ciphertext into memory */
    ciphertext_p = load_ciphertext(cipherBlocks);
    if(!ciphertext_p) {
        printf("Loading ciphertext failed\n");
        return(0);
    }

    /* Generate a key cycle */
    key_cycle_t* key_cycle_p = init_key_cycle(key);
    
    /* Preschedule the number of keys needed so we can iterate backwards
    through them */
    for(i=0;i<cipherBlocks; i++) {
        key_cycle_p = key_schedule(key_cycle_p);
                #ifdef DETAILEDDEBUG
            printf(TEXTRED(0));
            printf("Scheduling key %d:\n",i);
            printf(TEXTDEFAULT);
            printf("key_cycle_p = %p\n",key_cycle_p);
        #endif
    }

    /* Hold onto the last keycycle  */
    last_key_cycle_p = key_cycle_p;

    /* Iterate backwards through the cipherblocks, decrypting them each with
    the appropriate key */
    for(i=cipherBlocks-1;i>=0;i--) {
        *(plaintext_p+i) = feistel_network(
            *(ciphertext_p+i),*key_cycle_p,0);  
        key_cycle_p = (*key_cycle_p).previous_key_cycle_p;
    }

    #ifdef DEBUG
        printf(TEXTRED(0));
        printf("Returning plaintext:\n");
        printf(TEXTDEFAULT);
        for(i=0;i<cipherBlocks;i++) {
            printf("%llx\n",*(plaintext_p+i));
        }
    #endif

    /* Clean up used memory */
    free(ciphertext_p);
    ciphertext_p = NULL;
    free_key_cycles(last_key_cycle_p);
    
    return(1);
}

/*******************************************************************************
 * This function saves a block of ciphertext to a file (database.ssdb).
 * inputs:
 * - ciphertext_p | A pointer to a location where ciphertext is stored
 * - number_of_blocks | The number of blocks in ciphertext_p
 * outputs:
 * - 1 or 0 | Has saving succeeded ? 1 : 0
*******************************************************************************/
int save_ciphertext(unsigned long long* ciphertext_p, int number_of_blocks) {
    int i; /* iterator */
    unsigned long long* savetext_p = ciphertext_p;

    FILE* fp = fopen(DBNAME, "wb");
    /* Check we succeed in opening the file */
    if(fp == NULL) {
        return(0);
    }

    #ifdef DEBUG
        printf(TEXTRED(0));
        printf("Saving ciphertext:\n");
        printf(TEXTDEFAULT);
    #endif

    /* Add a header that is the number of 64 bit blocks in the encrypted file.
    This is used to allocate size when loading. */
    fwrite(&number_of_blocks,sizeof(number_of_blocks),1,fp);

    /* Write all the blocks stored in memory to file. */
    for(i=0;i<number_of_blocks;i++) {
        fwrite((savetext_p+i),sizeof(*(savetext_p+i)),1,fp);
        #ifdef DEBUG 
            printf("%llx\n", *(savetext_p+i));
        #endif
    }

    /* Close file, clean up file pointer */
    fclose(fp);
    fp = NULL;
    return(1);
}

/*******************************************************************************
 * This function reads the header of a database file (database.ssdb).
 * inputs:
 * - none
 * outputs:
 * - size or 0 | Has saving succeeded ? size of file (in encrypted blocks) : 0
*******************************************************************************/
int read_header() {
    int header_val; /* Value of header */
    FILE* fp = fopen(DBNAME, "rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) {
        return(0);
    }

    /* Read the header from the file */
    fread(&header_val,sizeof(int),1,fp);
    #ifdef DETAILEDDEBUG
        printf("Header value: %d\n", header_val);
    #endif
    fclose(fp);
    fp = NULL;
    return(header_val);
}

/*******************************************************************************
 * This function loads ciphertext from a file and returns it.
 * inputs:
 * - number_of_blocks | The integer number of blocks to be read
 * outputs
 * -unsigned long long* | A pointer to the blocks loaded
*******************************************************************************/
unsigned long long* load_ciphertext(int number_of_blocks) {
    int i; /* Iterator */
    unsigned long long buffer; /* Buffer for fread */

    #ifdef DEBUG
        printf(TEXTRED(0));
        printf("Loading ciphertext:\n");
        printf(TEXTDEFAULT);
    #endif

    FILE* fp = fopen(DBNAME, "rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) {
        return(0);
    }
    fseek(fp,sizeof(int),SEEK_SET);

    /* Allocate a block of memory of the right size to hold the number of
    blocks being loaded */
    unsigned long long* ciphertext_p = (unsigned long long*)malloc(sizeof
        (unsigned long long)*number_of_blocks);
    if(ciphertext_p == NULL) {
        printf("Error: malloc failed");
    }

    /* Read the blocks into the memory of ciphertext_p */
    for(i = 0; i<number_of_blocks;i++) {
        fread(&buffer,sizeof(buffer),1,fp);
        *(ciphertext_p+i) = buffer;
        #ifdef DEBUG
            printf("%llx\n",buffer);
        #endif
    }
    
    return(ciphertext_p);
}
/*******************************************************************************
 * This function impliments the Feistel Network encryption structure used. This
 * structure is reversed by application backwards, which is controlled by the
 * "encrypt" flag. This function uses terminology consistent with encryption:
 * i.e, plaintext is input, ciphertext is output. In decryption
 * mode these actually behave as if reversed.
 * inputs:
 * - plaintext | An unsigned long long containing the binary plaintext to be 
 *      encrypted (or the binary ciphertext to be decrypted)
 * outputs:
 * - ciphertext |  An unsigned long long containing the binary ciphertext 
 *      encrypted (or the binary plaintext decrypted)
*******************************************************************************/
unsigned long long feistel_network(unsigned long long plaintext, 
    key_cycle_t keys, int encrypt) {
    unsigned long s_bits; /* Used to swap r_bits and l_bits */
    unsigned long l_bits = plaintext >> 32; /* The high 32 bits of the block */
    unsigned long r_bits = plaintext & EIGHTLOWMASK; /* The low 32 bits */
    int i; /* iterator */
    
    /* A whole bunch of debug tooling is here */
    #ifdef DEBUG
        /* If we're in either of the more detailed debug modes this should be
        a heading, whereas otherwise it shouldn't. */
        #if defined DETAILEDDEBUG || defined GRAPHICALDEBUG
            printf(TEXTRED(0));
        #endif
        printf("Feistel network running, mode is: %s\n", encrypt ? 
            "ENCRYPT":"DECRYPT");
        /* Return to default text if we just set to heading mode */
        #if defined DETAILEDDEBUG || defined GRAPHICALDEBUG
            printf(TEXTDEFAULT);
        #endif
    #endif

    /* Input whitening. Ternary operator used to select encrypt/decrypt modes */
    l_bits = l_bits ^ keys.wkey[encrypt ? 0:3];
    r_bits = r_bits ^ keys.wkey[encrypt ? 1:2];

        for(i = 0; i<32; i++) {
            
            #ifdef DETAILEDDEBUG
                printf("Round %d:\n",i+1);
                printf("in:  %8lx %8lx\n", l_bits, r_bits);
            #endif
            
            r_bits = f_function(l_bits,keys.key[encrypt ? i : 31-i])^r_bits;
            
            #ifdef DETAILEDDEBUG
                printf("out: %8lx %8lx\n", l_bits, r_bits);
            #endif
            
            /* Swap l_bits and r_bits */
            s_bits = l_bits;
            l_bits = r_bits;
            r_bits = s_bits;
        }

    /* Output whitening */
    l_bits = l_bits ^ keys.wkey[encrypt ? 2:1];
    r_bits = r_bits ^ keys.wkey[encrypt ? 3:0];

    /* Recreate and return 64 bit block. Swap lbits and rbits while doing so */
    return((r_bits << 32) | l_bits);
    }

/*******************************************************************************
 * Applies the F function, creating a very non-linear output from a given input
 * inputs:
 * - plaintext | An unsigned long to be mangled
 * - key | A second unsigned long to be used in the mangling
 * outputs:
 * - ciphertext | An unrecognisable mangling to the plaintext, but repeatably so
*******************************************************************************/
unsigned long f_function(unsigned long plaintext, unsigned long key) {
    /* XOR key material with the plaintext. Doing this before applying Wolfram's
    Rule 30 increases the key 'diffusion' - the number of bits of ciphertext
    affected by the change of one bit of key. According to Shannon's 
    principles of cryptography, this is a key goal of strong ciphers.*/
    unsigned long ciphertext = plaintext^key;
    int i;

    #ifdef GRAPHICALDEBUG
        printf("Wolfram's Rule 30 executing:\n");
        graph_rule_30(ciphertext);
    #endif

    /* Applies Wolfram's Rule 30 to the ciphertext. Increasing the number of 
    iterations here makes the F function less linear, increasing resistance 
    to differential cryptanalysis. It also increases diffusion. */
    for(i=0;i<WOLFRAMCYCLES;i++) {
        ciphertext = wolframs_rule_30(ciphertext);
        #ifdef GRAPHICALDEBUG
            graph_rule_30(ciphertext);
        #endif
    }

    return(ciphertext);
}

/*******************************************************************************
 * Applies the cellular automata described by Wolfram's Rule 30 across a 32 cell
 * domain. The rightmost and leftmost cell are treated as next to each other, 
 * so the block "wraps".
 * For more info on the rule itself: http://mathworld.wolfram.com/Rule30.html
 * inputs:
 * - plaintext | This unsigned long is used as the initial seed
 * outputs:
 * - ciphertext | The values of the cells after a number of generations
*******************************************************************************/
unsigned long wolframs_rule_30(unsigned long plaintext) {

    unsigned long ciphertext = 0;
    int i;
    for(i=0;i<32;i++) {
        /* This impliments Wolfram's Rule 30: 0001 1110 */
        unsigned int bits[3] = {(plaintext >> MOD(i-1,32))&1,
                                (plaintext >> (i))&1,
                                (plaintext >> MOD(i+1,32))&1};
            
            ciphertext = (ciphertext) | ((bits[0]^(bits[1]|bits[2]))<<i);
    }
    return(ciphertext);
}

/*******************************************************************************
 * Cryptographically Secure Pseudo-Random Number Generator
 * This gets a random number from /dev/urandom, which gets entropy from system
 * hardware. It is a cryptographically secure source of randomness suiitable for
 * key generation.
 * inputs:
 * - none
 * outputs:
 * random | an unsigned long long integer which is as random as feasible.
*******************************************************************************/
unsigned long long csprng() {
    unsigned long long random; /* Storage for the pseudorandom number */
    
    /* If in EVILDEBUG mode, instead of a random number return zero. This
    was needed for debugging, but is a REALLY BAD IDEA. It might look like it 
    still works for encrpytion, but turning this mode on reduces the key domain
    to have one value.
    Do not use with EVILDEBUG on for anything other than debugging. */
    #ifdef EVILDEBUG
        printf("EVILDEBUG IS ON! ENCRYPTION NON-FUNCTIONAL.\n");
        return(0ULL);
    #endif

    FILE* fp = fopen(URANDOM,"rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) { 
        return(0);
    }
    /* Read randomness */
    fread(&random, sizeof(unsigned long long), 1, fp);
    fclose(fp);
    return(random);
};

/*******************************************************************************
 * This function saves a key to a file, and reads a key from a file. The keys
 * are not encrypted, because that requires hardcoding keys somewhere else,
 * which is bad practise. Do not email keys in the same email as a database file
 * if you're aiming for security.
 * inputs:
 * - key[] | The key material to be saved, or memory to write key being loaded 
 *         | into.
 * - save | Whether the function saves or loads key material.
 * outputs:
 *  - int | Has the function suceeded ? 1 : 0
*******************************************************************************/
int file_handle_key(unsigned long long key[NUMBER_OF_KEY_BLOCKS],int save) {
    int i; /* Iterators */
    unsigned long long keyArray[NUMBER_OF_KEY_BLOCKS] = 
        {key[0],key[1],key[2],key[3]};
    
    FILE* fp = fopen(KEYNAME, save ? "wb":"rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) { 
        return(0);
    }

    if(!save) {
        fread(keyArray,sizeof(unsigned long long),4,fp);
    }

    #ifdef DEBUG
        printf(TEXTRED(0));
        printf(save?"Saving keys:\n":"Loading keys:\n");
        printf(TEXTDEFAULT);
    #endif

    if(save) {
        fwrite(keyArray,sizeof(unsigned long long),4,fp);
    }
    fclose(fp);

    /* Originally forgot this code, which loads key blocks. Worryingly,
    it still worked without this so long as encrypt and decrpyt were called
    close enough together. I think that what was happening was that so long
    as C hadn't had time to clean up the memory assigned to the key array, when
    decrypt called this function and was given uninitialized memory back, 
    because the array it was going into had the same name the same block was
    allocated, with the same values. */
    for(i=0;i<NUMBER_OF_KEY_BLOCKS;i++){
        key[i]=keyArray[i];
    }

    return(1);
}



#ifdef GRAPHICALDEBUG
/*******************************************************************************
 * This function graph's Wolfram's Rule 30 in GRAPHICALDEBUG mode.
 * inputs:
 * - row | The binary values of a row of 32 cells
 * outputs:
 *  - none
*******************************************************************************/
void graph_rule_30(unsigned long row) {
    int i;
    printf("[");
    for(i=0;i<32;i++) {
        /* Can't print a better block char than @ without unicode, which needs
        other libraries */
        printf("%c",(row)&1 ? '#':' ');
        row = row >> 1;
    }
    printf("]\n");
}
#endif