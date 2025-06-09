#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "charater.h"
#include "projectile.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/algif.h"
#include "../scene/nest.h"
#include "../scene/road.h"
#include <stdio.h>
#include <stdbool.h>
extern Scene *scene;
/*
   [Character function]
*/
Elements *New_Character(int label)
{
    Character *pDerivedObj = (Character *)malloc(sizeof(Character));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load character images
    char state_string[3][10] = {"stop", "move", "attack"};
    
    for (int i = 0; i < 2; i++)
    {
        char buffer[50];
        sprintf(buffer, "assets/image/ant_%s.gif", state_string[i]);
        pDerivedObj->gif_status[i] = algif_new_gif(buffer, -1);
    }
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // initial the geometric information of character
    /*
    pDerivedObj->width = pDerivedObj->gif_status[0]->width;
    pDerivedObj->height = pDerivedObj->gif_status[0]->height;
    */
    pDerivedObj->width = 16;
    pDerivedObj->height = 16;
    pDerivedObj->x = 0;
    pDerivedObj->y = 10;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    // initial the animation component
    pDerivedObj->state = STOP;
    pDerivedObj->new_proj = false;
    pDerivedObj->on_ground = false;
    pDerivedObj->gravity = 0;
    pDerivedObj->climbing = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Character_draw;
    pObj->Update = Character_update;
    pObj->Interact = Character_interact;
    pObj->Destroy = Character_destroy;
    return pObj;
}
void Character_update(Elements *self)
{

    // use the idea of finite state machine to deal with different state
    Character *chara = ((Character *)(self->pDerivedObj));
    
    Character_interact(self);
    if(chara->climbing == true){
        //Climbing isn't finished yet
    }else if(chara->on_ground == false && chara->climbing == false){
        chara->gravity += 1;
        _Character_update_position(self, 0, chara->gravity);
    }
    else if(chara->on_ground == true){
        if(key_state[ALLEGRO_KEY_SPACE]){
            chara->gravity = -7;
            _Character_update_position(self, 0, chara->gravity);
            chara->on_ground = false;
        }
    }

    if (chara->state == STOP)
    {
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            chara->state = MOVE;
        }
        else
        {
            chara->state = STOP;
        }
    }
    else if (chara->state == MOVE)
    {
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            _Character_update_position(self, -4, 0);
            chara->state = MOVE;
        }
        else if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            _Character_update_position(self, 4, 0);
            chara->state = MOVE;
        }
        if (chara->gif_status[chara->state]->done)
            chara->state = STOP;
    }
}
void Character_draw(Elements *self)
{
    // with the state, draw corresponding image
    Character *chara = ((Character *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->gif_status[chara->state], al_get_time());
    if (frame)
    {
        al_draw_bitmap(frame, chara->x, chara->y, ((chara->dir) ? ALLEGRO_FLIP_HORIZONTAL : 0));
    }
    /*
    if (chara->state == STOP && chara->gif_status[chara->state]->display_index == 2)
    {
        al_play_sample_instance(chara->atk_Sound);
    }*/
}
void Character_destroy(Elements* self)
{
    if (!self) return;
    Character* c = (Character*)(self->pDerivedObj);
    if (!c) {
        printf("[WARN] character_destroy: pDerivedObj is NULL\n");
        return;
    }

    for (int i = 0; i < 2; i++) {
        if (c->gif_status[i])
            algif_destroy_animation(c->gif_status[i]);
    }

    free(c);
    free(self);
}


void _Character_update_position(Elements *self, int dx, int dy)
{
    Character *chara = ((Character *)(self->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

//the collision with floor
void Character_interact_floor(Elements *self, Elements *floorele) {
    Character *chara = (Character *)(self->pDerivedObj);
    Floor *floorObj = (Floor *)(floorele->pDerivedObj);

    int tile_x = chara->x;
    int tile_y = chara->y + chara->height;
    int tile_row = tile_y / TILE_SIZE;
    int tile_col = tile_x / TILE_SIZE;
    
    int top_y = chara->y;
    int left_x = chara->x;
    int right_x = chara->x + chara->width;
    int center_x = chara->x + chara->width / 2;
    int center_y = chara->y + chara->height / 2;
    
    int tile_id = floorObj->map_data[tile_row][tile_col];

    TileType type = tile_type_table[tile_id];
    //printf("%d\n", tile_id);

    if(check_tile_collision(floorObj, tile_x, tile_y)){
        switch(type){
            case TILE_TYPE_AIR:
                chara->on_ground = false;
                chara->climbing = false;
                break;
            case TILE_TYPE_CLIMABLE:
            case TILE_TYPE_GROUND:
                chara->y = tile_row * TILE_SIZE - chara->height;
                chara->gravity = 0;
                chara->on_ground = true;
                chara->climbing = false;
                break;
            case TILE_TYPE_HURT:
            default:
                chara->on_ground = false;
                break;
        }
    }else {
        chara->on_ground = false;
    }
    
    //collision with left wall
    if(check_tile_collision(floorObj, left_x, center_y)){
        chara->x = (left_x / TILE_SIZE + 1) * TILE_SIZE;
    }
    //collision with right wall
    if (check_tile_collision(floorObj, right_x, center_y)) {
        chara->x = (right_x / TILE_SIZE) * TILE_SIZE - chara->width;
    }
    //collision with celling
    if (check_tile_collision(floorObj, center_x, top_y)) {
        chara->y = (top_y / TILE_SIZE + 1) * TILE_SIZE;
    }
}
void Character_interact(Elements *self){
    ElementVec floors = _Get_label_elements(scene, Floor_L);
    if(floors.len > 0 && floors.arr[0] != NULL){
        Elements *floor = floors.arr[0];
        Character_interact_floor(self, floor);
    }else {
        fprintf(stderr, "[WARN] No floor element found!\n");
    }
}
