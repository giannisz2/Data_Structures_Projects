#include <stdio.h>
#include <stdlib.h>

#include "ADTSet.h"
#include "set_utils.h"

Pointer set_find_eq_or_greater(Set set, Pointer value) {
    if(set_size(set) > 1) {

        Pointer y = set_find(set,value);
        if(y == NULL) { //Αν δεν βρεις το value βαλτο στο set παρε το επομενο και καντο remove
            set_insert(set,value);
            SetNode z = set_find_node(set,value);
            z = set_next(set,z);
            if(z != SET_EOF) { // Ελεγξε αν υπαρχει επομενο node 
                Pointer x = set_node_value(set,z);
                set_remove(set,value);
                return x;
            }
        }   
        return value;
    }
    return NULL;
}

Pointer set_find_eq_or_smaller(Set set, Pointer value) {
    if(set_size(set) > 1) {

        Pointer y = set_find(set,value);
        if(y == NULL) { //Αν δεν βρεις το value βαλτο στο set παρε το επομενο και καντο remove
            set_insert(set,value);
            SetNode z = set_find_node(set,value);
            z = set_previous(set,z);
            if(z != SET_EOF) { // Ελεγξε αν υπαρχει επομενο node 
                Pointer x = set_node_value(set,z);
                set_remove(set,value);
                return x;
            }
        }   
        return value;
    }
    return NULL;
}

int compare_ints(Pointer a, Pointer b) {
    a = malloc(sizeof(Pointer));
    b = malloc(sizeof(Pointer));
    return *(int*)a - *(int*)b;
}

int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		
	*pointer = value;						
	return pointer;
}

float* create_float(float value) {
	float* pointer = malloc(sizeof(float));		
	*pointer = value;						
	return pointer;
}