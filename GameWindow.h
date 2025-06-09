#ifndef GAMEWINDOW_H_INCLUDED
#define GAMEWINDOW_H_INCLUDED
#include <allegro5/allegro.h>
#include "Resources.h"
#include <stdbool.h>

typedef struct _GAME Game;
typedef void (*fptrGameExecute)(Game *);
typedef void (*fptrGameInit)(Game *);
typedef bool (*fptrGameUpdate)(Game *);
typedef void (*fptrGameDraw)(Game *);
typedef void (*fptrGameDestroy)(Game *);
void execute(Game *);
void game_init(Game *);
bool game_update(Game *);
void game_draw(Game *);
void game_destroy(Game *);
struct _GAME
{
    bool should_change_scene;
    int next_scene_type;
    const char *title;
    // ALLEGRO Variables
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *canvas;
    fptrGameExecute execute;
    fptrGameInit game_init;
    fptrGameUpdate game_update;
    fptrGameDraw game_draw;
    fptrGameDestroy game_destroy;

    // Resources resources;
};
Game *New_Game();

#endif
