#ifndef GAME_FINISH_H_INCLUDED
#define GAME_FINISH_H_INCLUDED
#include "scene.h"

typedef struct _Game_Finish {
    ALLEGRO_FONT *font_large; 
    ALLEGRO_FONT *font_small; 
    int title_x, title_y;
} Game_Finish;

Scene *New_Game_Finish(int label);
void game_finish_update(Scene *self);
void game_finish_draw(Scene *self);
void game_finish_destroy(Scene *self);

#endif