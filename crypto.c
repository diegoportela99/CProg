#include <stdio.h>

struct keyCycle {
    unsigned int keyBytes[32];
    struct keyCycle* previousKeyCycle_p;
};
typedef struct keyCycle keyCycle_t;

unsigned int s_box[4][4] = {{0b11,0b00,0b01,0b10},
                            {0b01,0b10,0b00,0b11},
                            {0b10,0b01,0b11,0b00},
                            {0b00,0b11,0b10,0b01}
};

unsigned int shuffle_box[8] = {5,2,1,4,7,0,3,6}; 

/**************************** FUNCTION PROTOTYPES *****************************/
unsigned int apply_feistel_network(unsigned int s[4][4],
    unsigned int rotation_box[8],unsigned int plaintext, unsigned int key[], 
    int number_of_cycles,int encrypt);

unsigned int apply_f_function(unsigned int s[4][4],unsigned int rotation_box[8],
    unsigned int plaintext, unsigned int key, int encrypt);

unsigned int apply_s_box(unsigned int s[4][4], unsigned int plaintext,
    unsigned int key, int encrypt);

unsigned int shuffle_bits(unsigned int rotation_box[8], unsigned int plaintext, 
    int encrypt);

unsigned int getBitFromNum(unsigned int bitNum, unsigned int num);

/********************************* MAIN ***************************************/
int main(void) {

    unsigned int plaintext = 0b11110110;
    unsigned int key[4] = {0b11011101, 0b10111101, 0b11010110, 0b10100100};
    unsigned int ciphertext;

    printf("Encrypting:\n");
    
    ciphertext = apply_feistel_network(s_box,shuffle_box,plaintext,key,2,1);
    printf("Plaintext: %x Ciphertext: %x\n",plaintext,ciphertext);

    printf("\n");

    printf("Decrypting:\n");
    
    plaintext = apply_feistel_network(s_box,shuffle_box,ciphertext,key,2,0);
    printf("Ciphertext: %x Plaintext: %x\n",ciphertext,plaintext);

    return(0);
}

/*******************************************************************************
 * Apply Feistal network
*******************************************************************************/
unsigned int apply_feistel_network(unsigned int s[4][4],
    unsigned int rotation_box[8],unsigned int plaintext, unsigned int key[], 
    int number_of_cycles,int encrypt) {

    int i;
    unsigned int ciphertext;

    plaintext = plaintext ^ key[!encrypt * (number_of_cycles + 1)];

    for(i = 1; i<=number_of_cycles; i++) {
        int j = encrypt ? i : number_of_cycles+1-i;
        ciphertext = apply_f_function(s, shuffle_box,plaintext,key[j],
            encrypt);
        plaintext = ciphertext;
    }
    plaintext = plaintext ^ key[encrypt * (number_of_cycles + 1)];
return plaintext;
}

/*******************************************************************************
 * Apply F function, or "Round" function
*******************************************************************************/
unsigned int apply_f_function(unsigned int s[4][4],unsigned int shuffle_box[8],
    unsigned int plaintext, unsigned int key, int encrypt) {
    
    unsigned int ciphertext = plaintext;

    if (!encrypt) {
        ciphertext = shuffle_bits(shuffle_box,ciphertext,0);
    }
    
    ciphertext = apply_s_box(s,ciphertext,key,encrypt);

    if(encrypt) {
        ciphertext = shuffle_bits(shuffle_box,ciphertext,1);
    }
    return ciphertext;
}

/*******************************************************************************
 * Apply s_box to plaintext using key. Returns ciphertext.
 * flag encrypt selects encrypt/decrypt mode.
 * Naming conventions within the function assume that encryption is taking place
*******************************************************************************/
unsigned int apply_s_box(unsigned int s[4][4],unsigned int plaintext,
    unsigned int key, int encrypt) {

    int i; /* Iterator */
    unsigned int ciphertext = 0b0; /* the output encrypted text */

    /* iterate through an eight bit block two bit at a time */
    for(i=0;i<8;i+=2)
    {
        /* the encrypted two bits */
        unsigned int cipherbits; 
        unsigned int keybits;
        unsigned int plainbits;

        /* two bits are selected from the plaintext */
        plainbits = (getBitFromNum(i+1,plaintext)<<1) |
            (getBitFromNum(i,plaintext));
        
        if(encrypt) {
        /* Two bits are selected from the key */
        keybits = (getBitFromNum(i+1,key)<<1) |
            (getBitFromNum(i,key));
        
        } else {
        /* To decrypt, we need to parse the key in the opposite direction */
        keybits = (getBitFromNum(7-i,key)<<1) |
            (getBitFromNum(6-i,key));
        }

        if(encrypt) {
        /* The key and plaintext bits are used to look up the resultant
           bits in the s_box */
        cipherbits = s_box[plainbits][keybits];
        }

        /* Decrypting is a tad more involved. A version of this program more
        optimised for execution time would use a separate lookup table. */
        else {
            unsigned int i;
            for(i = 0; i<4; i++) {
                if(s_box[i][keybits] == plainbits) {
                    cipherbits = i;
                }
            }
        }
        /* the encrypted bits are appended to the ciphertext */
        ciphertext = ciphertext<<2 | cipherbits;

    }
    return ciphertext;
}

/*******************************************************************************
 * Rotate bits (reversably)
*******************************************************************************/

unsigned int shuffle_bits(unsigned int rot_box[8], unsigned int plaintext, 
    int encrypt) {
    /*printf("%x ", plaintext); DEBUG*/
    unsigned int ciphertext = 0b00000000;
    unsigned int mask;
    int i;

    for(i=0; i<8; i++) {
        if(encrypt) {
            /* If we're encrypting, work out which bit in the ciphertext needs
            to be edited, and the value it needs to take on */
            mask = getBitFromNum(7-i,plaintext)<<(7-rot_box[i]);
            /* If the mask bit is 1, add it to the ciphertext */
            ciphertext = ciphertext | (mask);
        }
        else {
            /* If we're decrypting, append the appropriate bit from the 
            'plaintext' to the 'ciphertext' */
            ciphertext = ciphertext<<1 | getBitFromNum(7-rot_box[i],plaintext); 
        }
    }
    /*printf("%x ", ciphertext); DEBUG*/
    return ciphertext;
}

unsigned int getBitFromNum(unsigned int bitNum, unsigned int num) {
    return((num >> bitNum) & 1);
}