#ifndef FLOOR_H_INCLUDED
#define FLOOR_H_INCLUDED
#include "element.h"
#define TILE_SIZE 16
#define TILE_ROW 23
#define TILE_COL 40

//tile types
typedef enum{
    TILE_TYPE_AIR,
    TILE_TYPE_GROUND,
    TILE_TYPE_CLIMABLE,
    TILE_TYPE_HURT
}TileType;

extern TileType tile_type_table[256];

void init_tile_type_table();
/*
   [floor object]
*/
typedef struct _Floor
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    int map_data[TILE_ROW][TILE_COL];
} Floor;

Elements *New_Floor(int label, const char *map_path);
void Floor_update(Elements *self);
void Floor_interact(Elements *self);
void Floor_draw(Elements *self);
void Floor_destory(Elements *self);
void _Floor_load_map(Floor *floor, const char *map_path);
void _Floor_interact_Character(Elements *self, Elements *tar);

#endif
