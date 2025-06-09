#include "global.h"
#include "shapes/Shape.h"
#include <stdbool.h>
// variables for global usage
const double FPS = 60.0;
const int GAME_WIDTH = 1920;
const int GAME_HEIGHT = 1080;
const int WIDTH = 640;
const int HEIGHT = 360;
int window = 0;

// Add with other global variable definitions
FoodState food_states[5] = {
    {true, -1}, // Menu_L (no food)
    {false, -1}, // Nest_L
    {false, -1}, // Road_L
    {false, -1}, // Kitchen_L
    {true, -1}  // Boss_Fight_L (no food)
};

ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
ALLEGRO_FONT *clock_font = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES] = {false};
Point mouse;
bool debug_mode = true;
GameClock game_clock;
Resources resources;
