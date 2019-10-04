/*******************************************************************************
 * DEBUG.H
 * Name: Owen Dowley
 * Student ID: 13234505
 * Description: This header file contains standardized debug functionality.
*******************************************************************************/

/* Only define this functionality if it isn't already defined */
#ifndef DEBUGHEADER
#define DEBUGHEADER

#include <stdio.h> /* Used for printf */

/* If defined, activates debug features */
#define DEBUG

/* If defined, adds extra debug features
#define DETAILEDDEBUG */

/* If defined adds visualizations of relevant features
#define GRAPHICALDEBUG */

/* This string is printed to set terminal text to default */
#define TEXTDEFAULT "\033[0m"

/* This string is printed to set terminal text to red. x is a boolean: 1 means 
   bold, 0 means normal. */
#define TEXTRED(x) "\033["#x";31m"

/* This string is printed to set terminal text to blue. x is a boolean: 1 means 
   bold, 0 means normal. */
#define TEXTBLUE(x) "\033["#x";34m"

/* Used to pad titles in debug mode */
#define TITLE "//////////"

/* Used to pad section titles in debug mode */
#define SECTIONTITLE "<><><><><>"

/* Very high level debug. Only Michael should be calling, when changing menu
section */
void print_section_title(const char* string) {
    printf(TEXTBLUE(1));
    printf("\n"SECTIONTITLE"%s"SECTIONTITLE"\n",string);
    printf(TEXTDEFAULT);
}

/* Print in debug mode when the primary functions in a section are called.
E.g., in crypto.c is called when encrypt() and decrypt() are called */
void print_title(const char* string) {
    printf(TEXTRED(1));
    printf("\n"TITLE"%s"TITLE"\n",string);
    printf(TEXTDEFAULT);
}

/* Call within debug as a header for content within the debug mode.
E.g., If you're printing an array of values in debug, use this as the header */
void print_header(const char* string) {
    printf(TEXTRED(0));
    printf("%s",string);
    printf(TEXTDEFAULT);
}

/* End the initial if. */
#endif
