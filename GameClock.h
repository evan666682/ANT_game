#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

typedef struct{
    int hour;
    int min;
    int day;
    double timer_accum;
} GameClock;

void Init_Game_Clock(GameClock *clock);
void Update_Game_Clock(GameClock *clock, double delta);
void Clock_Draw(GameClock *clock, int x, int y);
#endif