#include "GameWindow.h"
#include "GAME_ASSERT.h"
#include "global.h"
#include "GameClock.h"
#include "Resources.h"
#include "shapes/Shape.h"
#include "element/charater.h"
// include allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
// include scene and following component
#include "scene/sceneManager.h"
#include <stdbool.h>

static double last_time = 0;
static int last_day = 1;
extern ALLEGRO_FONT* count_font;

Game *New_Game()
{
    Game *game = (Game *)malloc(sizeof(Game));
    
    game->execute = execute;
    game->game_init = game_init;
    game->game_update = game_update;
    game->game_draw = game_draw;
    game->game_destroy = game_destroy;
    game->title = "Final Project Group 25";
    game->display = NULL;
    game->game_init(game);
    game->should_change_scene = false;
    game->next_scene_type = Menu_L;
    return game;
}
void execute(Game *self)
{
    // main game loop
    bool run = true;
    while (run)
    {
        // process all events here
        al_wait_for_event(event_queue, &event);
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
        {
            run &= self->game_update(self);
            self->game_draw(self);
            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE: // stop game
        {
            run = false;
            break;
        }
        case ALLEGRO_EVENT_KEY_DOWN:
        {
            key_state[event.keyboard.keycode] = true;
            break;
        }
        case ALLEGRO_EVENT_KEY_UP:
        {
            key_state[event.keyboard.keycode] = false;
            break;
        }
        case ALLEGRO_EVENT_MOUSE_AXES:
        {
            mouse.x = event.mouse.x;
            mouse.y = event.mouse.y;
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        {
            mouse_state[event.mouse.button] = true;
            break;
        }
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        {
            mouse_state[event.mouse.button] = false;
            break;
        }
        default:
            break;
        }
    }
}
void game_init(Game *self)
{
    printf("Game Initializing...\n");
    GAME_ASSERT(al_init(), "failed to initialize allegro.");
    // initialize allegro addons
    bool addon_init = true;
    addon_init &= al_init_primitives_addon();
    addon_init &= al_init_font_addon();   // initialize the font addon
    addon_init &= al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    addon_init &= al_init_image_addon();  // initialize the image addon
    addon_init &= al_init_acodec_addon(); // initialize acodec addon
    addon_init &= al_install_keyboard();  // install keyboard event
    addon_init &= al_install_mouse();     // install mouse event
    addon_init &= al_install_audio();     // install audio event
    GAME_ASSERT(addon_init, "failed to initialize allegro addons.");
    // Init Font
    count_font = al_load_ttf_font("assets/font/FFFFORWA.TTF", 24, 0);
    // Init Time
    last_time = al_get_time();
    Init_Game_Clock(&game_clock);
    // Init resorces
    Init_Resources(&resources);
    // Create display
    self->display = al_create_display(GAME_WIDTH, GAME_HEIGHT);
    GAME_ASSERT(self->display, "failed to create display.");
    // Create first scene
    create_scene(Menu_L);
    // create event queue
    event_queue = al_create_event_queue();
    GAME_ASSERT(event_queue, "failed to create event queue.");
    // Initialize Allegro settings
    al_set_window_position(self->display, 0, 0);
    al_set_window_title(self->display, self->title);
    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(self->display)); // register display event
    al_register_event_source(event_queue, al_get_keyboard_event_source());             // register keyboard event
    al_register_event_source(event_queue, al_get_mouse_event_source());                // register mouse event
    // register timer event
    fps = al_create_timer(1.0 / FPS);
    al_register_event_source(event_queue, al_get_timer_event_source(fps));
    al_start_timer(fps);
    // initialize the icon on the display
    ALLEGRO_BITMAP *icon = al_load_bitmap("assets/image/icon.jpg");
    al_set_display_icon(self->display, icon);
    // init canva
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_NO_PRESERVE_TEXTURE);
    self->canvas = al_create_bitmap(WIDTH, HEIGHT);
    
    GAME_ASSERT(self->canvas, "failed to create canvas.");
}
bool game_update(Game *self)
{
    //printf("[DEBUG] Game Update Start\n");

    if (!scene) {
        fprintf(stderr, "[ERROR] scene is NULL at update start!\n");
        return false;
    }
    
    if(window != 4){
        
    }
    double now = al_get_time();
    double delta = now - last_time;
    last_time = now;
    

    //只有螞蟻收集部分才會更新時間
    if (scene->label == Nest_L || scene->label == Road_L || scene->label == Kitchen_L) {
        
        Update_Game_Clock(&game_clock, delta);

        // 檢查是否超過3天
        if (game_clock.day > 3) {
            self->should_change_scene = true;
            self->next_scene_type = Game_Over_L;
        }

        if(game_clock.day != last_day){
            last_day = game_clock.day;
            Update_Resources(&resources);
        }
    }
    scene->Update(scene);

    //switch scene
    if (scene->scene_end && !self->should_change_scene) {
        Elements *ele = Get_Character(scene);
        if(ele != NULL && ele->pDerivedObj != NULL){
            Character *chara = (Character *)ele->pDerivedObj;
            last_x = chara->x;
            last_y = chara->y;
            last_scene = scene->label;
            //printf("Saved: last_x = %d, last_scene = %d\n", last_x, last_scene);
        }
        self->should_change_scene = true;
        self->next_scene_type = window;
        scene->scene_end = false;
    }
    
    if (self->should_change_scene) {

        if (self->next_scene_type == GAME_TERMINATE) {
            return false; // 終止遊戲主循環
        }

        if (scene->Destroy)
            scene->Destroy(scene);
        create_scene(self->next_scene_type);

        self->should_change_scene = false;
    }

    
    return true;
}


void game_draw(Game *self)
{
    // Flush the screen first.
    al_set_target_bitmap(self->canvas);
    al_clear_to_color(al_map_rgb(100, 100, 100));
    scene->Draw(scene);
    
    al_set_target_backbuffer(self->display);
    
    const int SCALE = 3; 
    al_draw_scaled_bitmap(
        self->canvas,
        0, 0, WIDTH, HEIGHT,
        0, 0, WIDTH * SCALE, HEIGHT * SCALE,
        0
    );

    Clock_Draw(&game_clock, 20, 20);
    Resources_Draw(&resources, WIDTH - 100, 20);

    al_flip_display();
}
void game_destroy(Game *self)
{
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(self->display);
    al_destroy_font(count_font);
    scene->Destroy(scene);
    free(self);
}
