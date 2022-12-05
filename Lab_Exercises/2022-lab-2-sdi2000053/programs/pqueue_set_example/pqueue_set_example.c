#include <stdlib.h>
#include <stdio.h>

#include "ADTPriorityQueue.h"
#include "ADTSet.h"
#include "common_types.h"

int compare_ints(Pointer a, Pointer b) {
    return *(int*)a - *(int*)b;
}

int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		
	*pointer = value;						
	return pointer;
}

int main() {

    PriorityQueue pq = pqueue_create(compare_ints,NULL,NULL); //Δημιουργησε set και priority_queue
    Set set = set_create(compare_ints,free);


    for(int i = 0; i < 20; i++) { //προσθεσε στο pq και set 20 τυχαιους αριθμους rand
        int y = rand();
        int x = rand();
        pqueue_insert(pq,create_int(y));
        set_insert(set,create_int(x));
    }

    printf("PriorityQueue:\n");

    while(pqueue_size(pq) != 0) { //οσο υπαρχει λιστα εκτυπωσε το value της και μετα διεγραψε το μεγιστο στοιχειο(για τη φθινουσα διαταξη) μεχρι να αδειασει
        int* el = pqueue_max(pq);
        printf("%d\n",*el);
        pqueue_remove_max(pq);
    }
    printf("\nSet:\n");

    for(SetNode node = set_first(set); node != SET_EOF; node = set_next(set,node)) { // απο το πρωτο node του set μεχρι να βρεις αδειο set με βημα το επομενο node κανε print το set 
        int* el2 = set_node_value(set,node);
        printf("%d\n",*el2);
    }

    pqueue_destroy(pq);
    set_destroy(set);
}
