#pragma once

#include "ADTVector.h"

// Ζευγάρι δύο ακεραίων
struct pair {
	int first;
	int second;
};
typedef struct pair* Pair;


// Βρίσκει αν υπάρχουν ακέραιοι a,b στο Vector numbers τέτοιοι ώστε a + b = target.
// Επιστρέφει ένα ζευγάρι με τους δύο ακεραίους, αν βρεθούν, ή NULL διαφορετικά.

Pair pair_sum(int target, Vector numbers);