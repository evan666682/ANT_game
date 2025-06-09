#include <allegro5/allegro_audio.h>
#include "kitchen.h"
#include "sceneManager.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/food.h"
/*
   Kitchen is the rightmost scene
*/
Scene *New_Kitchen(int label)
{
    Kitchen *pDerivedObj = (Kitchen *)malloc(sizeof(Kitchen));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/background_layer_1.png");
    if(!pDerivedObj->background){
        fprintf(stderr, "[ERROR] Failed to load background\n");
        exit(1);
    }
    pObj->pDerivedObj = pDerivedObj;
    
    Elements *floor = New_Floor(Floor_L, "assets/map/kitchen_map.txt");
    Elements *ele = New_Character(Character_L);
    Elements *food = New_Food(Food_L, 400, 240);//增加食物
    // register element
    _Register_elements(pObj, floor);
    _Register_elements(pObj, ele);
    _Register_elements(pObj, food);//增加食物
    

    Character *chara = (Character*)ele->pDerivedObj;
    
    chara->x = 10;
    chara->y = 10;
    
    // setting derived object function
    pObj->Update = kitchen_update;
    pObj->Draw = kitchen_draw;
    pObj->Destroy = kitchen_destroy;
    return pObj;
}
void kitchen_update(Scene *self)
{   
    Character *chara = NULL;
    // update every element
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele);
    }
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Interact(ele);
    }
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
    // link with character
    for (int i = 0; i < allEle.len; i++)
    {
        if(allEle.arr[i]->label == Character_L){
            chara = (Character *)(allEle.arr[i]->pDerivedObj);
            break;
        }
    }

    //Prevent the character from falling through the floor before the map is fully loaded.
    if(chara->y >= HEIGHT){
        chara->y = 10;
    }
    //switch to the second map
    
    if (chara != NULL && chara->x <= 0)
    {
        self->scene_end = true;
        window = 2;
        return;
    }else if (chara != NULL && chara->x + chara->width >= WIDTH)
    {
        self->scene_end = true;
        window = 4;
        return;
    }
    
}
void kitchen_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    Kitchen *gs = ((Kitchen *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
}
void kitchen_destroy(Scene *self)
{
    Kitchen *Obj = (Kitchen *)(self->pDerivedObj);
    if (!Obj) {
        //printf("[ERROR] self->pDerivedObj is NULL\n");
        return;
    }

    //printf("[DEBUG] Freeing background...\n");
    if (Obj->background)
        al_destroy_bitmap(Obj->background);

    //printf("[DEBUG] Getting all elements...\n");
    ElementVec allEle = _Get_all_elements(self);
    //printf("[DEBUG] Total elements: %d\n", allEle.len);

    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        //printf("[DEBUG] element %d label = %d\n", i, ele->label);

        //printf("[DEBUG] Destroying element %d at %p\n", i, ele);
        if (ele) {
            if (ele->Destroy) {
                ele->Destroy(ele);
                //printf("[DEBUG] Destroyed element %d successfully\n", i);
            } else {
                //printf("[WARN] ele->Destroy is NULL at index %d\n", i);
            }
        } else {
            //printf("[WARN] Element %d is NULL\n", i);
        }
    }

    //printf("[DEBUG] Freeing Kitchen object...\n");
    free(Obj);

    //printf("[DEBUG] Freeing Scene object...\n");
    free(self);

    //printf("[DEBUG] kitchen_destroy() finished\n");
}
