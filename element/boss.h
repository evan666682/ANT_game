#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include "element.h"
#include "../algif5/algif.h"
#include <stdbool.h>


typedef struct _Boss
{
    int x, y;
    int width, height;              // the width and height of image
    int state;                      // the state of boss
    ALGIF_ANIMATION *anime_gif; // gif for each state. 0: stop, 1: move, 2:attack
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
} Boss;
Elements *New_Boss(int label);
void Boss_update(Elements *self);
void Boss_draw(Elements *self);
void Boss_destroy(Elements *self);
void Boss_interact(Elements *self);
#endif
