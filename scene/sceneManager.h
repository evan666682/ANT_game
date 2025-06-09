#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED
#include "scene.h"
extern Scene *scene;

// for determining spawning point when switched map
extern int last_x; 
extern int last_y;
extern bool has_last_position;

typedef enum SceneType
{
    Menu_L = 0,
    Nest_L,
    Road_L,
    Kitchen_L,
    Boss_Fight_L
} SceneType;
extern SceneType last_scene;
void create_scene(SceneType);


#endif