#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "btree.h"
/* FUNCTION DEFINITIONS */

/*******************************************************************************
 * Creates root.
*******************************************************************************/
root_t* create_root() {
    root_t* new_root_p = (root_t*)malloc(sizeof(root_t));
    (*new_root_p).root = create_b_tree(NULL);
    (*new_root_p).number_of_entries = 0;
    return(new_root_p);
}

/*******************************************************************************
 * Creates b_tree.
*******************************************************************************/
btree_t* create_b_tree(btree_t* parent){
    btree_t* new_b_tree = (btree_t*)malloc(sizeof(btree_t));
    (*new_b_tree).l = NULL;
    (*new_b_tree).r = NULL;
    (*new_b_tree).p = parent;
    return(new_b_tree);
} 

/*******************************************************************************
 * Creates telemetry point.
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

void add_telemetry_point(unsigned int index, root_t* root, 
    telemetry_point_t* telemetry_point_p) {
        btree_t* current_node_p = (*root).root;
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
    (*root).number_of_entries++;
    (*current_node_p).data_p = telemetry_point_p;
}

void delete_datastructure(root_t* root) {
    btree_t* current_node_p = (*root).root;
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
            if((*current_node_p).data_p) {
                free((*current_node_p).data_p);
            }
            if(current_node_p == (*root).root) {
                #ifdef DEBUG
                    printf("Freeing root, all branches handled.\n");
                #endif
                free(current_node_p);
                free(root);
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

/*int main(void) {
    root_t* root_p = create_root();
    telemetry_point_t* tel_point_p = create_telemetry_point("loc","des","pla",
        "net","qua","pro","num","add","mod","fai","onl","fau","oos"
        );
}*/