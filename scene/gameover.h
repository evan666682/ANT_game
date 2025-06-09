#ifndef GAME_OVER_H_INCLUDED
#define GAME_OVER_H_INCLUDED
#include "scene.h"

typedef struct _Game_Over {
    ALLEGRO_FONT *font_large; 
    ALLEGRO_FONT *font_small; 
    int title_x, title_y;
} Game_Over;

Scene *New_Game_Over(int label);
void game_over_update(Scene *self);
void game_over_draw(Scene *self);
void game_over_destroy(Scene *self);

#endif