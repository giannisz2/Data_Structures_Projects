#include <stdlib.h>

#include "ADTList.h"
#include "ADTVector.h"
#include "ADTSet.h"
#include "ADTMap.h"
#include "state.h"
#include "set_utils.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Set objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};

int compare_obj(Pointer a, Pointer b) {
	if(a != NULL && b != NULL) {
		Object obj_b = malloc(sizeof(Object));
		Object obj_a;
		obj_a = a;
		obj_b = b;
		
		if(obj_a != NULL) {
			if(obj_a->rect.y > obj_b->rect.y)
				return 1;
			else if(obj_a->rect.y < obj_b->rect.y)
				return -1;
		}
	}
	return 0;
}

// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από y = start_y, και επεκτείνονται προς τα πάνω.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι το αριστερό μέρος της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά. Η πίστα έχει σταθερό πλάτος, οπότε όλες οι
//   συντεταγμένες x είναι ανάμεσα στο 0 και το SCREEN_WIDTH.
//
// - Στον άξονα y το 0 είναι η αρχή της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα αντιθέτως επεκτείνεται προς τα πάνω, οπότε όλες οι
//   συντεταγμένες των αντικειμένων είναι _αρνητικές_.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_y) {
	// Προσθέτουμε BRIDGE_NUM γέφυρες.
	// Στο διάστημα ανάμεσα σε δύο διαδοχικές γέφυρες προσθέτουμε:
	// - Εδαφος, αριστερά και δεξιά της οθόνης (με μεταβαλλόμενο πλάτος).
	// - 3 εχθρούς (ελικόπτερα και πλοία)
	// Τα αντικείμενα έχουν SPACING pixels απόσταση μεταξύ τους.

	for (int i = 0; i < BRIDGE_NUM; i++) {
		// Δημιουργία γέφυρας
		Object bridge = create_object(
			BRIDGE,
			0,								// x στο αριστερό άκρο της οθόνης
			start_y - 4 * (i+1) * SPACING,	// Η γέφυρα i έχει y = 4 * (i+1) * SPACING
			SCREEN_WIDTH,					// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// Δημιουργία εδάφους
		Object terain_left = create_object(
			TERRAIN,
			0,								// Αριστερό έδαφος, x = 0
			bridge->rect.y,					// y ίδιο με την γέφυρα
			rand() % (SCREEN_WIDTH/3),		// Πλάτος επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);
		int width = rand() % (SCREEN_WIDTH/2);
		Object terain_right = create_object(
			TERRAIN,
			SCREEN_WIDTH - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
			bridge->rect.y,					// y ίδιο με τη γέφυρα
			width,							// Πλάτος, επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);

		set_insert(state->objects, terain_left);
		set_insert(state->objects, terain_right);
		set_insert(state->objects, bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_WIDTH - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			set_insert(state->objects, enemy);
		}
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->info.score = 0;					// Αρχικό σκορ 0
	state->info.missile = NULL;				// Αρχικά δεν υπάρχει πύραυλος
	state->speed_factor = 1;				// Κανονική ταχύτητα

	// Δημιουργία του αεροσκάφους, κεντραρισμένο οριζόντια και με y = 0
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	// Δημιουργούμε τη λίστα των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	state->objects = set_create(compare_ints,NULL);
	add_objects(state, 0);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info; 
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη y είναι ανάμεσα στο y_from και y_to.

List state_objects(State state, float y_from, float y_to) {
	List list_y = list_create(NULL);
	ListNode node1 = malloc(sizeof(ListNode));
	
	SetNode node2 = set_first(state->objects);
	Object obj = set_node_value(state->objects,node2);
	obj->rect.y = y_from;

	Object first = set_find_eq_or_greater(state->objects,obj);
	node2 = set_find(state->objects,first);
	
	for(int i = 0; i < set_size(state->objects); i++) {
		Pointer x = set_node_value(state->objects,node2);
		node2 = set_next(state->objects,node2);
		list_insert_next(list_y,node1,x);
		node1 = list_next(list_y,node1);
	}
	

	return list_y;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	
	if(state->info.playing && state->info.paused == false) {


		if(state->info.missile == NULL) {
			if(keys->space)
				state->info.missile = create_object(MISSILE, state->info.jet->rect.x, state->info.jet->rect.y - 3, 10, 20);
		}
		if(state->info.missile != NULL) {
			state->info.missile->rect.x += -10 * state->speed_factor;
		}
		

		if(keys->up)
			state->info.jet->rect.y += -6 * state->speed_factor;
		else if(keys->down)
			state->info.jet->rect.y += -2 * state->speed_factor;
		else
			state->info.jet->rect.y += -3 * state->speed_factor;

		if(keys->left)
			state->info.jet->rect.x += -3 * state->speed_factor;
		else if(keys->right)
			state->info.jet->rect.x += 3 * state->speed_factor;

		Map map = map_create(compare_ints,NULL,NULL);

		//Λιστα με αντικειμενα μονο σε αποσταση 2 οθονων απο το jet
		List list_x = state_objects(state, state->info.jet->rect.y, 2 * SCREEN_HEIGHT - state->info.jet->rect.y);
		for(ListNode n = list_first(list_x); n != LIST_EOF; list_next(list_x,n)) {
			Object enemy = list_node_value(list_x,n);

			// Max συντεταγμενες που μπορει να κινηθει ο εχθρος
			float x_max = SCREEN_WIDTH - enemy->rect.x;
			float x_min = enemy->rect.x;

			map_insert(map,create_float(x_max),create_float(enemy->rect.x));
			map_insert(map,create_float(x_min),create_float(enemy->rect.x));
			
			if(CheckCollisionRecs(state->info.jet->rect,enemy->rect) && enemy->type != TERRAIN) {
				state->info.jet = NULL;
				state->info.playing = false;
			}

			if(state->info.missile != NULL) {
				if(CheckCollisionRecs(state->info.missile->rect,enemy->rect) && (enemy->type == HELICOPTER || enemy->type == WARSHIP || enemy->type == BRIDGE)) {
					state->info.missile = NULL;
					state->info.score += 10;
					enemy = NULL;
				}

				if(state->info.missile != NULL)
					if(CheckCollisionRecs(state->info.missile->rect, enemy->rect) && enemy->type == TERRAIN)
						state->info.missile = NULL;

				if(state->info.missile != NULL)
					if(SCREEN_HEIGHT <= (state->info.jet->rect.y - state->info.missile->rect.y))
						state->info.missile = NULL;
			}
			if(state->info.jet != NULL && enemy != NULL) {

				if(enemy->type == HELICOPTER) {
					if(enemy->forward)
						enemy->rect.x += 4 * state->speed_factor;
					else if(enemy->forward == 0)
						enemy->rect.x += -4 * state->speed_factor;
				}
			
				if(enemy->type == WARSHIP) {
					if(enemy->forward)
						enemy->rect.x += 3 * state->speed_factor;
					else if(enemy->forward == 0)
						enemy->rect.x += -3 * state->speed_factor;
				}
			}

			//Δεν εξεταζει ολα τα αντικειμενα της πιστας.
			List list3 = state_objects(state,enemy->rect.y + 200,enemy->rect.y - 200);
			for(ListNode node = list_first(list3); node != NULL; node = list_next(list3,node)) {
				Object obj2 = list_node_value(list3,node);
				if(obj2->type == TERRAIN) {
					if(map_find(map,create_float(obj2->rect.x)) != NULL) 
						enemy->forward = false;
					else	
						enemy->forward = true;
						
				}
			}
			
			if(enemy->type == BRIDGE && state->info.jet != NULL) {
				if((state->info.jet->rect.y - enemy->rect.y) <= SCREEN_HEIGHT) {
					add_objects(state,enemy->rect.y);
					state->speed_factor += (state->speed_factor * 30) / 100;
				}
			}

		}

		if(keys->p) 
			state->info.paused = true;
		
	}else if(state->info.playing == false) {   
		if(keys->enter) {
			state->info.playing = true;
			state = state_create();
		}
	}
	else if(state->info.playing == true && state->info.paused == true) {
		if(keys->p)
			state->info.paused = false;
		else if(keys->n)
			state_update(state,keys);
	}
	
}


// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	free(state);
}
