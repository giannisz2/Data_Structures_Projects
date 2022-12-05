/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Hash Table με seperate chaining (Fixed_Array & ADTSet)
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMap.h"
#include "ADTSet.h"


int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

#define MAX_LOAD_FACTOR 0.9
#define FIXED_SIZE 3

// container 
typedef struct container {
	Set set;
	MapNode fixed_array;
	int size;
} *Container;

struct map_node{
	Pointer key;		
	Pointer value;
	Map owner; 
};

struct map {
	Container container; // array απο containers 				
	int capacity;				
	int size;					
	CompareFunc compare;		
	HashFunc hash_function;		
	DestroyFunc destroy_key;	
	DestroyFunc destroy_value;
};


static int compare_nodes(MapNode a, MapNode b) {
	return a->owner->compare(a->key, b->key);
}

// Συνάρτηση που καταστρέφει ένα map node
static void destroy_nodes(MapNode node) {
	if (node->owner->destroy_key != NULL)
		node->owner->destroy_key(node->key);

	if (node->owner->destroy_value != NULL)
		node->owner->destroy_value(node->value);

	free(node);
}

Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	
	Map map = malloc(sizeof(*map));
	map->capacity = prime_sizes[0];
	map->container = malloc(map->capacity * sizeof(struct container));
	map->compare = compare;

	// Δημιουργω το container
	for (int i = 0; i < map->capacity; i++) {
		map->container[i].set = set_create((CompareFunc)compare_nodes, (DestroyFunc)destroy_nodes);
		map->container[i].fixed_array = malloc(FIXED_SIZE * sizeof(struct map_node));
		map->container[i].size = 0;
	}
		
	map->size = 0;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}


int map_size(Map map) {
	return map->size;
}


static void rehash(Map map) {
	
	int old_cap = map->capacity;
	// Κραταω container και κανω ιδια διαδικασια με linear probing πανω κατω
	Container old_cont = malloc(map->capacity * sizeof(struct container));

	// Malloc για καθε container
	for(int i = 0; i < old_cap; i++) {
		old_cont[i].fixed_array = map->container[i].fixed_array;
		old_cont[i].set = map->container[i].set;
		old_cont[i].size = map->container[i].size;
	}

	MapNode old_node = malloc(sizeof(struct map_node));

	int prime_no = sizeof(prime_sizes) / sizeof(int);	
	for (int i = 0; i < prime_no; i++) {				
		if (prime_sizes[i] > old_cap) {
			map->capacity = prime_sizes[i]; 
			break;
		}
	}

	if(map->capacity == old_cap) map->capacity *= 2;

	map->container = malloc(map->capacity * sizeof(struct container));
	for (int i = 0; i < map->capacity; i++) {
		map->container[i].fixed_array = malloc(FIXED_SIZE * sizeof(struct map_node));
		map->container[i].set = set_create((CompareFunc)compare_nodes, (DestroyFunc)destroy_nodes);
		map->container[i].size = 0;
	}

	map->size = 0;
	
	int con_size = 0;

	for (int i = 0; i < old_cap; i++) {
		con_size = old_cont[i].size;
		if(con_size < FIXED_SIZE) {

			for(int y = 0; y < con_size; y++) {
				old_node->key = old_cont[i].fixed_array[y].key;
				old_node->value = old_cont[i].fixed_array[y].value;
				map_insert(map, old_node->key, old_node->value);
			}
		}
		else {
			
			for(SetNode n = set_first(map->container[i].set); n != SET_EOF; n = set_next(map->container[i].set, n)) {
				old_node = (MapNode)set_node_value(old_cont[i].set, n);
				map_insert(map, old_node->key, old_node->value);
			}

		}
			
	}

	free(old_node);
	free(old_cont);

}


void map_insert(Map map, Pointer key, Pointer value) {

	MapNode node = malloc(sizeof(struct map_node));
	struct map_node search_node = { .key = key, .owner = map };

	if(key != NULL && value != NULL) {
		node->key = key;
		node->value = value;
	}else
		return;

	
	uint i = 0;

	if(map->hash_function != NULL)
		i = map->hash_function(key) % map->capacity;
	

	int con_size = map->container[i].size;

	// Κοιταω αμα υπαρχει ηδη το key
	MapNode node2 = malloc(sizeof(MapNode));

	if(con_size < FIXED_SIZE) {

		for(int y = 0; y < con_size; y++) {
			if(map->compare(map->container[i].fixed_array[y].key, key) == 0) {
				if(map->destroy_key != NULL)
					map->destroy_key(map->container[i].fixed_array[y].key);
				if(map->destroy_value != NULL)
					map->destroy_value(map->container[i].fixed_array[y].value);
				map->size--;
			}
		}
		
	}else {

		node2 = set_find(map->container[i].set, &search_node);
		if(node2 != NULL)
			set_remove(map->container[i].set, node2);
		map->size--;
		
	}
	free(node2);

	// Αν το size του container στην θεση i ειναι μικροτερο του FIXED_SIZE βαλε το node στο array,
	// αλλιως στο set
	if(con_size < FIXED_SIZE) {
		map->container[i].fixed_array[con_size].key = key;
		map->container[i].fixed_array[con_size].value = value;
		map->container[i].fixed_array[con_size].owner = map;
	}
	else {
		set_insert(map->container[i].set, node);
	}

	// Αυξανω size του container
	map->container[i].size += 1;
	map->size++;

	float load_factor = (float)map->size / map->capacity;
	if (load_factor > MAX_LOAD_FACTOR)
		rehash(map);
}


bool map_remove(Map map, Pointer key) {
	MapNode node = map_find_node(map, key);
	if (node == MAP_EOF)
		return false;

	uint i = map->hash_function(key) % map->capacity;
	int con_size = map->container[i].size;

	if(con_size < FIXED_SIZE) {

		if(map->destroy_key != NULL) 
			map->destroy_key(node->key);
		if(map->destroy_value != NULL)
			map->destroy_value(node->value);
		
		map->container[i].size--;
		map->size--;
		return true;

	}else {

		map->container[i].size--;
		set_remove(map->container[i].set, node);
		map->size--;
		return true;
	}
}



Pointer map_find(Map map, Pointer key) {
	if(map->compare == NULL || key == NULL)
		return NULL;

	uint i = map->hash_function(key) % map->capacity;
	
	struct map_node search_node = { .key = key, .owner = map};

	int con_size = map->container[i].size;
	if(con_size < FIXED_SIZE) {

		for(int y = 0; y < con_size; y++) {
			if(map->compare(map->container[i].fixed_array[y].key, key) == 0)
				if(map->container[i].fixed_array[y].value != NULL)
					return map->container[i].fixed_array[y].value;
		}

	} else {
		// Βρισκω το node σε σχεση με το key
		MapNode node = set_find(map->container[i].set, &search_node);
		if(node != NULL)
			return node->value;
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

void map_destroy(Map map) {

	// Free καθε set και array καθε κομβου
	for (int i = 0; i < map->capacity; i++) {
		free(map->container[i].fixed_array);
		set_destroy(map->container[i].set);
	}

	free(map);
}


MapNode map_first(Map map) {

	MapNode node = malloc(sizeof(MapNode));
	// Βρισκω τον πρωτο κομβο που δεν ειναι null και τον επιστρεφω.
	for (int i = 0; i < map->capacity; i++) {
		
		if(map->container[i].fixed_array[0].key != NULL && map->container[i].fixed_array[0].value != NULL) {
			node->key = map->container[i].fixed_array[0].key;
			node->value = map->container[i].fixed_array[0].value;
			node->owner = map;
			return node;
		}
	}
		
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	if(map->compare == NULL)
		return MAP_EOF;

	struct map_node search_node = { .key = node->key, .owner = map};
	uint pos = map->hash_function(node->key) % map->capacity;
	MapNode node2 = malloc(sizeof(MapNode));

	int flag = 0;
	
	for(int i = pos; i <= map->capacity; i++) {
		int con_size = map->container[i].size;

		// Αν flag == 1 σημαινει οτι δεν υπηρχε τιποτα αλλο στο container που εκανε hash το node->key, αρα αναγκαστικα το επομενο
		// στοιχειο θα ειναι το πρωτο του επομενου container που εχει στοιχειο.
		if(flag == 1) {

			node2->key = map->container[i].fixed_array[0].key;
			node2->value = map->container[i].fixed_array[0].value;
			node2->owner = map->container[i].fixed_array[0].owner;

			if(map->container[i].fixed_array[0].key != NULL && map->container[i].fixed_array[0].value != NULL)
				return node2;

			continue;
		}

		if(con_size < FIXED_SIZE) {

			for(int y = 0; y < con_size; y++) {
				if(map->compare(map->container[i].fixed_array[y].key, node->key) == 0) {
					// Βρηκα το node, τωρα παω για το next
					y++;
					// Αν το επομενο στοιχειο δεν βγαινει απο το fixed array
					if(y < FIXED_SIZE) {
						node2->key = map->container[i].fixed_array[y].key;
						node2->value = map->container[i].fixed_array[y].value;
						node2->owner = map->container[i].fixed_array[y].owner;

						if(map->container[i].fixed_array[y].key != NULL && map->container[i].fixed_array[y].value != NULL)
							return node2;

						continue;

					}else {
						// Αν βγηκαμε απο το fixed array, κοιταω αν υπαρχει set, αν δεν υπαρχει το επομενο στοιχειο ναγκαστικα θα ειναι
						// το πρωτο του επομενου container που δεν ειναι NULL
						if(set_size(map->container[i].set) != 0) {
							node2 = set_find(map->container[i].set, &search_node);
							return node2;

						}else {
							flag = 1;
							continue;
						}
					}
				}
			}
			
		}else {
			
			SetNode find_node = set_find(map->container[i].set, &search_node);
			find_node = set_next(map->container[i].set, find_node);
			MapNode n = (MapNode)set_node_value(map->container[i].set, find_node);
			if(n != NULL)
				return n;
			flag = 1;
		}
	}

	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	if(node != NULL)
		return node->key;
	return NULL;
}

Pointer map_node_value(Map map, MapNode node) {
	if(node != NULL)
		return node->value;
	return NULL;
}

MapNode map_find_node(Map map, Pointer key) {

	if(map->compare == NULL || key == NULL)
		return MAP_EOF;

	MapNode node = malloc(sizeof(struct map_node));
	struct map_node search_node = { .key = key, .owner = map };
	// Βρισκω το node που εχει ιδιο key με το ορισμα και τον επιστρεφω
	uint i = map->hash_function(key) % map->capacity;
	int con_size = map->container[i].size;
	if(con_size < FIXED_SIZE) {
		// Για καθε θεση του fixed_array στο συγκεκριμενο container
		for(int y = 0; y < con_size; y++) {
			if(&map->container[i].fixed_array[y] != NULL) {
				// Αν βρω το key επιστρεφω τον κομβο
				if(map->compare(map->container[i].fixed_array[y].key, key) == 0) {
					node->key = map->container[i].fixed_array[y].key;
					node->value = map->container[i].fixed_array[y].value;
					node->owner = map;
					return node;
				}
			}
		}

	}else  {

		node = set_find(map->container[i].set, &search_node);
		if(node != NULL)
			if(map->compare(node->key, key) == 0) 
				return node;
		
	}

	

 	return MAP_EOF;
}

void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

uint hash_string(Pointer value) {

    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				
}