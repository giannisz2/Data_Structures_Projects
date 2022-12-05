//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"
#include "ADTSet.h"
#include "set_utils.h"


void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);
	
	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);

	// Έλεγχος για το αν δημιουργείται jet
	TEST_ASSERT(info->jet != NULL);




	// Έλεγχος για το αν δημιουργείται πύραυλος
	struct key_state keys = { false, false, false, false, false, false, false, false };
	info->playing = true;
	info->paused = false;
	keys.space = true;
	state_update(state,&keys);
	TEST_ASSERT(info->missile != NULL);
	
	// Έλεγχος για types
	TEST_CHECK(info->jet->type == JET);
	TEST_CHECK(info->missile->type == MISSILE);

	// Έλεγχος για rects
	TEST_CHECK(info->jet->rect.x == (SCREEN_WIDTH - 35)/2);
	TEST_CHECK(info->missile->rect.width == 10);
	TEST_CHECK(info->jet->rect.height == 40);

	// Έλεγχος αν η λίστα που επιστρέφει η state_objects δεν είναι NULL και size != 0
	state = state_create();
	List list = state_objects(state,-10000,100000);
	TEST_CHECK(list_size(list) != 0);
	
}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, το αεροσκάφος μετακινείται 3 pixels πάνω
	Rectangle old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 3);

	// Με πατημένο το δεξί βέλος, το αεροσκάφος μετακινείται 6 pixels μπροστά
	keys.up = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 6);




	// Με πατημένο το κάτω βέλος, το αεροσκάφος μετακινείται 2 pixels πάνω
	keys.up = false;
	keys.down = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 2);

	// Με πατημένο το δεξί βέλος, το αεροσκάφος μετακινείται 3 pixels δεξιά
	keys.down = false;
	keys.right = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x + 3);


	//Έλεγχος για το αν κινείται σωστά ο πύραυλος
	keys.space = true;
	state_update(state,&keys);
	old_rect = state_info(state)->missile->rect;
	state_update(state,&keys);
	new_rect = state_info(state)->missile->rect;

	TEST_CHECK(new_rect.y == old_rect.y -10);

	//Έλεγχος για collisions του πυραύλου
	struct object obj = {HELICOPTER, {30,-10,20,10}, 1};
	state_info(state)->missile->rect.x = 30;
	state_info(state)->missile->rect.y = -10;
	state_info(state)->missile->rect.width = 20;
	state_info(state)->missile->rect.height = 10;

	TEST_CHECK(CheckCollisionRecs(state_info(state)->missile->rect, obj.rect));

	obj.rect = state_info(state)->missile->rect;
	TEST_CHECK(CheckCollisionRecs(state_info(state)->missile->rect, obj.rect));

}

// Πρόσθεση τεστς για την set_ex.c 
void test_set_ex(){
	Set set2 = set_create(compare_ints,NULL);
	SetNode n = malloc(sizeof(SetNode));
	
	set_insert(set2, create_int(2));
	set_insert(set2, create_int(3));
	set_insert(set2, create_int(4));
	set_insert(set2, create_int(5));

	n = set_first(set2);
	
	SetNode u = malloc(sizeof(SetNode));
	Pointer x = malloc(sizeof(Pointer));

	x = set_find_eq_or_greater(set2,create_int(1));

	u = set_find_node(set2,x);
	
	//Έλεγχος αν επιστρεφει τον επομενο κομβο αμα δεν υπαρχει το στοιχειο
	TEST_ASSERT(u == n);


	Set set3 = set_create(compare_ints,NULL);
	SetNode y = malloc(sizeof(SetNode));
	
	set_insert(set3, create_int(2));
	set_insert(set3, create_int(3));
	set_insert(set3, create_int(4));
	set_insert(set3, create_int(5));

	y = set_last(set2);
	
	SetNode o = malloc(sizeof(SetNode));
	Pointer p = malloc(sizeof(Pointer));

	p = set_find_eq_or_smaller(set2,create_int(6));

	o = set_find_node(set2,p);
	
	//Έλεγχος αν επιστρεφει τον προηγουμενο κομβο αμα δεν υπαρχει το στοιχειο
	TEST_ASSERT(o == y);
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },
	{ "test_set_ex", test_set_ex },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};