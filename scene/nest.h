#ifndef NEST_H_INCLUDED
#define NEST_H_INCLUDED
#include "scene.h"
#include "element_type.h"

/*
   [game scene object]
*/

typedef struct _Nest
{
    ALLEGRO_BITMAP *background;

} Nest;
Scene *New_Nest(int label);
void nest_update(Scene *self);
void nest_draw(Scene *self);
void nest_destroy(Scene *self);

#endif
