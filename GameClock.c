#include "GameClock.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

extern ALLEGRO_FONT* count_font;

void Init_Game_Clock(GameClock *clock){
    clock->day = 1;
    clock->hour = 8;
    clock->min = 0;
    clock->timer_accum = 0.0;
}

void Update_Game_Clock(GameClock *clock, double delta){
    double game_minute = delta * (540 / 300);

    clock->timer_accum += game_minute;

    while(clock->timer_accum >= 1.0){
        clock->min++;
        clock->timer_accum -= 1.0;

        if(clock->min >= 60){
            clock->hour++;
            clock->min = 0;

            if(clock->hour > 17){
                clock->hour = 8;
                clock->day++;
            }
        }
    }
}

void Clock_Draw(GameClock *clock, int x, int y){
    char buf[32];
    snprintf(buf, sizeof(buf), "Day %d %02d:%02d", clock->day, clock->hour, clock->min);
    al_draw_text(count_font, al_map_rgb(255, 255, 255), x, y, 0, buf);
}