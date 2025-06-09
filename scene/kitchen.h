#ifndef KITCHEN_H_INCLUDED
#define KITCHEN_H_INCLUDED
#include "scene.h"
#include "element_type.h"

/*
   [game scene object]
*/

typedef struct _Kitchen
{
    ALLEGRO_BITMAP *background;

} Kitchen;
Scene *New_Kitchen(int label);
void kitchen_update(Scene *self);
void kitchen_draw(Scene *self);
void kitchen_destroy(Scene *self);

#endif
