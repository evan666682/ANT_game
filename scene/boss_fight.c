#include <allegro5/allegro_audio.h>
#include "boss_fight.h"
#include "sceneManager.h"
#include "../element/boss.h"

Scene *New_Boss_Fight(int label)
{
    Boss_Fight *pDerivedObj = (Boss_Fight *)malloc(sizeof(Boss_Fight));
    Scene *pObj = New_Scene(label);

    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/background_layer_1.png");
    pObj->pDerivedObj = pDerivedObj;

    Elements *boss = New_Boss(BOSS_L);
    _Register_elements(pObj, boss);
    
    // setting derived object function
    pObj->Update = boss_fight_update;
    pObj->Draw = boss_fight_draw;
    pObj->Destroy = boss_fight_destroy;
    return pObj;
}
void boss_fight_update(Scene *self)
{   
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
    printf("HERE\n");
    //switch to the second map
    if (key_state[ALLEGRO_KEY_P])
    {
        self->scene_end = true;
        window = 1;
        return;
    }
    
}
void boss_fight_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    Boss_Fight *gs = ((Boss_Fight *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);

    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
}
void boss_fight_destroy(Scene *self)
{
    Boss_Fight *Obj = (Boss_Fight *)(self->pDerivedObj);
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
