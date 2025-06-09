#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "boss.h"
#include "../scene/sceneManager.h"
#include "../algif5/algif.h"
#include "../scene/boss_fight.h"
#include <stdio.h>
#include <stdbool.h>
extern Scene *scene;
/*
   [Boss function]
*/
Elements *New_Boss(int label)
{
    Boss *pDerivedObj = (Boss *)malloc(sizeof(Boss));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load boss images
    //char state_string[3][10] = {"stop", "move", "attack"};
    pDerivedObj->anime_gif = algif_load_animation("assets/image/boss.gif");
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    pDerivedObj->width = 240;
    pDerivedObj->height = 315;
    pDerivedObj->x = 0;
    pDerivedObj->y = 10;

    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Draw = Boss_draw;
    pObj->Update = Boss_update;
    pObj->Destroy = Boss_destroy;
    pObj->Interact = Boss_interact;
    return pObj;
}
void Boss_interact(Elements *self){
    return;
}
void Boss_update(Elements *self)
{
    return;
    //Boss *chara = ((Boss *)(self->pDerivedObj));
}
void Boss_draw(Elements *self)
{
    // with the state, draw corresponding image
    Boss *chara = ((Boss *)(self->pDerivedObj));
    ALLEGRO_BITMAP *frame = algif_get_bitmap(chara->anime_gif, al_get_time());
    if(frame){
        al_draw_bitmap(frame, 5, 5, 0);
    }
}
void Boss_destroy(Elements* self)
{
    if (!self) return;
    Boss* c = (Boss*)(self->pDerivedObj);
    if (!c) {
        printf("[WARN] boss_destroy: pDerivedObj is NULL\n");
        return;
    }

    if(c->anime_gif){
        algif_destroy_animation(c->anime_gif);
        c->anime_gif = NULL;
    }

    free(c);
    free(self);
}
