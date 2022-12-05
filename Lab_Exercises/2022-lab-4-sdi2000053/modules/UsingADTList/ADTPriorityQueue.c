///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω ταξινομημένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTList.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί List


// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	List list;	// η λίστα στην οποία αποθηκεύουμε τα στοιχεία
	CompareFunc compare_values;
	DestroyFunc destroy_value;
};



// Βοηθητικές συναρτήσεις //////////////////////////////////////////////////////////////////

// Αρχικοποιεί την ταξινομημένη λίστα από τα στοιχεία του vector values.

static void naive_initialize_list(PriorityQueue pqueue, Vector values) {
	// Απλά κάνουμε insert τα στοιχεία ένα ένα (όπως η naive_heapify).
	// Προαιρετικά: sort τα στοιχεία και εισαγωγή όλα μαζί (πιο αποδοτικό).
		
	//INSERTION SORT	
	int i, j;
	Pointer key = malloc(sizeof(Pointer));

	for(i = 1; i < vector_size(values); i++) {
		key = vector_get_at(values, i);
		j = i - 1;

		while(j >= 0 && pqueue->compare_values(vector_get_at(values, j), key) > 0) {
			vector_set_at(values, j + 1, vector_get_at(values, j));
			j = j - 1;
		}

		vector_set_at(values, j + 1, key);
	}
		
	//INSERT ALL VALUES IN LIST
	for(int i = 0; i < vector_size(values); i++) {
		list_insert_next(pqueue->list, list_last(pqueue->list), vector_get_at(values, i));
	}

}


// Συναρτήσεις του ADTPriorityQueue ////////////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	PriorityQueue pqueue = malloc(sizeof(*pqueue));

	// Για την κλήση της συνάρτησης destroy_value, την περνάμε απλά στη list_create
	// οπότε θα καλείται αυτόματα όταν το στοιχείο αφαιρεθεί από τη λίστα.
	pqueue->compare_values = compare;
	pqueue->destroy_value = destroy_value;

	pqueue->list = list_create(destroy_value);

	// Αν values != NULL, αρχικοποιούμε την ταξινομημένη λίστα.
	if (values != NULL)
		naive_initialize_list(pqueue, values);

	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	return list_size(pqueue->list);
}

Pointer pqueue_max(PriorityQueue pqueue) {
	return list_node_value(pqueue->list, list_last(pqueue->list));
}

void pqueue_insert(PriorityQueue pqueue, Pointer value) {
	list_insert_next(pqueue->list,list_last(pqueue->list),value);

	Vector vec = vector_create(list_size(pqueue->list), pqueue->destroy_value);

	for(ListNode n = list_first(pqueue->list); n != list_last(pqueue->list); n = list_next(pqueue->list,n)) {
		vector_insert_last(vec, list_node_value(pqueue->list, n));
	}

	//BUBBLESORT 2
	for(int i = 0; i < vector_size(vec) - 1; i++) 
		for(int j = 0; j < vector_size(vec) - i - 1; j++) {
			if(vector_get_at(vec,j) == NULL && vector_get_at(vec, j+1) == NULL) 
				break;
			if(pqueue->compare_values(vector_get_at(vec, j), vector_get_at(vec, j+1)) > 0) {
				Pointer saved = vector_get_at(vec,j);
				vector_set_at(vec, j, vector_get_at(vec, j+1));
				vector_set_at(vec, j+1, saved);
			}
		}

	
	// Η βασικη ιδεα μετα ειναι να διαγραψω ολα τα στοιχεια της λιστας και να τα ξαναπροσθεσω + το καινουριο στοιχειο επισης ταξινομημενο
	// αλλα τρωω errors

}

void pqueue_remove_max(PriorityQueue pqueue) {
	//TRAVERE WHOLE LIST AND WHEN YOU FIND NODE BEFORE LAST REMOVE THE NEXT(LAST)
	ListNode m = list_find_node(pqueue->list, pqueue_max(pqueue), pqueue->compare_values);
	for(ListNode n = list_first(pqueue->list); n != LIST_EOF; n = list_next(pqueue->list,n)) {
		if(list_next(pqueue->list,n) == m) {
			list_remove_next(pqueue->list,n);
		}
	}
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	// Απλά καλούμε την αντίστοιχη συνάρτηση της λίστας
	return list_set_destroy_value(pqueue->list, destroy_value);
}

void pqueue_destroy(PriorityQueue pqueue) {
	// DESTROY LIST AND PQUEUE
	list_destroy(pqueue->list);
	free(pqueue);
}
