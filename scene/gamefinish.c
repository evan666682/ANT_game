#include "gamefinish.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "../global.h"

Scene *New_Game_Finish(int label) {
    Game_Finish *pDerivedObj = (Game_Finish *)malloc(sizeof(Game_Finish));
    Scene *pObj = New_Scene(label);
    pDerivedObj->font_large = al_load_ttf_font("assets/font/pirulen.ttf", 36, 0);
    pDerivedObj->font_small = al_load_ttf_font("assets/font/pirulen.ttf", 18, 0);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = game_finish_update;
    pObj->Draw = game_finish_draw;
    pObj->Destroy = game_finish_destroy;
    return pObj;
}

void game_finish_update(Scene *self) {
    if (key_state[ALLEGRO_KEY_ENTER]) {
        self->scene_end = true;
        window = GAME_TERMINATE;
    }
}

void game_finish_draw(Scene *self) {
    Game_Finish *Obj = (Game_Finish *)(self->pDerivedObj);
    al_clear_to_color(al_map_rgb(0, 0, 100));
    al_draw_text(Obj->font_large, al_map_rgb(0, 255, 0), Obj->title_x, Obj->title_y, ALLEGRO_ALIGN_CENTRE, "YOU WIN!");
    al_draw_text(Obj->font_small, al_map_rgb(255, 255, 255), Obj->title_x, Obj->title_y + 60, ALLEGRO_ALIGN_CENTRE, "Press Enter to exit");
}

void game_finish_destroy(Scene *self) {
    Game_Finish *Obj = (Game_Finish *)(self->pDerivedObj);
    al_destroy_font(Obj->font_large);
    al_destroy_font(Obj->font_small);
    free(Obj);
    free(self);
}