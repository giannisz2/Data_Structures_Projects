/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Cuckoo hashing
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMap.h"


int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

#define LOAD_FACTOR 0.5

typedef enum {
	EMPTY, OCCUPIED
} State;

// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map)
struct map_node{
	Pointer value;
	Pointer key;
	State state;
};

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
	MapNode array1;
	MapNode array2;
	int cap;
	int size;
	CompareFunc compare;
	HashFunc hash_func;
	DestroyFunc destroy_value;
	DestroyFunc destroy_key;
};


Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	Map map = malloc(sizeof(*map));
	map->cap = prime_sizes[0];
	map->array1 = malloc(map->cap * sizeof(struct map_node));
	map->array2 = malloc(map->cap * sizeof(struct map_node));

	for(int i = 0; i < map->cap; i++) {
		map->array1[i].state = EMPTY;
		map->array2[i].state = EMPTY;
	}

	map->size = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;
}

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.

static void rehash(Map map)  {
	// Ιδια διαδικασια με τον κωδικα linear probing απλα με το δευτερο array
	int old_cap = map->cap;
	MapNode old_array1 = map->array1;
	MapNode old_array2 = map->array2;

	int prime_no = sizeof(prime_sizes) / sizeof(int);	
	for (int i = 0; i < prime_no; i++) {					
		if (prime_sizes[i] > old_cap) {
			map->cap = prime_sizes[i]; 
			break;
		}
	}

	if (map->cap == old_cap)					
		map->cap *= 2;

	map->array1 = malloc(map->cap * sizeof(struct map_node));
	map->array2 = malloc(map->cap * sizeof(struct map_node));

	for (int i = 0; i < map->cap; i++) {
		map->array1[i].state = EMPTY;
		map->array2[i].state = EMPTY;
	}

	map->size = 0;
	for (int i = 0; i < old_cap; i++) {
		if (old_array1[i].state == OCCUPIED)
			map_insert(map, old_array1[i].key, old_array1[i].value);

		if(old_array2[i].state == OCCUPIED)
			map_insert(map, old_array2[i].key, old_array2[i].value);

	}

	free(old_array1);
	free(old_array2);
		
}

void map_insert(Map map, Pointer key, Pointer value) {
	if(key == NULL || value == NULL)
		return;

	uint pos;
	// Flags για να δουμε σε ποιον απο τους δυο πινακες μπηκε η τιμη
	int f1 = 0;
	int f2 = 0;

	Pointer kept_value = malloc(sizeof(Pointer));
	Pointer kept_key = malloc(sizeof(Pointer));
	Pointer kept_value2 = malloc(sizeof(Pointer));
	Pointer kept_key2 = malloc(sizeof(Pointer));

	// "Flags"
	kept_key2 = key;
	kept_value2 = value;

	// Main loop για το cuckoo hashing
	for(int i = 0; i < map->cap; i++) {

		// Αν απο συμβαση τα kept δεν ειναι ισα με τα ορισματα σημαινει οτι λειτουργει αυτη τη στιγμη το cuckoo hashing
		if(kept_key2 != key && kept_value2 != value) {
			key = kept_key2;
			value = kept_value2;
		}

		// 1o hash
		if(map->hash_func != NULL)
			pos = map->hash_func(key) % map->cap;
		

		if(map->array1[pos].state == EMPTY) {
			map->array1[pos].value = value;
			map->array1[pos].key = key;
			map->size++;
			f1 = 1;
			break;

		} else if(map->array1[pos].state == OCCUPIED) {

			kept_value = map->array1[pos].value;
			kept_key = map->array1[pos].key;
			map->array1[pos].value = value;
			map->array1[pos].key = key;

			// 2o hash
			if(map->hash_func != NULL)
				pos = ((map->hash_func(kept_key) * map->hash_func(kept_key))/ map->cap) % map->cap;

			if(map->array2[pos].state == EMPTY) {
				map->array2[pos].value = kept_value;
				map->array2[pos].key = kept_key;
				
				map->size++;
				f2 = 1;
			} else {
				kept_value2 = map->array2[pos].value;
				kept_key2 = map->array2[pos].key;
				map->array2[pos].value = kept_value;
				map->array2[pos].key = kept_key;
				map->size++;
				f2 = 1;
			}
				
			if(i == map->cap - 1) {
				rehash(map);
				break;
			}
				

		}

	}
	
	if(f1 == 1)
		map->array1[pos].state = OCCUPIED;
	else if(f2 == 1)
		map->array2[pos].state = OCCUPIED;

	float load_factor = (float)map->size / map->cap;
	if (load_factor > LOAD_FACTOR)
		rehash(map);
	
}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, Pointer key) {
	if(key == NULL)
		return false;
		
	int pos = map->hash_func(key) % map->cap;
	if(map->compare != NULL && map->array1[pos].key != NULL && map->array1[pos].value != NULL) {

		if(map->compare(map->array1[pos].key, key) == 0) {

			if(map->destroy_key != NULL)
				map->destroy_key(map->array1[pos].key);
			if(map->destroy_value != NULL)
				map->destroy_value(map->array1[pos].value);

			map->array1[pos].state = EMPTY;
			map->size--;
			return true;
			
		}
		else {
			pos = ((map->hash_func(key) * map->hash_func(key))/ map->cap) % map->cap;
			if(map->array2[pos].key != NULL && map->array2[pos].value != NULL) {

				if(map->compare(map->array2[pos].key, key) == 0) {
					if(map->destroy_key != NULL) 
						map->destroy_key(map->array2[pos].key);
					if(map->destroy_value != NULL)
						map->destroy_value(map->array2[pos].value);
					
					map->array1[pos].state = EMPTY;
					map->size--;
					return true;
					
				}
			}
		}
	}

	return false;
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

Pointer map_find(Map map, Pointer key) {

	uint i = map->hash_func(key) % map->cap;
	if(map->compare != NULL && map->array1[i].key != NULL) {
		if(map->compare(map->array1[i].key, key) == 0) {
			return map->array1[i].value;
		}
	}
	
	i = ((map->hash_func(key) * map->hash_func(key))/ map->cap) % map->cap;
	if(map->compare != NULL && map->array2[i].key != NULL) {
		if(map->compare(map->array2[i].key, key) == 0) {
			return map->array2[i].value;
		}
	}

	return NULL;
}


DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = map->destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = map->destroy_value;
	map->destroy_value = destroy_value;
	return old;
}

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {
	free(map->array1);
	free(map->array2);
	free(map);
}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

MapNode map_first(Map map) {
	MapNode node = malloc(sizeof(MapNode));
	
	for(int i = 0; i < map->cap; i++) {
		if(map->array1[i].state == OCCUPIED) {
			node->key = map->array1[i].key;
			node->value = map->array1[i].value;
			node->state = OCCUPIED;
			return node;
		}

		if(map->array2[i].state == OCCUPIED) {
			node->key = map->array2[i].key;
			node->value = map->array2[i].value;
			node->state = OCCUPIED;
			return node;			
		}
	}

	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	if(map->compare == NULL)
		return MAP_EOF;
	
	MapNode node2 = malloc(sizeof(MapNode));
	uint pos = map->hash_func(node->key) % map->cap;
	for(int i = pos + 1; i < map->cap; i++) {
		if(map->array1[i].key != NULL) {
			node2->key = map->array1[i].key;
			node2->value = map->array1[i].value;
			node2->state = OCCUPIED;
			return node2;
		}

		pos = ((map->hash_func(node->key) * map->hash_func(node->key))/ map->cap) % map->cap;
		if(map->array2[i].key != NULL) {
			node2->key = map->array2[i].key;
			node2->value = map->array2[i].value;
			node2->state = OCCUPIED;
			return node2;
		}

		pos = map->hash_func(node->key) % map->cap;
	}

	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	if(node != MAP_EOF)
		return node->key;
	return NULL;
}

Pointer map_node_value(Map map, MapNode node) {
	if(node != MAP_EOF)
		return node->value;
	return NULL;
}

MapNode map_find_node(Map map, Pointer key) {
	if(key == NULL)
		return MAP_EOF;
		
	MapNode node = malloc(sizeof(MapNode));
	
	if(map->compare != NULL) {
		uint i = map->hash_func(key) % map->cap;
		if(map->array1[i].key != NULL) { 
			
			if(map->compare(map->array1[i].key, key) == 0) {
				node->key = map->array1[i].key;
				node->value = map->array1[i].value;
				node->state = map->array1[i].state;
				return node;
				
			}
		}

		i = ((map->hash_func(key) * map->hash_func(key))/ map->cap) % map->cap;
		if(map->array2[i].key != NULL) {
			if(map->compare(map->array2[i].key, key) == 0) {
				node->key = map->array2[i].key;
				node->value = map->array2[i].value;
				node->state = map->array2[i].state;
				return node;
			}
		}
		
	}
	return MAP_EOF;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_func = func;
}

uint hash_string(Pointer value) {
	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}