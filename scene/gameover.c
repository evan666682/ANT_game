#include "./gameover.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "../global.h"

Scene *New_Game_Over(int label) {
    Game_Over *pDerivedObj = (Game_Over *)malloc(sizeof(Game_Over));
    Scene *pObj = New_Scene(label);
    pDerivedObj->font_large = al_load_ttf_font("assets/font/pirulen.ttf", 36, 0);
    pDerivedObj->font_small = al_load_ttf_font("assets/font/pirulen.ttf", 18, 0);

    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = game_over_update;
    pObj->Draw = game_over_draw;
    pObj->Destroy = game_over_destroy;
    return pObj;
}

void game_over_update(Scene *self) {
    if (key_state[ALLEGRO_KEY_ENTER]) {
        // 設定 window 為 GAME_TERMINATE 會讓主遊戲循環停止
        self->scene_end = true;
        window = GAME_TERMINATE;
    }
}

void game_over_draw(Scene *self) {
    Game_Over *Obj = (Game_Over *)(self->pDerivedObj);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    al_draw_text(Obj->font_large, al_map_rgb(255, 0, 0), Obj->title_x, Obj->title_y, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_draw_text(Obj->font_small, al_map_rgb(255, 255, 255), Obj->title_x, Obj->title_y + 60, ALLEGRO_ALIGN_CENTRE, "Press Enter to exit");
}

void game_over_destroy(Scene *self) {
    Game_Over *Obj = (Game_Over *)(self->pDerivedObj);
    al_destroy_font(Obj->font_large);
    al_destroy_font(Obj->font_small);
    free(Obj);
    free(self);
}