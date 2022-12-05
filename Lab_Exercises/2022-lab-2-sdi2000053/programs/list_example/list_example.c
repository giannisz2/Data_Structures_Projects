#include <stdlib.h>
#include <stdio.h>

#include "ADTList.h"

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

int main() {
	List list1 = list_create(NULL);

	// προσθήκη των αριθμών από το 0 έως 9 στη λίστα
	for (int i = 0; i < 10; i++)
		list_insert_next(list1, list_last(list1), create_int(i));

	// εκτύπωση των περιεχομένων της λίστας
	
	for (ListNode node = list_first(list1); node != LIST_EOF; node = list_next(list1,node)) {
		int* i = list_node_value(list1,node);
		printf("%d\n", *i); 
	}
} 