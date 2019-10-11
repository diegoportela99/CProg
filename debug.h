/*******************************************************************************
 * DEBUG.H
 * Name: Owen Dowley
 * Student ID: 13234505
 * Description: This header file contains standardized debug functionality.
*******************************************************************************/
#include <stdio.h> /* Used for printf */

/* If defined, activates debug features */
/* #define DEBUG */

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

void print_section_title(char* string);
void print_title(char* string, int argc);
void print_header(char* string);