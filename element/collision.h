#ifndef COLLISION_H
#define COLLISION_H
#include <stdbool.h>
#include "../shapes/Rectangle.h"
#include "floor.h"
#define TILE_WALKABLE -1

bool check_collision(Shape *a, Shape *b);
bool is_tile_walkable(int tile_id);
bool check_tile_collision(Floor *floor, int x, int y);

#endif