#include <allegro5/allegro_audio.h>
#include "road.h"
#include "sceneManager.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/floor.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/food.h"
/*
   The middle scene
*/
Scene *New_Road(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    Elements *ele = New_Character(Character_L);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/background_layer_1.png");
    pObj->pDerivedObj = pDerivedObj;
    if(!pDerivedObj->background){
        fprintf(stderr, "[ERROR] Failed to load background\n");
        exit(1);
    }
    // register element
    _Register_elements(pObj, New_Floor(Floor_L, "assets/map/road_map.txt"));
    _Register_elements(pObj, New_Food(Food_L, 300, 250,pObj->label));//增加食物
    //_Register_elements(pObj, New_Teleport(Teleport_L));
    //_Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, ele);
  
    Character *chara = (Character*)ele->pDerivedObj;
    

    //set character position base on the previous map
    if(has_last_position){
        if(last_scene == Nest_L && last_x >= WIDTH - chara->width){
            chara->x = 10;
        }else if(last_scene == Kitchen_L && last_x <= 0){
            chara->x = WIDTH - chara->width - 20;
        }else {
            chara->x = 5;
        }
    }else chara->x = 5;
    chara->y = 10;
    
    // setting derived object function
    pObj->Update =  road_update;
    pObj->Draw =  road_draw;
    pObj->Destroy =  road_destroy;
    
    return pObj;
}
void  road_update(Scene *self)
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
    for (int i = 0; i < allEle.len; i++) {
        if (allEle.arr[i]->label == Character_L) {
            chara = (Character *)(allEle.arr[i]->pDerivedObj);
            break;
        }
    }

    //prevent character from falling through the floor due to map not fully loaded
    if(chara->y >= HEIGHT){
        chara->y = 10;
    }
    
    //switch to the first scene and the third dcene
    if (chara != NULL && chara->x <= 0)
    {
        self->scene_end = true;
        window = 1;
        return;
    }else if (chara != NULL && chara->x + chara->width >= WIDTH)
    {
        self->scene_end = true;
        window = 3;
        return;
    }
    
}
void road_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
}
void road_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
