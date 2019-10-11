/*******************************************************************************
 * DATA STRUCTURE HEADER FILE
 * author: Owen Dowley
 * student id: 13234505
 * description: This header file provides our data structure - a binary tree of
 * telemetry points, and basic methods for interacting with it.
 * 
 * TO USE: 
 * 1) Include in a file: #include "btree.h"
 * 2) Create root: btree_t* root_p = create_b_tree(NULL); 
 * (ONLY IN TOP LEVEL, root NEEDS TO BE PASSED DOWN TO FUNCTIONS NEEDING DATA)
 * 
 * Think of the "index" as you would an array index, and this as effectively 
 * just an array that works faster.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* For malloc, atoi */
#include "debug.h"
#include "btree.h" /* for btree_t, telemetry_point_t */

/* FUNCTION PROTOTYPES */

void delete_telemetry_point(telemetry_point_t* tp_p);

/* FUNCTION DEFINITIONS */

/*******************************************************************************
 * Creates the root of the datastructure
 * inputs:
 * - none
 * outputs:
 * -root_t* | A pointer to the root created
*******************************************************************************/
root_t* create_root() {
    root_t* new_root_p = (root_t*)malloc(sizeof(root_t));
    (*new_root_p).root = create_b_tree(NULL);
    (*new_root_p).number_of_entries = 0;
    return(new_root_p);
}

/*******************************************************************************
 * Creates a btree given a parent
 * inputs:
 * - parent | A pointer to the parent to create a btree node off
 * outputs:
 * -btree_t* | A pointer to the btree node created
*******************************************************************************/
btree_t* create_b_tree(btree_t* parent){
    btree_t* new_b_tree = (btree_t*)malloc(sizeof(btree_t));
    (*new_b_tree).data_p = NULL;
    (*new_b_tree).l = NULL;
    (*new_b_tree).r = NULL;
    (*new_b_tree).p = parent;
    return(new_b_tree);
} 

/*******************************************************************************
 * Creates a telemetry point given data. Effectively stores one row of the
 * imported csv
 * inputs:
 * - char arrays | Each char array is a single string from an entry in the csv
 * outputs:
 * - telemetry_point_t* | A pointer to the telemetry point created
*******************************************************************************/
telemetry_point_t* create_telemetry_point(
	char loc[],char des[],char pla[],char net[],char qua[],char pro[],
    char num[],char add[],char mod[],char fai[],char onl[],char fau[],
    char oos[]) {
    
    /* Create a telemetry point using malloc */
    telemetry_point_t* t_p_p = (telemetry_point_t*)malloc(
        sizeof(telemetry_point_t));

    (*t_p_p).location = (char*)malloc(sizeof(char)*(strlen(loc)+1)); 
    strcpy((*t_p_p).location,loc);
    (*t_p_p).desig = (char*)malloc(sizeof(char)*(strlen(des)+1)); 
    strcpy((*t_p_p).desig,des);
    (*t_p_p).plant = (char*)malloc(sizeof(char)*(strlen(pla)+1));
    strcpy((*t_p_p).plant,pla); 
    (*t_p_p).network = (char*)malloc(sizeof(char)*(strlen(net)+1)); 
    strcpy((*t_p_p).network,net); 
    (*t_p_p).quantity = (char*)malloc(sizeof(char)*(strlen(qua)+1)); 
    strcpy((*t_p_p).quantity,qua); 
    (*t_p_p).protocol = (char*)malloc(sizeof(char)*(strlen(pro)+1)); 
    strcpy((*t_p_p).protocol,pro); 
    (*t_p_p).number = (char*)malloc(sizeof(char)*(strlen(num)+1)); 
    strcpy((*t_p_p).number,num); 
    (*t_p_p).address = (char*)malloc(sizeof(char)*(strlen(add)+1)); 
    strcpy((*t_p_p).address,add); 
    (*t_p_p).moduletype = (char*)malloc(sizeof(char)*(strlen(mod)+1)); 
    strcpy((*t_p_p).moduletype,mod); 
    (*t_p_p).failed = (char*)malloc(sizeof(char)*(strlen(fai)+1)); 
    strcpy((*t_p_p).failed,fai); 
    (*t_p_p).online = (char*)malloc(sizeof(char)*(strlen(onl)+1)); 
    strcpy((*t_p_p).online,onl); 
    (*t_p_p).faulty = (char*)malloc(sizeof(char)*(strlen(fau)+1)); 
    strcpy((*t_p_p).faulty,fau); 
    (*t_p_p).oos = (char*)malloc(sizeof(char)*(strlen(oos)+1)); 
    strcpy((*t_p_p).oos,oos); 

return(t_p_p);
}

/*******************************************************************************
 * Deletes a telemetry point, freeing all parts of it.
 * inputs:
 * - tp_p | A pointer to a telemetry point
 * outputs:
 * - none
*******************************************************************************/
void delete_telemetry_point(telemetry_point_t* tp_p) {
    if(tp_p!=NULL) {
        free(tp_p->desig);
        free(tp_p->failed);
        free(tp_p->faulty);
        free(tp_p->location);
        free(tp_p->moduletype);
        free(tp_p->network);
        free(tp_p->number);
        free(tp_p->online);
        free(tp_p->oos);
        free(tp_p->plant);
        free(tp_p->protocol);
        free(tp_p->quantity);
        free(tp_p);
    }
}

/*******************************************************************************
 * Gets the telemetry point at an index in the datastructure.
 * inputs:
 * - index | The index to retrieve within the datastructure
 * - root | A pointer to the root of the datastructure
 * outputs:
 * - telemetry_point_t* | A pointer to a located telemetry point
*******************************************************************************/
telemetry_point_t* get_telemetry_point(unsigned int index, root_t* root) {
    btree_t* current_node_p = (*root).root;
    #ifdef DETAILEDDEBUG
        printf("Index: %d\n",index);
    #endif
    while(index) {
        current_node_p = index&1 ? (*current_node_p).r : (*current_node_p).l;
        index = index >> 1;
    }
    return((*current_node_p).data_p);
}

/*******************************************************************************
 * Adds a telemetry point to the datastructure at the requested index.
 * inputs:
 * - index | The index to store the telemetry point at
 * - root | A pointer to the root of the datastructure
 * - telemetry_point_t* | A pointer to the telemetry point to be stored
 * outputs:
 * - none
*******************************************************************************/
void add_telemetry_point(unsigned int index, root_t* root, 
    telemetry_point_t* telemetry_point_p) {
        btree_t* current_node_p = (*root).root; /* Initially the top node */
        btree_t* next_node_p; /* The next node to change to */

    /* Iterate through the btree until the current node is the one corresponding
    to the index provided. */
    while(index) {
        next_node_p = index&1 ? (*current_node_p).r : (*current_node_p).l;
        if (next_node_p == NULL) {
            if(index&1){
                (*current_node_p).r = create_b_tree(current_node_p);
                next_node_p = (*current_node_p).r;
            }
            if(!(index&1)) {
                (*current_node_p).l = create_b_tree(current_node_p);
                next_node_p = (*current_node_p).l;
            }
        }
        current_node_p = next_node_p;
        index = index >> 1;
    }
    (*root).number_of_entries++;

    /* If the located index has no data points, add one. If not, return an 
    error message. */
    if((*current_node_p).data_p == NULL ) {
        (*current_node_p).data_p = telemetry_point_p;
    } else {
        printf("Attempt to overwrite datapoint\n");
    }
}

/*******************************************************************************
 * Deletes the entire datastructure except the root, freeing every item.
 * inputs:
 * - root | A pointer to the root of the datastructure
 * outputs:
 * - none
*******************************************************************************/
void delete_datastructure(root_t* root) {
    printf("Deleting datastructure\n");
    btree_t* current_node_p = (*root).root; /* Initially the top node */
    btree_t* next_node_p; /* The next node to change to */
    int exit = 0; /* Set upon reaching the node with no branches left */

    while (!exit) {
        /* If there is a left branch take it */
        if((*current_node_p).l!=NULL) {
            #ifdef DEBUG
                printf("Branching left\n");
            #endif
            current_node_p = (*current_node_p).l;
            
        }
        /* If there are no left branches but are right branches, take them */
        else if((*current_node_p).r!=NULL) {
            #ifdef DEBUG
                printf("Branching right\n");
            #endif
            current_node_p = (*current_node_p).r;
            
        }
        /* If there are no branches, this is a leaf. Remove it. */
        else {
            #ifdef DEBUG
                printf("Deleting node\n");
            #endif
            /* If there's a telemetry point to free, free it. */
            if((*current_node_p).data_p!=NULL) {
                #ifdef DEBUG
                    printf("Deleting telemetry point\n");
                #endif
                delete_telemetry_point((*current_node_p).data_p);
            }
            /* If this is the top node, free it and exit */
            if(current_node_p == (*root).root) {
                #ifdef DEBUG
                    printf("Freeing root, all branches handled.\n");
                #endif
                free(current_node_p);
                exit = 1;
            }
            /* If this isn't the top node, free it and move up */
            if(!exit) {
                next_node_p = (*current_node_p).p;
                if((*(*current_node_p).p).r == current_node_p) {
                    (*(*current_node_p).p).r = NULL;
                }
                else {
                    (*(*current_node_p).p).l = NULL;
                }
                free(current_node_p);
                #ifdef DEBUG
                    printf("Accending tree\n");
                #endif
                current_node_p = next_node_p;
            }
        }
    }
}
