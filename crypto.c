/*******************************************************************************
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

/* This adds debug features */
/*#define DEBUG*/

/* This adds extra debug features 
#define DETAILEDDEBUG */

/* This debug mode helps make memory issues visible by removing the random
key generation. However, it also effectively turns off crypto, thus the
name. */
/* #define EVILDEBUG */

/* This adds visualizations of the application of Wolfram's rule 30
#define GRAPHICALDEBUG */

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

/* Selects the rightmost 8 bytes of a 16 byte int */
#define EIGHTLOWMASK 0xFFFFFFFF

/* The number of blocks of key material. */
#define NUMBER_OF_KEY_BLOCKS 4

/* The number of sections of key used for key whitening */
#define NUMBER_OF_WKEYS 4

/* The number of sections of key used in the F function */
#define NUMBER_OF_KEYS 32

/* The number of cycles of Wolfram's Rule 30 applied in the F function. Could
   be as low as 1, but higher numbers cause greater non-linearity. */
#define WOLFRAMCYCLES 5

/* This string is printed to set terminal text to default */
#define TEXTDEFAULT "\033[0m"

/* This string is printed to set terminal text to red. x is a boolean: 1 means 
   bold, 0 means normal. */
#define TEXTRED(x) "\033["#x";31m"

/* This string is printed to set terminal text to blue. x is a boolean: 1 means 
   bold, 0 means normal. */
#define TEXTBLUE(x) "\033["#x";34m"

/* Used to pad titles in debug mode */
#define TITLE "===================="

/*******************************************************************************
 * Structure Definitions
*******************************************************************************/

/* This structure holds a 'cycle' of keys - all the keys needed to encrypt a
   single block of ciphertext. In addition, it holds a pointer to the previous
   key cycle - needed in decryption to iterate backwards through key cycles, but
   also needed for deallocation of memory. */
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

/* Loads an encrypted file into memory */
unsigned long long* load_ciphertext(int number_of_blocks);

/* The core encryption scheme is a feistel network. It takes a block and key 
   material, and either encryts or decrypts the block */
unsigned long long feistel_network(unsigned long long plaintext,key_cycle_t key,
    int encrypt);

/* Also known as a "round function", the heart of a feistel cipher. Takes a 
   block of plaintext, performs pseudo-random number generation with
   it as the seed, and then XORs in the key. Is strongly non-linear. */
unsigned long f_function(unsigned long plaintext, unsigned long key);

/* Performs the pseudo-random number generation used in the F-function. */
unsigned long wolframs_rule_30(unsigned long plaintext);

/* Cryptographically Secure Pseudo-Random Number Generator. Generates a key 
   from system randomness provided in /dev/urandom. Is Linux system dependent.*/
unsigned long long csprng();

/* Saves the key to a file, or loads it from one. */
int file_handle_key(unsigned long long key[NUMBER_OF_KEY_BLOCKS],int save);

#ifdef GRAPHICALDEBUG
    /* Graphs Wolfram's rule 30 */
    void graph_rule_30(unsigned long row);
#endif

/******************************************************************************* 
 * This function creates a new key cycle struct from 256 bits of key material.
 * This should only be used to create the very first key cycle, from then on use
 * key_schedule().
 * inputs:
 * - key[] | The array of key material used to initialize the key cycle
 * outputs:
 * - key_cycle_p | A pointer to the initialized key_cycle_p. 
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
 * - key_cycle_p | A pointer to the new key cycle 
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

    /* Frees the key_cycles created for encryption */
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
    
    #ifdef DEBUG
        printf(TEXTRED(1));
        printf("\n"TITLE"ENCRYTPING"TITLE"\n");
        printf(TEXTDEFAULT);
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
 * Decrypt
*******************************************************************************/
int decrypt(unsigned long long* plaintext_p) {
    
    #ifdef DEBUG
        printf(TEXTBLUE(1));
        printf("\n"TITLE"DECRYTPING"TITLE"\n");
        printf(TEXTDEFAULT);
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
            printf(TEXTRED(0));
            printf("Scheduling key %d:\n",i);
            printf(TEXTDEFAULT);
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
        printf(TEXTRED(0));
        printf("Returning plaintext:\n");
        printf(TEXTDEFAULT);
        for(i=0;i<cipherBlocks;i++) {
            printf("%llx\n",*(plaintext_p+i));
        }
    #endif

    free(ciphertext_p);
    ciphertext_p = NULL;

    free_key_cycles(last_key_cycle_p);
    
    return(1);
}

/*******************************************************************************
 * Save Ciphertext
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
       This is used to check we have a complete file when loading. */
    fwrite(&number_of_blocks,sizeof(number_of_blocks),1,fp);

    for(i=0;i<number_of_blocks;i++) {
        fwrite((savetext_p+i),sizeof(*(savetext_p+i)),1,fp);
        #ifdef DEBUG 
            printf("%llx\n", *(savetext_p+i));
        #endif
    }

    fclose(fp);
    fp = NULL;
    return(1);
}

/*******************************************************************************
 * Read header
*******************************************************************************/
int read_header() {
    int header_val;
    FILE* fp = fopen(DBNAME, "rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) {
        return(0);
    }
    fread(&header_val,sizeof(int),1,fp);
    #ifdef DETAILEDDEBUG
        printf("Header value: %d\n", header_val);
    #endif
    fclose(fp);
    fp = NULL;
    return(header_val);
}

/*******************************************************************************
 * Load Ciphertext
*******************************************************************************/
unsigned long long* load_ciphertext(int number_of_blocks) {
    int i;
    unsigned long long buffer;

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

    unsigned long long* ciphertext_p = (unsigned long long*)malloc(sizeof
        (unsigned long long)*number_of_blocks);
    if(ciphertext_p == NULL) {
        printf("Error: malloc failed");
    }

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
 * Feistel Network
*******************************************************************************/
unsigned long long feistel_network(unsigned long long plaintext, key_cycle_t keys,
    int encrypt) {
    unsigned long s_bits; /* Used to swap r_bits and l_bits */
    unsigned long l_bits = plaintext >> 32;
    unsigned long r_bits = plaintext & EIGHTLOWMASK;
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


    /* Input whitening */
    l_bits = l_bits ^ keys.wkey[encrypt ? 0:3];
    r_bits = r_bits ^ keys.wkey[encrypt ? 1:2];

        for(i = 0; i<32; i++) {
            
            #ifdef DETAILEDDEBUG
                printf("Round %d:\n",i+1);
                printf("in:  %8lx %8lx\n", l_bits, r_bits);
            #endif
            
            r_bits = f_function(l_bits,keys.key[encrypt ? i : 31-i])^r_bits;
            /* Swap l_bits and r_bits */
            
            #ifdef DETAILEDDEBUG
                printf("out: %8lx %8lx\n", l_bits, r_bits);
            #endif
            
            s_bits = l_bits;
            l_bits = r_bits;
            r_bits = s_bits;
        }

    /* Output whitening */
    l_bits = l_bits ^ keys.wkey[encrypt ? 2:1];
    r_bits = r_bits ^ keys.wkey[encrypt ? 3:0];

    return((r_bits << 32) | l_bits);
    }
/*******************************************************************************
 * F Function
*******************************************************************************/
unsigned long f_function(unsigned long plaintext, unsigned long key) {
    /* XOR key material with the plaintext. Doing this before applying Wolfram's
       Rule 30 increases the key 'diffusion' - the number of bits of ciphertext
       affected by the change of one bit of key. According to Shannon's 
       principles of cryptography, this is a key goal of strong ciphers.*/
    unsigned long cryptotext = plaintext^key;
    int i;

    /* Applies Wolfram's Rule 30 to the ciphertext. Increasing the number of 
       iterations here makes the F function less linear, increasing resistance 
       to differential cryptanalysis. It also increases diffusion, in accordance
       with Shannon's principles of cryptography. */
    #ifdef GRAPHICALDEBUG
        printf("Wolfram's Rule 30 executing:\n");
        graph_rule_30(cryptotext);
    #endif
    for(i=0;i<WOLFRAMCYCLES;i++) {
        cryptotext = wolframs_rule_30(cryptotext);
        #ifdef GRAPHICALDEBUG
            graph_rule_30(cryptotext);
        #endif
    }

    return(cryptotext);
}

/*******************************************************************************
 * Wolfram's Rule 30
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
*******************************************************************************/
unsigned long long csprng() {
    unsigned long long random; /* Storage for the pseudorandom number */
    
    /* If in debug mode, print where we are in the code as a title */
    #ifdef EVILDEBUG
        return(0ULL);
    #endif


    FILE* fp = fopen(URANDOM,"rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) { 
        return(0);
    }
    fread(&random, sizeof(unsigned long long), 1, fp);
    fclose(fp);
    return(random);
};


/*******************************************************************************
 * Saves key to a file
*******************************************************************************/
int file_handle_key(unsigned long long key[NUMBER_OF_KEY_BLOCKS],int save) {
    int i; /* Iterators */
    unsigned long long keyArray[NUMBER_OF_KEY_BLOCKS] = {key[0],key[1],key[2],key[3]};
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

    for(i=0;i<NUMBER_OF_KEY_BLOCKS; i++) {
        /*keyArray[i] = key_muddle(keyArray[i]);*/
        #ifdef EVILDEBUG
            printf("%d: %llx\n",i+1,keyArray[i]);
        #endif
    }
    if(save) {
        fwrite(keyArray,sizeof(unsigned long long),4,fp);
    }
    fclose(fp);

    for(i=0;i<NUMBER_OF_KEY_BLOCKS;i++){
        key[i]=keyArray[i];
    }

    return(1);
}

#ifdef GRAPHICALDEBUG
/* Graphs Wolfram's rule 30 */
void graph_rule_30(unsigned long row) {
    int i;
    printf("[");
    for(i=0;i<32;i++) {
        printf("%c",(row)&1 ? '#' :' ');
        row = row >> 1;
    }
    printf("]\n");
}
#endif