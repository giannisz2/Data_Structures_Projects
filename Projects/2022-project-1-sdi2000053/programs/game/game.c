#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;
KeyState keys;

void update_and_draw() {
	keys = malloc(sizeof(KeyState));

	keys->left = IsKeyDown(KEY_LEFT);
	keys->up = IsKeyDown(KEY_UP);
	keys->right = IsKeyDown(KEY_RIGHT);
	keys->down = IsKeyDown(KEY_DOWN);
	keys->enter = IsKeyPressed(KEY_ENTER);
	keys->p = IsKeyPressed(KEY_P);
	keys->n = IsKeyDown(KEY_N);


	state_update(state,keys);
	interface_draw_frame(state);

}

int main() {

	interface_init();
	state = state_create();
	
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}
