#include "Resources.h"
ALLEGRO_FONT* count_font;
void Init_Resources(Resources *self){
    self->ants = 1;
    self->food = 0;
}
void Update_Resources(Resources *self){
    self->ants += 5;
}
void Resources_Draw(Resources *self, int x, int y){
    char buf[64];
    snprintf(buf, sizeof(buf), "Food: %d Ant: %d", self->food, self->ants);
    al_draw_text(count_font, al_map_rgb(255, 255, 255), x, y, 0, buf);
}
