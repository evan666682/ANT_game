#include "floor.h"
#include "charater.h"
#include "../scene/nest.h" // for element label
#include "../scene/road.h" // for element label
#include "../scene/sceneManager.h" // for scene variable
#include <stdio.h>

TileType tile_type_table[256];

//Determine the tile type from tilemap.png
void init_tile_type_table(){
    for (int i = 0; i < 256; i++) tile_type_table[i] = TILE_TYPE_AIR;
    for (int i = 162; i <= 189; i++){
        tile_type_table[i] = TILE_TYPE_GROUND;
    }
    tile_type_table[81] = TILE_TYPE_CLIMABLE;
}
/*
   [floor function]
*/
Elements *New_Floor(int label, const char *map_path)
{
    Floor *pDerivedObj = (Floor *)malloc(sizeof(Floor));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/tileset.png");
    if(!pDerivedObj->img){
        fprintf(stderr, "[ERROR] Failed to load tileset\n");
        exit(1);
    }
    pDerivedObj->width = TILE_SIZE;
    pDerivedObj->height = TILE_SIZE;
    _Floor_load_map(pDerivedObj, map_path);
    pDerivedObj->x = 0;
    pDerivedObj->y = 0;
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Character_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Draw = Floor_draw;
    pObj->Update = Floor_update;
    pObj->Interact = Floor_interact;
    pObj->Destroy = Floor_destory;
    init_tile_type_table();
    return pObj;
}
void _Floor_load_map(Floor *floor, const char *map_path)
{
    FILE *data;
    data = fopen(map_path, "r");
    if(!data){
        fprintf(stderr, "[ERROR] Failed to load map\n");
        exit(1);
    }
    for (int i = 0; i < TILE_ROW; i++)
    {
        for (int j = 0; j < TILE_COL; j++)
        {
            fscanf(data, "%d", &floor->map_data[i][j]);
        }
    }
    
    fclose(data);
}
void Floor_update(Elements *ele) {}
void Floor_interact(Elements *self)
{
    ElementVec labelEle = _Get_label_elements(scene, Character_L);
    for (int i = 0; i < labelEle.len; i++)
    {
        _Floor_interact_Character(self, labelEle.arr[i]);
    }
}
void _Floor_interact_Character(Elements *self, Elements *tar)
{
    Character *chara = (Character *)(tar->pDerivedObj);

    int right_limit = WIDTH - chara->width / 2;
    int left_limit = 0 - chara->width / 2;
    if (chara->x < left_limit)
    {
        _Character_update_position(tar, (left_limit - chara->x), 0);
    }
    else if (chara->x > right_limit)
    {
        _Character_update_position(tar, (right_limit - chara->x), 0);
    }

}
void Floor_draw(Elements *self)
{
    Floor *Obj = ((Floor *)(self->pDerivedObj));
    for (int i = 0; i < TILE_ROW; i++)
    {
        for (int j = 0; j < TILE_COL; j++)
        {
            int tile_id = Obj->map_data[i][j];
            if(tile_id == -1) continue;
            int tiles_per_row = al_get_bitmap_width(Obj->img) / TILE_SIZE;

            int tx = (tile_id % tiles_per_row) * TILE_SIZE;
            int ty = (tile_id / tiles_per_row) * TILE_SIZE;

            al_draw_bitmap_region(
                Obj->img,
                tx + 0.01, ty + 0.01, TILE_SIZE - 0.02, TILE_SIZE - 0.02,
                Obj->x + j * TILE_SIZE,
                Obj->y + i * TILE_SIZE,
                0
            );
        }
    }
}
void Floor_destory(Elements *self)
{
    Floor *Obj = ((Floor *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj);
    free(self);
}
