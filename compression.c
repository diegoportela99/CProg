/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3, Huffman Compression
 * Name: Ray Robinson
 * Student ID: 12616292
 * Date of submission: 16/10/2019
 * 
 * This file forms part of the Substation System Databse (SSD) which has been 
 * designed and built by team 3. 
 * This compression program takes data in from the structure which holds all 
 * the substation telemetry information, builds a Huffman Tree which is a form
 * of a binary tree which utilises min heaps to replace the characters with
 * the most occurances with a binary code which has less bits than it would 
 * otherwise. 
 * Thie functionality can be reversed and return the data back to its original 
 * form also.
 * 
*******************************************************************************/

/*******************************************************************************
*Header files which are required
*******************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "btree.h"
#include "substation.h"
#include "Compression.h"
#include "debug.h"

/*******************************************************************************
*List of preprocessing directives
*******************************************************************************/

/*#define DEBUG*/

int Huffman_Code[20];



/*******************************************************************************
 * Main
*******************************************************************************/

/*  Main function which takes input from user UPDATE */

/*int main()*/
void Compression (root_t* root_p) {
	

	int loopVar;

	/*Maximum amount of characters which can be processed*/
	char str_raw[MAXLEN];
	strcpy(str_raw, "");
	/*The number each of ASCII characters occurances in the data set used*/
	int count_raw[ASCII_CODE];
	int i;




	/*import function to pull data in*/

	for (loopVar = 0; loopVar < (*root_p).number_of_entries-1; loopVar++) 
	{
		/* Gets the data point at the specified index */
		telemetry_point_t* current = get_telemetry_point(loopVar, root_p);
		strcat(str_raw,(*current).location); 
		strcat(str_raw,",");
		strcat(str_raw,(*current).desig);
		strcat(str_raw,",");
		strcat(str_raw,(*current).plant);
		strcat(str_raw,",");
		strcat(str_raw,(*current).network); 
		strcat(str_raw,",");
		strcat(str_raw,(*current).quantity);
		strcat(str_raw,",");
		strcat(str_raw,(*current).protocol);
		strcat(str_raw,",");
		strcat(str_raw,(*current).number);
		strcat(str_raw,",");
		strcat(str_raw,(*current).address);
		strcat(str_raw,",");
		strcat(str_raw,(*current).moduletype);
		strcat(str_raw,",");
		strcat(str_raw,(*current).failed);
		strcat(str_raw,",");
		strcat(str_raw,(*current).online);
		strcat(str_raw,",");   
		strcat(str_raw,(*current).faulty );
		strcat(str_raw,","); 
		strcat(str_raw,(*current).oos);
		strcat(str_raw,",");
		strcat(str_raw,"\n");

	}



	printf("Data compression is underway\n");

	/*Start processing data strings now..*/

	/* This sets the raw count of all characters in an array to 0 to begin with  */
    for(i=0;i<ASCII_CODE;i++)
    {
        count_raw[i]=0;
	}

    /*inits i to 0, for less than str length it will add 
	* a value to the ASCII index of letter found 
	*/
  
	for(i=0;i<strlen(str_raw);i++)
    {
        
		count_raw[(int)str_raw[i]] ++;

    }
	
	

	/*Loop which is used to find out how many unique characters were entered
	* by checking if the character occurs atleast once, if it doesn't it 
	* keeps searching through the array, if it does occur atleast once it
	* increments the size which determines the size of the consolidated arrays
	* used in the next part of the program.
	*/
	int size=0;
	int j=0;
	for (j=0;j<ASCII_CODE;j++){
		if (count_raw[j]>0){
		size++;
		
	
		}
		
	}

	/*The compression algorithm only needs to know which characters are present and their
	* number of occurances. Here we cut down from an array of 256 (ASCII characters avail)
	* to an array of size as determined by the characters only present.
	*/

	/*Arrays for consolidated count and characters (str) which are initialised to the size
	* as determined above.*/
	int count_consol[size];
	char str_consol[size];

	/*Loop through all 255 characters in the original array, if the count is equal to 
	* atleast 1 transfer it accross to the next available element of the consolidated
	* array for both the character and its count.
	*/
	i=0;
	for(j=0;j<ASCII_CODE;j++){
		/*printf("%d\n",(int)count_raw[j]);*/
		if ((int)count_raw[j] !=0) {
			count_consol [i]= count_raw[j];
			str_consol[i] = j;
			i++;
		}
	}

	

	/*Debugging print statement to show that the raw count and 
	* char have been transferred into the cut down colsolidated array.
	*/
	#ifdef DEBUG
	for(i=0;i<size;i++)
	{
		printf("consolidated chars are %c & ASCII Code  %d",str_consol[i],str_consol[i]);
		printf(" and they occur %d times\n",count_consol[i]);
	}	
	#endif
	
	


	/*Send off the relvant consolidated data to the Huffman functions 
	 *to build out tree and export data to the encryption step*/

	Huffman_t tree[size];
	HuffmanCodes(str_consol,count_consol,size,tree);
	
	
	printf("\nThe Huffman encoded output is shown below;\n\n");
	int k=0;
	for (i=0;i<sizeof(str_raw);i++)
	{
		
		
		for (j=0;j<size;j++)
		{			
			if(str_raw[i] == (char)tree[j].data)
			{
				if(str_raw[i] == (char)tree[j].data) 
				{	for(k=0;k<tree[j].size;k++)
					printf("%d",/*tree[j].data*/tree[j].Huffman_Code[k]);
				}
			}
		}
	}

	printf("\nData compression is complete.\n");
	/*return 0; */
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeapNode_t* newNode(char data, unsigned freq) 
{ 
		 MinHeapNode_t* temp 
		= (MinHeapNode_t*)malloc
(sizeof(MinHeapNode_t)); 

	temp->left = temp->right = NULL; 
	temp->data = data; 
	temp->freq = freq; 

	return temp; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

struct MinHeap* createMinHeap(unsigned capacity) 

{ 

	MinHeap_t* minHeap 
		= (MinHeap_t*)malloc(sizeof(MinHeap_t)); 

	/*  current size is 0  */
	minHeap->size = 0; 

	minHeap->capacity = capacity; 

	minHeap->array 
		= (MinHeapNode_t**)malloc(minHeap-> 
capacity * sizeof(MinHeapNode_t*)); 
	return minHeap; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void swapMinHeapNode(MinHeapNode_t** a, MinHeapNode_t** b) 

{ 

	MinHeapNode_t* t = *a; 
	*a = *b; 
	*b = t; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void minHeapify(struct MinHeap* minHeap, int idx) 

{ 

	int smallest = idx; 
	int left = 2 * idx + 1; 
	int right = 2 * idx + 2; 

	if (left < minHeap->size && minHeap->array[left]-> 
freq < minHeap->array[smallest]->freq) 
		smallest = left; 

	if (right < minHeap->size && minHeap->array[right]-> 
freq < minHeap->array[smallest]->freq) 
		smallest = right; 

	if (smallest != idx) { 
		swapMinHeapNode(&minHeap->array[smallest], 
						&minHeap->array[idx]); 
		minHeapify(minHeap, smallest); 
	} 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

int isSizeOne(struct MinHeap* minHeap) 
{ 

	return (minHeap->size == 1); 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeapNode_t* extractMin(MinHeap_t* minHeap) 

{ 

	MinHeapNode_t* temp = minHeap->array[0]; 
	minHeap->array[0] 
		= minHeap->array[minHeap->size - 1]; 

	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return temp; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void insertMinHeap(MinHeap_t* minHeap, MinHeapNode_t* MinHeapNode_t) 

{ 

	++minHeap->size; 
	int i = minHeap->size - 1; 

	while (i && MinHeapNode_t->freq < minHeap->array[(i - 1) / 2]->freq) { 

		minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
		i = (i - 1) / 2; 
	} 

	minHeap->array[i] = MinHeapNode_t; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void buildMinHeap(struct MinHeap* minHeap) 

{ 

	int n = minHeap->size - 1; 
	int i; 

	for (i = (n - 1) / 2; i >= 0; --i) 
		minHeapify(minHeap, i); 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

Huffman_t printArr(int str[], int n, int freq,char data) 
{ 
	/*#ifdef DEBUG*/
	int i=0; 
	int j=0;
	Huffman_t code1;

	for (i = 0; i < n; ++i)  {
		Huffman_Code[j] = str[i];
		j++;
		}

	 code1=write_Huff_array(Huffman_Code,n,data,freq);

	return code1;
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

Huffman_t write_Huff_array(int code[], int n, int ASCII, int freq) 
{ 
	
	Huffman_t code1;
	/*int character_count=0;*/

	int i=0;
	int j=0;

	for (i = 0; i < n; i++)  {
		code1.Huffman_Code[i] = code[j];
		j++;
		}

	code1.size = n;
	
	code1.data = ASCII;
	
	return code1;

}



/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

int isLeaf(MinHeapNode_t* root) 

{ 

	return !(root->left) && !(root->right); 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeap_t* createAndBuildMinHeap(char data[], int freq[], int size) 

{ 

	MinHeap_t* minHeap = createMinHeap(size); 

    int i;
	for (i = 0; i < size; ++i) 
		minHeap->array[i] = newNode(data[i], freq[i]); 

	minHeap->size = size; 
	buildMinHeap(minHeap); 

	return minHeap; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - a MinHeapNode struct
*******************************************************************************/

MinHeapNode_t* buildHuffmanTree(char data[], int freq[], int size)  

{ 
	MinHeapNode_t *left, *right, *top; 

/* 	 Step 1: Create a min heap of capacity equal to size. Initially, 
			 there are modes equal to size.  */
	MinHeap_t* minHeap = createAndBuildMinHeap(data, freq, size); 
	

/* 	 Iterate while size of heap doesn't become 1  */
	while (!isSizeOne(minHeap)) { 
		
/* 		 Step 2: Extract the two minimum freq items from min heap  */
		left = extractMin(minHeap);
		right = extractMin(minHeap); 
/* 
		 Step 3: Create a new internal node with frequency equal to the 
		 sum of the two nodes frequencies. 
		 Make the two extracted node as left and right children of this new 
		 node. Add this node to the min heap,
		 '$' is a special value for internal nodes, not used  */
		top = newNode('$', left->freq + right->freq); 

		top->left = left; 
		top->right = right; 

		insertMinHeap(minHeap, top); 
	} 

/* 	 Step 4: The remaining node is the root node and the tree is complete.  */
	return extractMin(minHeap); 
}

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - void
*******************************************************************************/

/***Huffman_t printCodes(MinHeapNode_t* root, int str[], int top, int size) ***/
void printCodes(MinHeapNode_t* root, int str[], int top, int size, Huffman_t tree[], int* row) 
{ 
	Huffman_t code1;


/* 	 Assign 0 to left edge and recur  */
	if (root->left) { 

		str[top] = 0; 
		/***printCodes(root->left, str, top + 1,size); ***/
		printCodes(root->left, str, top + 1,size, tree, row); 
	} 

/* 	 Assign 1 to right edge and recur  */
	if (root->right) { 

		str[top] = 1; 
		/***printCodes(root->right, str, top + 1,size); ***/
		printCodes(root->right, str, top + 1,size, tree, row); 
	} 

/* 	 If this is a leaf node, then it contains one of the input 
	 characters, print the character and its code from str[]  */
	if (isLeaf(root)) { 

		code1=printArr(str, top,root->freq,root->data);
		tree[*row] = code1;
		*row = *row + 1;
			
	
	}
	
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - void
*******************************************************************************/

/***void HuffmanCodes(char data[], int count[], int size)***/
void HuffmanCodes(char data[], int count[], int size, Huffman_t tree[])
{ 
	/*printf("Size is = %d\n",size);*/

	/* 	 Construct Huffman Tree  */
	MinHeapNode_t* root = buildHuffmanTree(data, count, size); 
		/*printf("frequency is %d\ndata is: %c\n",root->freq,root->data);
		printf("freq step 1 ok\n");*/

		

	/* 	 Print Huffman codes using the Huffman tree built above  */
	int str[MAX_TREE_HT], top = 0; 

	/***printCodes(root, str, top, size); ***/
	int row = 0;
	printCodes(root, str, top, size, tree, &row);
	
} 
