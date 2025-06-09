#ifndef BOSS_FIGHT_H_INCLUDED
#define BOSS_FIGHT_H_INCLUDED
#include "scene.h"
#include "element_type.h"

/*
   [game scene object]
*/

typedef struct _Boss_Fight
{
    ALLEGRO_BITMAP *background;
} Boss_Fight;
Scene *New_Boss_Fight(int label);
void boss_fight_update(Scene *self);
void boss_fight_draw(Scene *self);
void boss_fight_destroy(Scene *self);

#endif
