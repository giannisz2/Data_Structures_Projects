#include <stdlib.h>
#include <stdio.h>

#include "pair_sum.h"
#include "ADTMap.h"

int compare_ints(Pointer a, Pointer b) {
    return *(int*)a - *(int*)b;
}

Pair pair_sum(int target, Vector numbers) {
	VectorNode node1;
	VectorNode node2;
	Pair p;
	for(node1 = vector_first(numbers); node1 != VECTOR_EOF; node1 = vector_next(numbers,node1)) {// loop για καθε στοιχειο
		int* el1 = vector_node_value(numbers,node1);
		for(node2 = vector_first(numbers); node2 != VECTOR_EOF; node2 = vector_next(numbers,node2)) { // loop για συγκριση ολων των στοιχειων με αυτο της προηγουμενης loopas
			int* el2 = vector_node_value(numbers,node2);
			if((*el1 + *el2) == target) {
				p = malloc(sizeof(Pair));
				p->first = *el1;
				p->second = *el2;
				printf("%d + %d = %d\n", p->first,p->second,target);
				if(p->first == 99)
					return p;
			}
		}
	}
	Map map = map_create(compare_ints,NULL,NULL);
	for(int i = 0; i < vector_size(numbers);i++){
		Pointer q = vector_get_at(numbers,i);
		map_insert(map,q,q);
	}
	for(node1 = vector_first(numbers); node1 != VECTOR_EOF; node1 = vector_next(numbers,node1)) {

		int v = target - *(int*)vector_node_value(numbers,node1);
		if(map_find(map,&v) && target != 0){
			p = malloc(sizeof(Pair));
			p->first = v;
			p->second = *(int*) vector_node_value(numbers,node1);
			printf("%d + %d = %d", p->first, p->second, target);
			return p;
		}

		
	}
	return NULL;
}
