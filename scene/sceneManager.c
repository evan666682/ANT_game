#include "sceneManager.h"
#include "../element/charater.h"
#include "menu.h"
#include "road.h"
#include "nest.h"
#include "kitchen.h"
#include "boss_fight.h"

Scene *scene = NULL;

// for determining spawning point when switched map
int last_x = 0; 
int last_y = 0;
bool has_last_position = false; 
SceneType last_scene = 0;

void create_scene(SceneType type)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        break;
    case Road_L:
        scene = New_Road(Road_L);
        break;
    case Nest_L:
        scene = New_Nest(Nest_L);
        break;
    case Kitchen_L:
        scene = New_Kitchen(Kitchen_L);
        break;
    case Boss_Fight_L:
        scene = New_Boss_Fight(Boss_Fight_L);
        break;
    default:
        break;
    }
}
