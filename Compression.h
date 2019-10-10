/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3, Huffman Compression
 * Header file
 * Name: Ray Robinson
 * Student ID: 12616292
 * Date of submission: 16/10/2019
 * 
 * This file forms part of the Substation System Databse (SSD) which has been 
 * designed and built by team 3. 
 * This compression header file provides the data structure and compression
 * functions called withing the SSD 
 *  
*******************************************************************************/


/*******************************************************************************
*Header files which are required
*******************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 

/*******************************************************************************
*List of preprocessing directives
*******************************************************************************/
#define MAX_TREE_HT 100
#define MAXLEN 100000
#define ASCII_CODE 127


/*******************************************************************************
*List of data structures*
*******************************************************************************/

/* A Huffman tree node  */
struct MinHeapNode { 

	/*  One of the input characters  */
	char data; 

	/*  Frequency of the character  */
	unsigned freq; 

	/*  Left and right child of this node  */
	struct MinHeapNode *left, *right; 
}; 

typedef struct MinHeapNode MinHeapNode_t;


/* 
A Min Heap: Collection of 
 min-heap (or Huffman tree) nodes  */

struct MinHeap { 

	/*  Current size of min heap  */
	unsigned size; 
/* 
	 capacity of min heap  */
	unsigned capacity; 

/* 	 Array of minheap node pointers  */
	MinHeapNode_t** array; 
}; 

typedef struct MinHeap MinHeap_t;


/* 
A Min Heap: Collection of 
 min-heap (or Huffman tree) nodes  */

struct Huffman { 

	/*  ASCII code  */
	int data; 

/*	 The Huffman code of character  */
	char Huffman_Code[MAX_TREE_HT] ; 

	/* Size of Huffman code */
	int size ;

}; 

typedef struct Huffman Huffman_t;

/*******************************************************************************
 * Function prototypes 
*******************************************************************************/

/*  The overall compression function which calls the functions listed below to 
carry out more specific functions  */
void Compression (root_t* root_p);

/*  function allocate a new  min heap node with given character 
 and frequency of the character  */
MinHeapNode_t* newNode(char data, unsigned freq) ;

/*  function to create a min heap of given capacity  */
struct MinHeap* createMinHeap(unsigned capacity);

/*  function to swap two min heap nodes  */
void swapMinHeapNode(MinHeapNode_t** a, MinHeapNode_t** b) ;

/*  The standard minHeapify function.  */
void minHeapify(struct MinHeap* minHeap, int idx); 

/*  function to check  if size of heap is 1 or not  */
int isSizeOne(struct MinHeap* minHeap); 

/*  function to extract  minimum value node from heap  */
MinHeapNode_t* extractMin(MinHeap_t* minHeap) ;

/* function to insert  a new node to Min Heap  */
void insertMinHeap(MinHeap_t* minHeap, MinHeapNode_t* MinHeapNode_t);

/*  A standard function to build min heap  */
void buildMinHeap(struct MinHeap* minHeap); 

/*  function to print an array of size n  */
Huffman_t printArr(const int str[], const int n,const int freq,const char data) ;

/*  function to store Huffman codes  */
Huffman_t write_Huff_array(const int code[], const int n, const int ASCII, const int freq);

/* function to check if this node is leaf  */
int isLeaf(MinHeapNode_t* root);

/*  Creates a min heap of capacity equal to size and inserts all character of 
 data[] in min heap. Initially size of min heap is equal to capacity  */
MinHeap_t* createAndBuildMinHeap(char data[], int freq[], int size) ;

/*  The main function that builds Huffman tree  */
MinHeapNode_t* buildHuffmanTree(char data[], int freq[], int size);

/*  Prints huffman codes from the root of Huffman Tree. 
 	It uses str[] to store codes  */
void printCodes(MinHeapNode_t* root, int str[], int top, int size, Huffman_t tree[],int* row);

/*  The main function that builds a Huffman Tree and print codes by
    traversing the built Huffman Tree  */
void HuffmanCodes(char data[], int count[], int size, Huffman_t tree[]);

void saveoutput(Huffman_t tree[], int size, int str_raw[]);


/*EOF*/

