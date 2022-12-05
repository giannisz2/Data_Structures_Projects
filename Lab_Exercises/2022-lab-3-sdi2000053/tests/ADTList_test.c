//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT List.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTList.h"


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα με NULL δείκτη συνάρτησης delete_value
	List list = list_create(NULL);
	list_set_destroy_value(list, NULL);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_CHECK(list != NULL);
	TEST_CHECK(list_size(list) == 0);

	list_destroy(list);
}


void test_insert(void) {

	List list = list_create(NULL);
	
	// Θα προσθέτουμε, μέσω της insert, δείκτες ως προς τα στοιχεία του π΄ίνακα
	int N = 1000;
	int array[N];					


	for (int i = 0; i < N; i++) {

		// LIST_BOF για εισαγωγή στην αρχή
		list_insert_next(list, LIST_BOF, &array[i]);
		
		// Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
		TEST_CHECK(list_size(list) == (i + 1));	

		// Ελέγχουμε εάν ο πρώτος κόμβος περιέχει σαν τιμή τον δείκτη που μόλις κάναμε insert								
		TEST_CHECK(list_node_value(list, list_first(list)) == &array[i]);	
	}

	// Ελέγχουμε εάν τα στοιχεία έχουν μπει με την αντίστροφη σειρά
	ListNode node = list_first(list);

	for (int i = N - 1; i >= 0; i--) {
		TEST_CHECK(list_node_value(list, node) == &array[i]);
		node = list_next(list, node);
	}

	// Εισαγωγή σε ενδιάμεσο κόμβο: προσθέτουμε το NULL σαν δεύτερο κόμβο
	ListNode first_node = list_first(list);
	list_insert_next(list, first_node, NULL);
	TEST_CHECK(list_node_value(list, list_next(list, first_node)) == NULL);

	list_destroy(list);
}

void test_remove_next(void) {

	// Δημιουργία λίστας που καλεί αυτόματα τη free σε κάθε στοιχείο που αφαιρείται
	List list = list_create(free);

	int N = 1000;
	int* array[N];

	// Χρησιμοποιούμε την insert για να γεμίσουμε την λίστα, αφού την έχουμε δοκιμάσει ήδη στην test_insert()
	for (int i = 0; i < N; i++) {
		
		// Δημιουργούμε δυναμικά δεσμευμένα αντικείμενα για να δοκιμάσουμε την destroy_function
		array[i]  = malloc(sizeof(int));
		*array[i] = i;
		list_insert_next(list, LIST_BOF, array[i]);
	}


	for (int i = N - 1; i >= 0; i--) {
		// Διαγράφουμε απο την αρχή και ελέγχουμε εάν η τιμή του πρώτου κόμβου 
		// ήταν η ίδια με αυτή που κάναμε insert παραπάνω
		TEST_CHECK(list_node_value(list, list_first(list)) == array[i]);
		list_remove_next(list, LIST_BOF);

		// Ελέγχουμε ότι ενημερώνεται (μειώνεται) το size/μέγεθος της λίστας
		TEST_CHECK(list_size(list) == i);
	}

	// Ξαναγεμίζουμε την λίστα για να δοκιμάσουμε την διαγραφή απο ενδιάμεσο κόμβο
	for (int i = 0; i < N; i++) {
		array[i]  = malloc(sizeof(int));
		*array[i] = i;
		list_insert_next(list, LIST_BOF, array[i]);
	}	

	// Δοκιμάζουμε την διαγραφή κόμβων ενδιάμεσα της λίστας, και συγκεκριμένα του δεύτερου κόμβου απο την αρχή
	list_remove_next(list, list_first(list));
	TEST_CHECK(list_size(list) == N - 1);

	list_destroy(list);
}


// Σύγκριση δύο int pointers
int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}


void test_find() {
	List list = list_create(NULL);
	int N = 1000;
	int array[N];

	// Εισάγουμε δοκιμαστικές τιμές στον πίνακα , για να ελέγξουμε την test_find
	for (int i = 0; i < N; i++) {
		array[i] = i;
		list_insert_next(list, LIST_BOF, &array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for (int i = 0; i < N; i++) {
		int* value = list_find(list, &i, compare_ints); 
		TEST_CHECK(value == &array[i]);
	}

	// Δοκιμάζουμε, για μια τυχαία τιμή που δεν μπορεί πιθανώς να υπάρχει στην λίστα,
	// αν η list_find γυρνάει σωστά NULL pointer
	int not_exists = -1;
	TEST_CHECK(list_find(list, &not_exists, compare_ints) == NULL);

	list_destroy(list);
}


void test_find_node() {

	List list = list_create(NULL);

	int N = 1000;
	int array[N];

	// Εισαγωγή τιμών στον πίνακα
	for (int i = 0; i < N; i++) {
		array[i] = i;
		list_insert_next(list, LIST_BOF, &array[i]);
	}

	// Ξεκινάμε από την αρχή της λίστας
	ListNode node = list_first(list);

	for (int i = N - 1; i >= 0; i--) {
		// Ελέγχουμε ότι η list_find_node βρίσκει σωστά τον πρώτο κόμβο με value τον δείκτη &array[i].
		// Σε αυτή την λίστα, δοκιμάζουμε ότι ο πρώτος κόμβος περιέχει τον δείκτη &array[N - 1], 
		// o δεύτερος τον &array[998] κοκ
		ListNode found_node = list_find_node(list, &i, compare_ints); 
		TEST_CHECK(found_node == node);
		TEST_CHECK(list_node_value(list, found_node) == &array[i]);

		// Προχωράμε στον επόμενο κόμβο για να προσπελάσουμε όλη την λίστα
		node = list_next(list, node);
	}

	list_destroy(list);
}

//// Επιπλέον συναρτήσεις προς υλοποίηση στο Εργαστήριο 3

void test_get_at() {
	List list = list_create(NULL);
	int N = 1000;
	int array[N];

	// Εισάγουμε δοκιμαστικές τιμές στον πίνακα , για να ελέγξουμε την test_find
	for (int i = 0; i < N; i++) {
		array[i] = i;
		list_insert_next(list, LIST_BOF, &array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for (int i = 0; i < N; i++) {
		int* value = list_get_at(list, i);
		TEST_ASSERT(value == &array[N-1 - i]);		// τα στοιχεία έχουν μπει με ανάποδη σειρά!
	}

	list_destroy(list);
}

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

// Δημιουργεί μια λίστα με N στοιχεία. Αν array != NULL τότε εισάγονται τα στοιχεία του array, χωρίς destroy handler.
// Αν array == NULL τότε εισάγονται οι αριθμοί 0..Ν-1 σε νέα μνήμη, με free σαν destroy handler.
List create_test_list(int N, int array[]) {
	List list = list_create(array == NULL ? free : NULL);
	ListNode node = LIST_BOF;

	for (int i = 0; i < N; i++) {
		int* value = array == NULL ? create_int(i) : &array[i];
		list_insert_next(list, node, value);
		node = node == LIST_BOF ? list_first(list) : list_next(list, node);
	}

	return list;
}

void test_remove() {
	int N = 1000;
	List list = create_test_list(N, NULL);

	// Διαγραφή του δεύτερου στοιχείου
	ListNode first = list_first(list);
	list_remove(list, list_next(list, first));	
	TEST_ASSERT(list_size(list) == N - 1);
	TEST_ASSERT(*(int*)list_node_value(list, list_next(list, first)) == 2);

	// Διαγραφή του πρώτου
	list_remove(list, list_first(list));	
	TEST_ASSERT(list_size(list) == N - 2);
	TEST_ASSERT(*(int*)list_node_value(list, list_first(list)) == 2);

	// Διαδοχική διαγραφή του τελευταίου στοιχείου μέχρι να αδειάσει η λίστα
	for (int i = 0; i <= N - 3; i++) {
		TEST_ASSERT(*(int*)list_node_value(list, list_last(list)) == N-1 - i);
		list_remove(list, list_last(list));	
		TEST_ASSERT(list_size(list) == N - 3 - i);
		break;
	}

	list_destroy(list);
}

void test_append() {
	int N = 10;
	int array[2*N];	// 2*N στοιχεία!

	// append σε κενή λίστα
	List list = create_test_list(0, array);
	List to_append = create_test_list(N, array);

	list_append(list, to_append);
	TEST_ASSERT(list_size(list) == N);
	for (int i = 0; i < N; i++)
		TEST_ASSERT(list_get_at(list, i) == &array[i]);

	// append κενής λίστας
	to_append = create_test_list(0, array);

	list_append(list, to_append);
	TEST_ASSERT(list_size(list) == N);
	for (int i = 0; i < N; i++)
		TEST_ASSERT(list_get_at(list, i) == &array[i]);

	// append γεμάτης λίστας
	to_append = create_test_list(N, &array[N]);	// το δεύτερο μισό του πίνακα!

	list_append(list, to_append);
	TEST_ASSERT(list_size(list) == 2*N);
	for (int i = 0; i < 2*N; i++)
		TEST_ASSERT(list_get_at(list, i) == &array[i]);

	// destroy _μόνο_ τη list, η to_append καταστρέφεται από την append
	list_destroy(list);
}



// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "list_create", test_create },
	{ "list_insert_next", test_insert },
	{ "list_remove_next", test_remove_next },
	{ "list_find", test_find },
	{ "list_find_node", test_find_node },

	{ "list_get_at", test_get_at },
	{ "list_remove", test_remove },
	{ "list_append", test_append },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};