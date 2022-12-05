#include <stdlib.h>

#include "ADTList.h"
#include "ADTVector.h"
#include "state.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	List objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητας (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


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

		list_insert_next(state->objects, list_last(state->objects), terain_left);
		list_insert_next(state->objects, list_last(state->objects), terain_right);
		list_insert_next(state->objects, list_last(state->objects), bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_WIDTH - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			list_insert_next(state->objects, list_last(state->objects), enemy);
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
	state->objects = list_create(NULL);
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
	ListNode node;
	ListNode node2 = malloc(sizeof(ListNode));
	
	//Διατρεχω τη λιστα για να παρω αντικειμενα στις συγκεκριμενες  συντεταγμενες
	for(node = list_first(state->objects);node != LIST_EOF; node = list_next(state->objects,node)) {
		Object object = list_node_value(state->objects,node);
		if(object->rect.y >= y_from && object->rect.y <= y_to) {
			list_insert_next(list_y,node2,object);
			node2 = list_next(list_y,node2);
		}	
	}

	return list_y;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	
	if(state->info.playing == true && state->info.paused == false && state->info.jet != NULL) {
			
		//Δημιουργια missile αν δεν υπαρχει
		if(state->info.missile == NULL) {
			if(keys->space) {
				state->info.missile = create_object(MISSILE, (SCREEN_WIDTH - 35)/2,  0, 10, 20);
			}
		}
		if(state->info.missile != NULL) {
			state->info.missile->rect.y += -10 ;
		}
		
		// Μετακινηση Jet
		if(keys->up)
			state->info.jet->rect.y += -6;
		else if(keys->down)
			state->info.jet->rect.y += -2;
		else	
			state->info.jet->rect.y += -3;	

		if(keys->left)
			state->info.jet->rect.x += -3;
		else if(keys->right)
			state->info.jet->rect.x += 3;
		else	
			state->info.jet->rect.x += 0;

		
		for(ListNode node1 = list_first(state->objects); node1 != LIST_EOF; node1 = list_next(state->objects,node1)) { //Για ολα τα αντικειμενα της λιστας
			Object enemy = list_node_value(state->objects,node1);
			if(enemy != NULL) {
				if(state->info.jet != NULL) {
					if(CheckCollisionRecs(state->info.jet->rect,enemy->rect) && enemy->type != TERRAIN) { // Αν το jet συγκρουστει με οτιδηποτε τοτε game over
						state->info.jet = NULL;
						state->info.playing = false;
			
					}
				}
				

				if(state->info.missile != NULL && state->info.jet != NULL) {
					if(CheckCollisionRecs(state->info.missile->rect,enemy->rect) && (enemy->type == HELICOPTER || enemy->type == WARSHIP || enemy->type == BRIDGE)) { //Τσεκαρω τα αντικειμενα για να δω αν πρεπει να ανεβαινει το σκορ 
						state->info.missile = NULL;
						state->info.score += 10;
						enemy = NULL;
					}
					if(state->info.missile != NULL && enemy != NULL) {
						if(CheckCollisionRecs(state->info.missile->rect, enemy->rect) && enemy->type == TERRAIN) //Αν ο πυραυλος χτυπησει τοιχο δεν παιρνει σκορ
							state->info.missile = NULL;
					}

					if(state->info.missile != NULL) {
						if(SCREEN_HEIGHT <= (state->info.jet->rect.y - state->info.missile->rect.y)) // Αν ο πυραυλος περασει την αποσταση αυτη καταστρεφεται
							state->info.missile = NULL;
					}
				}
			}

			if(state->info.jet != NULL && enemy != NULL) {
				// Μετακινηση των εχθρων
				if(enemy->type == HELICOPTER) {
					if(enemy->forward)
						enemy->rect.x += 4;
					else if(enemy->forward == 0)
						enemy->rect.x += -4;
				}
				
				if(enemy->type == WARSHIP) {
					if(enemy->forward)
						enemy->rect.x += 3;
					else if(enemy->forward == 0)
						enemy->rect.x += -3;
				}
				
				// Αλλαγη κατευθηνσης εχθρων αν συγκορυστουν με TERAIN
				for(ListNode node3 = list_first(state->objects); node3 != LIST_EOF; node3 = list_next(state->objects,node3)) {
					Object obj2 = list_node_value(state->objects,node3);

					if(CheckCollisionRecs(enemy->rect, obj2->rect) && obj2->type == TERRAIN) {
						if(enemy->forward)
							enemy->forward = false;
						else	
							enemy->forward = true;
					}
				}
				
				// Ελεγχος αν το jet φτασει σε συγκεκριμενη αποσταση απο γεφυρα και δημιουργηθουν νεα αντικειμενα καθως και να αλλαξει η ταχυτητα του παιχνιδιου
				if(enemy->type == BRIDGE) {
					if((state->info.jet->rect.y - enemy->rect.y) <= SCREEN_HEIGHT) {
						add_objects(state,enemy->rect.y);
						state->speed_factor += (state->speed_factor * 30) / 100;
					}
				}
				

			}
		}

		//Αν πατηθει παυση σταματα το παιχνιδι
		if(keys->p) {
			state->info.paused = true;
			
		}
		
	} //Αν ειναι game over πατα enter για new game αλλιως αν ειναι σε παυση πατα p να ξανασυνεχισεις ή n για update frame αλλιως return 
	else if(state->info.playing == false) {   
		if(keys->enter) {
			state->info.playing = true;
			state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);
			state = state_create();
			
		}

	}
	else if(state->info.playing == true && state->info.paused == true) {
		if(keys->p) {
			state->info.paused = false;
			
		}
		else if(keys->n)
			state_update(state,keys);
	}
	
}


// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	free(state);
}
