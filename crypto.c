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

#define DEBUG

/* Database name */
#define DBNAME "database"

/* Defines a modulus which behaves correctly for negative values, unlike  "%" 
   Mathematically, negative inputs to mod should wrap round to positive. */
#define MOD(a,b) ((((a)%(b))+(b))%(b)) 

/* Selects the rightmost 8 bytes of a 16 byte int */
#define EIGHTLOWMASK 0xFFFFFFFF 

/*******************************************************************************
 * Structure Definitions
*******************************************************************************/
struct key_cycle {
    unsigned long long wkey[4]; /* The sections of key used for key whitening */
    unsigned long long key[32]; /* The sections of key used in the F function */
    
    /* Links to previous key cycle. Needed to iterate backwards through key
       cycles for decryption */
    struct key_cycle* previous_key_cycle_p; 
};
typedef struct key_cycle key_cycle_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/
/* Initializes the first key cycle */
key_cycle_t* init_key_cycle(unsigned long long key[4]);

/* Schedules a new key cycle given the previous one */
key_cycle_t* key_schedule(key_cycle_t* previous_key_cycle_p);

/* Encrypts 64 bit blocks of plaintext, given an array of plaintext blocks,
    the number of blocks of plaintext, and a key.
    Writes ciphertext to file. */
int encrypt(unsigned long long plaintext[], 
    int number_of_blocks, unsigned long long key[4]);

int decrypt(unsigned long long* plaintext_p,
    unsigned long long key[4]);

int save_ciphertext(unsigned long long* ciphertext_p, int number_of_blocks);

int read_header();

unsigned long long* load_ciphertext(int number_of_blocks);

unsigned long long feistel_network(unsigned long long plaintext,key_cycle_t key,
    int encrypt);

unsigned long f_function(unsigned long plaintext, unsigned long key);

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void) {
    unsigned long long key[4] = {0xfe1443539c74d13c,
                                 0x8dedd14a0de5c057,
                                 0x9cad51f195a8bebf,
                                 0x24e210ec42c7d2f3};

    unsigned long long plaintext[4] = {0x2d1cd84c6567854c, 
                                       0x3853166d394825d4,
                                       0xc325242519821d33,
                                       0x2690fc4452ed668e};
    
    unsigned long long decrypted[4] = {0,0,0,0};

    encrypt(plaintext,4,key);
    decrypt(decrypted,key);
    printf("Plaintext:\n%llx\n%llx\n%llx\n%llx\n",
        decrypted[0],decrypted[1],decrypted[2],decrypted[3]);

    return(0);
}
/*******************************************************************************
 * Key to Key Cycle - Generate a key cycle from a key
*******************************************************************************/
key_cycle_t* init_key_cycle(unsigned long long key[]) {
        int i;
        key_cycle_t* key_cycle = (key_cycle_t*)malloc(sizeof(key_cycle_t));
        if(key_cycle == NULL) {printf("AAHH NULL POINTER DEBUG");}
        

        #ifdef DEBUG
            printf("Initializing key cycle\n");
        #endif

        (*key_cycle).previous_key_cycle_p = NULL;

        unsigned long temp[4] = {f_function(key[0]>>32,key[1]&EIGHTLOWMASK),
                                 f_function(key[1]>>32,key[2]&EIGHTLOWMASK),
                                 f_function(key[2]>>32,key[3]&EIGHTLOWMASK),
                                 f_function(key[3]>>32,key[0]&EIGHTLOWMASK)};
        
        for(i = 0; i<4; i++) {
            temp[0] = f_function(temp[2],temp[1]);
            temp[1] = f_function(temp[3],temp[2]);
            temp[2] = f_function(temp[0],temp[3]);
            temp[3] = f_function(temp[1],temp[0]);
            (*key_cycle).wkey[i] = f_function(temp[0],temp[1]);
            
            #ifdef DETAILEDDEBUG
                printf("WKey %d: %llx ", i, (*key_cycle).wkey[i]);
            #endif
        }
        
        for(i = 0; i<32; i++) {
            temp[0] = f_function(temp[2],temp[1]);
            temp[1] = f_function(temp[3],temp[2]);
            temp[2] = f_function(temp[0],temp[3]);
            temp[3] = f_function(temp[1],temp[0]);
            (*key_cycle).key[i] = f_function(temp[0],temp[1]);
            #ifdef DETAILEDDEBUG
                printf("Key %d: %llx ", i, (*key_cycle).key[i]);
            #endif
        }
        #ifdef DETAILEDDEBUG
            printf("\n");
        #endif
    return key_cycle;
}

/*******************************************************************************
 * Key Schedule
*******************************************************************************/
key_cycle_t* key_schedule(key_cycle_t* previous_key_cycle_p) {
    
    int i;
    
    key_cycle_t* key_cycle_p = (key_cycle_t*)malloc(sizeof(key_cycle_t));
    if(key_cycle_p == NULL) {printf("AAHH NULL POINTER DEBUG");}
    (*key_cycle_p).previous_key_cycle_p = previous_key_cycle_p;

    for(i=0; i<4; i++) {
        (*key_cycle_p).wkey[i]= f_function((*previous_key_cycle_p).wkey[MOD(i+1,4)],
                                         (*previous_key_cycle_p).wkey[i]);
    }
    for(i=0; i<32; i++) {
        (*key_cycle_p).key[i]= f_function((*previous_key_cycle_p).key[MOD(i+7,32)],
                                        (*previous_key_cycle_p).key[MOD(i-5,32)]);
    }
    return(key_cycle_p);
}
/*******************************************************************************
 * Encrypt
*******************************************************************************/
int encrypt(unsigned long long plaintext[], 
    int number_of_blocks, unsigned long long key[4]) {
    int i; /* iterator */

    key_cycle_t* key_cycle_p = init_key_cycle(key);
    key_cycle_t* previous_key_cycle_p;

    unsigned long long* ciphertext_p  = (unsigned long long*)malloc(sizeof
        (unsigned long long)*number_of_blocks);
    if(ciphertext_p == NULL) {printf("AAHH NULL POINTER DEBUG");}
    
    /* Generate key cycles for each block, and then encrypt them. */
    for(i=0; i<number_of_blocks;i++) {
        key_cycle_p = key_schedule(key_cycle_p);
        #ifdef DETAILEDDEBUG
            printf("Scheduled key %d, key_cycle_p = %p\n",i,key_cycle_p);
        #endif
        (*(ciphertext_p+i)) = feistel_network(plaintext[i],*key_cycle_p,1);
        
    }

    save_ciphertext(ciphertext_p, number_of_blocks);
    free(ciphertext_p);
    ciphertext_p = NULL;

    #ifdef DEBUG
        int free_n = 0;
    #endif

    while(key_cycle_p != NULL) {
        #ifdef DEBUG
        free_n++;
            printf("Freeing key %d: %p\n",free_n,key_cycle_p);
        #endif
        previous_key_cycle_p = (*key_cycle_p).previous_key_cycle_p;
        free(key_cycle_p);
        key_cycle_p = previous_key_cycle_p;
    }
    return(1);
}

/*******************************************************************************
 * Decrypt
*******************************************************************************/
int decrypt(unsigned long long* plaintext_p,
    unsigned long long key[4]) {
    
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

    #ifdef DEBUG
        for(i=0;i<cipherBlocks;i++) {
           printf("%llx\n",*(ciphertext_p+i));
        }
    #endif
    
    printf("CipherBlocks: %d\n", cipherBlocks);
    key_cycle_t* key_cycle_p = init_key_cycle(key);
    
    for(i=0;i<cipherBlocks; i++) {
        printf("Scheduling key %d\n", i);
        key_cycle_p = key_schedule(key_cycle_p);
        
    }

    for(i=cipherBlocks-1;i>=0;i--) {
        (*(plaintext_p+i)) = feistel_network(
            *(ciphertext_p+i),*key_cycle_p,0);  
        key_cycle_p = (*key_cycle_p).previous_key_cycle_p;
        printf("Decrypting block %d\n",i);
    }

    free(ciphertext_p);
    ciphertext_p = NULL;
    free(key_cycle_p);
    key_cycle_p = NULL;
    
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
        printf("Saving ciphertext:\n");
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
    #ifdef DEBUG
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
        #ifdef DETAILEDDEBUG
            printf("%llx\n",buffer);
        #endif
    }
    
    /* Note: we have to account for the increment of i where the condition in
    the for loop is false */
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


    #ifdef DEBUG
        printf("Feistel network running, mode is: %s\n", encrypt ? 
            "ENCRYPT":"DECRYPT");
    #endif


    /* Input whitening */
    l_bits = l_bits ^ keys.wkey[encrypt ? 0:3];
    r_bits = r_bits ^ keys.wkey[encrypt ? 1:2];

        for(i = 0; i<32; i++) {
            
            #ifdef DETAILEDDEBUG
                printf("Round %d: ",i+1);
                printf("in: %8lx %8lx ", l_bits, r_bits);
            #endif
            
            r_bits = f_function(l_bits,keys.key[encrypt ? i : 31-i])^r_bits;
            /* Swap l_bits and r_bits */
            
            #ifdef DETAILEDDEBUG
                printf("key: %llx out: %8lx %8lx\n", keys.key[encrypt ? i : 31-i], l_bits, r_bits);
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
    
    unsigned long cryptotext = 0;
    int i;

    for(i=0;i<32;i++) {
        /* This impliments Wolfram's Rule 30: 0001 1110*/
        unsigned int bits[3] = {(plaintext >> MOD(i-1,32))&1,
                                (plaintext >> (i))&1,
                                (plaintext >> MOD(i+1,32))&1};
            
            cryptotext = (cryptotext << 1) | (bits[0]^(bits[1]|bits[2]));
    }
    return(cryptotext^key);
}
