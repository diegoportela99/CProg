#include <stdio.h>
#include "debug.h"

/* Used to pad titles in debug mode */
#define TITLE "//////////"

/* Used to pad section titles in debug mode */
#define SECTIONTITLE "<><><><><>"

/* Print in debug mode when the primary functions in a section are called.
E.g., in crypto.c is called when encrypt() and decrypt() are called */
void print_title(char* string, int argc) {
    if (argc <= 1) {
		printf(TEXTRED(1));
    }
    printf("\n"TITLE"%s"TITLE"\n",string);
    if (argc <= 1) {
		printf(TEXTDEFAULT);
    }
    
}
