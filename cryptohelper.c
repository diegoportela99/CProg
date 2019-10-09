#include "cryptohelper.h"
#include "debug.h"
#include <string.h>
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
            print_header("Initializing key cycle:\n");
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
        print_header("Freeing key cycles:\n");
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
 * Save Ciphertext
*******************************************************************************/
int save_ciphertext(unsigned long long* ciphertext_p, int number_of_blocks) {
    int i; /* iterator */
    unsigned long long* savetext_p = ciphertext_p;
    char filename[BUFSIZ];

    printf("Please enter a filename for the encrypted database: ");
    fgets(filename, BUFSIZ, stdin);
    sscanf(filename, "%[^\n]", filename);
    strcat(filename, ".ssdb");

    FILE* fp = fopen(filename, "wb");
    /* Check we succeed in opening the file */
    if(fp == NULL) {
        printf("Filehandling error\n");
        return(0);
    }

    #ifdef DEBUG
        print_header("Saving ciphertext:\n");
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
    char filename[BUFSIZ];

    #ifdef DEBUG
        print_header("Loading ciphertext:\n");
    #endif

    printf("Please enter the name of an encrypted database to load: ");
    fgets(filename, BUFSIZ, stdin);
    sscanf(filename, "%[^\n]", filename);

    FILE* fp = fopen(filename, "rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) {
        printf("Filehandling error\n");
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
    #ifdef DEBUG
        printf("Generating key...\n");
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
    unsigned long long keyArray[4] = {key[0],key[1],key[2],key[3]};
    FILE* fp = fopen(KEYNAME, save ? "wb":"rb");
    /* Check we succeed in opening the file */
    if(fp == NULL) { 
        return(0);
    }

    if(!save) {
        fread(keyArray,sizeof(unsigned long long),4,fp);
    }

    #ifdef DEBUG
        print_header(save?"Saving keys:\n":"Loading keys:\n");
    #endif

    for(i=0;i<NUMBER_OF_KEY_BLOCKS; i++) {
        /*keyArray[i] = key_muddle(keyArray[i]);*/
        #ifdef DEBUG
            printf("%d: %llx\n",i+1,keyArray[i]);
        #endif
    }
    if(save) {
        fwrite(keyArray,sizeof(unsigned long long),4,fp);
    }
    fclose(fp);

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

