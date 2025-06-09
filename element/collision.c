#include "collision.h"
#include "floor.h"
#include "../shapes/Rectangle.h"
bool check_collision(Shape *a, Shape *b){
    Rectangle *rect_a = (Rectangle*)a;
    Rectangle *rect_b = (Rectangle*)b;



    // 檢查指標是否有效 (可選，但建議)
    if (!rect_a || !rect_b) return false;

    double a_left = fmin(rect_a->x1, rect_a->x2);
    double a_right = fmax(rect_a->x1, rect_a->x2);
    double a_top = fmin(rect_a->y1, rect_a->y2);
    double a_bottom = fmax(rect_a->y1, rect_a->y2);

    double b_left = fmin(rect_b->x1, rect_b->x2);
    double b_right = fmax(rect_b->x1, rect_b->x2);
    double b_top = fmin(rect_b->y1, rect_b->y2);
    double b_bottom = fmax(rect_b->y1, rect_b->y2);

    return a_left < b_right &&
            a_right > b_left &&
            a_top < b_bottom &&
            a_bottom > b_top;
}


bool is_tile_walkable(int tile_id){
    TileType type = tile_type_table[tile_id];
    if(type == TILE_TYPE_GROUND) return true;
    return false;
}

bool check_tile_collision(Floor *floor, int x, int y){
    int col = x / TILE_SIZE;
    int row = y / TILE_SIZE;
    /*
    if(row < 0 || row >= TILE_ROW || col < 0 || col >= TILE_COL){
        return false;
    }*/
   if(row < 0 || row >= TILE_ROW || col < 0 || col >= TILE_COL){
        return false; // 如果在陣列外，應視為不可行走
    }
    int tile_id = floor->map_data[row][col];
    return is_tile_walkable(tile_id);
}