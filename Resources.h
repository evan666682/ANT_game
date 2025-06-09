#ifndef RESOURCES_H
#define RESOURCES_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

typedef struct{
    int ants;
    int food;
} Resources;

extern ALLEGRO_FONT* count_font;

void Init_Resources(Resources *self);
void Update_Resources(Resources *self);
void Resources_Draw(Resources *self, int x, int y);

#endif