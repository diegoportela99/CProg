#include <stdio.h>
#include "debug.h"

/* Used to pad titles in debug mode */
#define TITLE "//////////"

/* Used to pad section titles in debug mode */
#define SECTIONTITLE "<><><><><>"

/* Very high level debug. Only Michael should be calling, when changing menu
section */
void print_section_title(char* string) {
    printf(TEXTBLUE(1));
    printf("\n"SECTIONTITLE"%s"SECTIONTITLE"\n",string);
    printf(TEXTDEFAULT);
}

/* Print in debug mode when the primary functions in a section are called.
E.g., in crypto.c is called when encrypt() and decrypt() are called */
void print_title(char* string, int argc) {
    if (argc <= 1) {
		printf(TEXTRED(0));
	}
    printf("\n"TITLE"%s"TITLE"\n",string);
    if (argc <= 1) {
		printf(TEXTDEFAULT);
	}
}

/* Call within debug as a header for content within the debug mode.
E.g., If you're printing an array of values in debug, use this as the header */
void print_header(char* string) {
    printf(TEXTRED(0));
    printf("%s",string);
    printf(TEXTDEFAULT);
}