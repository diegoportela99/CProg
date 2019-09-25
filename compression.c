/*********************************************************************
*program to compress substation data using Huffman algorithm
*Ray Robinson
**********************************************************************/

#include <stdio.h> 

/*Read each character from the CSV*/



/*check each charaacter for its ASCII code and count each char*/

int main()
{
    char str[16];
    int count[256], i;
    int dup_count = 0;
    
    /*fgetc();*/

    /*printf("Enter a word>\n");
    fgets(str,16,stdin);
	printf("%s",str);*/

    /* set all 255 values to 0  */
    for(i=0;i<256;i++)
    {
        count[i]=0;
    }
    /* inits i to 0, for less than str length it will 
    add a value to the ASCII index of letter found */
    for(i=0;i<strlen(str);i++)
    {
        count[(int)str[i]] ++;
        /*printf("%c %d\n",str[i],count[(int)str[i]]);*/
    }
    /* checks if the value is greater than 1, if so it prints the letter & value   */
    for(i=0;i<strlen(str);i++)
    {
        if(count[(int)str[i]] > 1)
        {
            printf("Duplicate letter: %c, Occurrences: %d\n",str[i],count[(int)str[i]]);
            count[(int)str[i]]  = 0;
            dup_count++;/* how many duplicates i've found */
        }

    }
    /* if no duplicates prints the message */
    if(dup_count == 0) 
    {
        printf("No duplicates found\n");
    }

return 0;
}