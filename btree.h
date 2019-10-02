/*******************************************************************************
 * DATA STRUCTURE HEADER FILE
 * author: Owen Dowley
 * student id: 13234505
 * description: This header file provides our data structure - a binary tree of
 * telemetry points, and basic methods for interacting with it.
 * Include in a file using #include "btree.h"
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

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
	int number;
	/*the module address for master station communications*/
	int address;
	/*what type of signal, eg. analog, digital etc*/
	char* moduletype;
	/*has the telemetry failed at the time of CSV save*/
	int failed;
	/*Is the telemetry online when CSV saved*/
	int online;
	/*same as failed*/
	int faulty;
	/* Out of service */
	int oos; 
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

btree_t* create_b_tree(btree_t* parent){
    btree_t* new_b_tree = (btree_t*)malloc(sizeof(btree_t));
    (*new_b_tree).l = NULL;
    (*new_b_tree).r = NULL;
    (*new_b_tree).p = parent;
    return(new_b_tree);
} 

telemetry_point_t* create_telemetry_point(
	char* loc,char* des,char* pla,char* net,char* qua,char* pro,int num,int add,
    char* mod,int fai,int onl,int fau,int oos) {
    
    telemetry_point_t* t_p_p = (telemetry_point_t*)malloc(
        sizeof(telemetry_point_t)); 
    (*t_p_p).location = loc;
    (*t_p_p).desig = des;
    (*t_p_p).plant = pla;
    (*t_p_p).network = net;
    (*t_p_p).quantity = qua;
    (*t_p_p).protocol = pro;
    (*t_p_p).number = num;
    (*t_p_p).address = add;
    (*t_p_p).moduletype = mod;
    (*t_p_p).failed = fai;
    (*t_p_p).online = onl;
    (*t_p_p).faulty = fau;
    (*t_p_p).oos = oos;

return(t_p_p);
}

telemetry_point_t* get_telemetry_point(unsigned int index, btree_t* root) {
    btree_t current_node = *root;
    while(index) {
        current_node = index&1 ? *current_node.r : *current_node.l;
        index = index >> 1;
    }
    return(current_node.data_p);
}

void add_telemetry_point(unsigned int index, btree_t* root, 
    telemetry_point_t* telemetry_point_p) {
        btree_t* current_node_p = root;
        btree_t* next_node_p;
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
    (*current_node_p).data_p = telemetry_point_p;
}

void delete_datastructure(btree_t* root) {
    btree_t* current_node_p = root;
    btree_t* next_node_p;
    int exit = 0;
    while (!exit) {
        if((*current_node_p).l) {
            #ifdef DEBUG
                printf("Branching left\n");
            #endif
            current_node_p = (*current_node_p).l;
            
        }
        else if((*current_node_p).r) {
            #ifdef DEBUG
                printf("Branching right\n");
            #endif
            current_node_p = (*current_node_p).r;
            
        }
        else {
            #ifdef DEBUG
                printf("Deleting node\n");
            #endif
            if(current_node_p == root) {
                #ifdef DEBUG
                    printf("Freeing root, all branches handled.\n");
                #endif
                free(current_node_p);
                exit = 1;
            }
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