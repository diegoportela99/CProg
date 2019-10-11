/*******************************************************************************
 * DATA STRUCTURE HEADER FILE
 * author: Owen Dowley
 * student id: 13234505
 * description: This header file provides our datastructure - a binary tree of
 * telemetry points - and prototypes functions for interacting with it.
 * Those functions are defined in btree.c
*******************************************************************************/

#ifndef DATASTRUCTUREHEADER
#define DATASTRUCTUREHEADER

struct telemetry_point {
	/*the location of the substation*/
	char* location; 
	/*A unique code given to each piece of equipment*/
	char* desig;
	/*the type of equipment, eg. Cb = circuit breaker*/
	char* plant;
	/*eg. voltage level 11kV*/
	char* network;
	/*The name of the actual point eg. oil temperature*/
	char* quantity;
	/*Always DNP in this case, comms protocol that talks to master station*/
	char* protocol;
	/*channel number of the module */
	char* number;
	/*the module address for master station communications*/
	char* address;
	/*what type of signal, eg. analog, digital etc*/
	char* moduletype;
	/*has the telemetry failed at the time of CSV save*/
	char* failed;
	/*Is the telemetry online when CSV saved*/
	char* online;
	/*same as failed*/
	char* faulty;
	/* Out of service */
	char* oos; 
}; typedef struct telemetry_point telemetry_point_t;

/* A binary tree implimentation */
struct btree {
    /* branch left if zero */
    struct btree* l;
    /* branch right if not zero */
    struct btree* r;
    /* parent. used in destructor */
    struct btree* p;
    telemetry_point_t* data_p;
}; typedef struct btree btree_t;

/* The root of a btree */
struct root {
    /* The root of the btree */
    btree_t* root;
    /* The number of entries in a btree. Equal to the maximum index */
    int number_of_entries;
}; typedef struct root root_t;

/*FUNCTION PROTOTYPES*/

/* Creates a binary tree root */
root_t* create_root();

/* Creates a binary tree node */
btree_t* create_b_tree(btree_t* parent);

/* A tool for creating telemetry points. It handles memory allocation. */
telemetry_point_t* create_telemetry_point(
	char loc[],char des[],char pla[],char net[],char qua[],char pro[],
    char num[],char add[],char mod[],char fai[],char onl[],char fau[],
    char oos[]);

/* Gets a telemetry point from a given root, by its index. */
telemetry_point_t* get_telemetry_point(unsigned int index, root_t* root);

/* Adds a given telemetry point to the appropriate node. Also adds any other
points needed to get to that node. */
void add_telemetry_point(unsigned int index, root_t* root, 
    telemetry_point_t* telemetry_point_p);

/* Deletes the datastructure. Call when done with the datastructure in memory.*/
void delete_datastructure(root_t* root);

#endif
