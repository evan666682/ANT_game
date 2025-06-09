#ifndef FOOD_H_INCLUDED
#define FOOD_H_INCLUDED
#include "element.h"
#include "../shapes/Shape.h"
#include <stdbool.h>

#define MAX_COLLECTION_PROGRESS 100.0
#define COLLECTION_RATE 0.85 

typedef struct _Food {
    int x, y;
    int width, height;
    ALLEGRO_BITMAP *img;
    Shape *hitbox;
    bool is_collected;
    int day_collected;
    double collection_progress; 
    bool character_is_colliding;
} Food;

Elements *New_Food(int label, int x, int y);
void Food_update(Elements *self);
void Food_draw(Elements *self);
void Food_destroy(Elements *self);
void Food_interact(Elements *self);

#endif