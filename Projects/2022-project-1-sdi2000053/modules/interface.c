#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "state.h"
#include "interface.h"

void interface_close() {
    CloseAudioDevice();
	CloseWindow();
}

void interface_init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
}


void interface_draw_frame(State state) {
    BeginDrawing();
    
    StateInfo st = state_info(state);
    ClearBackground(BLUE);


    if(st->playing && st->paused == false && st->jet != NULL) {

        if(st->jet != NULL)
            DrawRectangle(st->jet->rect.x  , st->jet->rect.y + 700, 50, 70, WHITE);
    

           
        if(st->missile != NULL) {
            float o = st->missile->rect.x - (SCREEN_WIDTH- 250);
            float p = st->missile->rect.y - (SCREEN_HEIGHT - 150);
            DrawRectangle(st->missile->rect.x - o, st->missile->rect.y - p, 10, 30, WHITE);
        }
        
        List enemies = state_objects(state,0, 2*SCREEN_HEIGHT);
        ListNode node;
        Object enemy;

        for(node = list_first(enemies); node != LIST_EOF; node = list_next(enemies,node)) {
            enemy = list_node_value(enemies,node);
            int u = enemy->rect.x - (SCREEN_WIDTH -300);
            int t = enemy->rect.y - (SCREEN_HEIGHT - 250);
            int z = enemy->rect.x - (SCREEN_HEIGHT - 400);
            int j = enemy->rect.y - (SCREEN_HEIGHT - 350);
            int l = enemy->rect.y - (SCREEN_HEIGHT - 450);
            if(enemy != NULL) {
                if(enemy->type == WARSHIP)
                    DrawRectangle(enemy->rect.x - u, enemy->rect.y - t,  30, 10, YELLOW);
                else if(enemy->type == HELICOPTER)
                    DrawRectangle(enemy->rect.x - z, enemy->rect.y - j, 30, 10, PURPLE);
                else if(enemy->type == BRIDGE)
                    DrawRectangle(0, enemy->rect.y - l, SCREEN_WIDTH, 10, RED);
                else if(enemy->type == TERRAIN)
                    DrawRectangle(0,0,100,SCREEN_HEIGHT,GREEN);
            }
        }   
        
        //TEXT
        DrawText(TextFormat("%04i", st->score), 20, 20, 40, GRAY);
        DrawFPS(SCREEN_WIDTH - 80, 0);

    }else if(st->playing == false)
        DrawText("YOU DIED. PRESS [ENTER] TO RESTART THE GAME", 0, (SCREEN_HEIGHT / 2), 18, RED);
    else if(st->paused == true)
        DrawText("THE GAME IS PAUSED. PRESS P.", 55,(SCREEN_HEIGHT / 2), 20, RED);

    EndDrawing();
}
