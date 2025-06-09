#include "food.h"
#include "../global.h"
#include "../scene/scene.h"
#include "charater.h"
#include "../shapes/Rectangle.h"
#include "../scene/element_type.h" //  <-- 新增
#include "../scene/sceneManager.h"   //  <-- 新增
#include "collision.h"             //  <-- 新增 (為了 check_collision)


Elements *New_Food(int label, int x, int y,int scene_label) {
    Food *pDerivedObj = (Food *)malloc(sizeof(Food));
    Elements *pObj = New_Elements(label);

    pDerivedObj->img = al_load_bitmap("assets/image/icon.jpg");
    if (!pDerivedObj->img) {
        fprintf(stderr, "failed to load food image\n");
        return NULL;
    }

    pDerivedObj->width = 32;
    pDerivedObj->height = 32;
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->hitbox = New_Rectangle(x, y, x + pDerivedObj->width, y + pDerivedObj->height);

    pDerivedObj->is_collected = food_states[scene_label].is_collected;
    pDerivedObj->day_collected = food_states[scene_label].day_collected;
    pDerivedObj->collection_progress = 0;
    // pDerivedObj->character_is_colliding = false;
    
    pObj->pDerivedObj = pDerivedObj;
    pObj->inter_obj[pObj->inter_len++] = Character_L;

    pObj->Update = Food_update;
    pObj->Draw = Food_draw;
    pObj->Destroy = Food_destroy;
    pObj->Interact = Food_interact;

    return pObj;
}

void Food_update(Elements *self) {
    Food *food = (Food *)(self->pDerivedObj);

    // Respawn logic
    if (food->is_collected && game_clock.day > food->day_collected) {
        food->is_collected = false;
        food->day_collected = -1;

        food_states[scene->label] = (FoodState){.is_collected = false, .day_collected = -1};
    }

    if (food->is_collected) {
        return;
    }

    // Collection logic
     ElementVec char_vec = _Get_label_elements(scene, Character_L);
    bool colliding = false;
    if (char_vec.len > 0) {
        Elements *char_ele = char_vec.arr[0];
        Character *character = (Character *)(char_ele->pDerivedObj);
        if  (food->hitbox->overlap(food->hitbox, character->hitbox)) {
            colliding = true;
        }
    }

    if (colliding) {
        if (key_state[ALLEGRO_KEY_F]) {
            food->collection_progress += COLLECTION_RATE;
            if (food->collection_progress >= MAX_COLLECTION_PROGRESS) {
                resources.food += 2 * resources.ants;
                food->is_collected = true;
                food->day_collected = game_clock.day;
                food->collection_progress = 0;

                food_states[scene->label] = (FoodState){.is_collected = true, .day_collected = game_clock.day};
            }
        } else {
            food->collection_progress = 0; // Reset progress if F is not held
        }
    } else {
        food->collection_progress = 0; // Reset progress if not colliding
    }
}

void Food_interact(Elements *self) {
    // Food *food = (Food *)(self->pDerivedObj);

    // if (food->is_collected) return;

    // ElementVec char_vec = _Get_label_elements(scene, Character_L);
    // if (char_vec.len > 0) {
    //     Elements *char_ele = char_vec.arr[0];
    //     Character *character = (Character *)(char_ele->pDerivedObj);
        
    //     if (check_collision(food->hitbox, character->hitbox)) {
    //         food->character_is_colliding = true;
    //     }
    // }
    return;
}

void Food_draw(Elements *self) {
    Food *food = (Food *)(self->pDerivedObj);
    if (!food->is_collected) {
        al_draw_scaled_bitmap(food->img, 0, 0, al_get_bitmap_width(food->img), al_get_bitmap_height(food->img), food->x, food->y, food->width, food->height, 0);

        if (food->collection_progress > 0) {
            float bar_x = food->x;
            float bar_y = food->y - 10;
            float bar_width = food->width;
            float bar_height = 5;

            // Draw progress bar background
            al_draw_filled_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(100, 100, 100));

            // Draw progress
            float progress_width = (food->collection_progress / MAX_COLLECTION_PROGRESS) * bar_width;
            al_draw_filled_rectangle(bar_x, bar_y, bar_x + progress_width, bar_y + bar_height, al_map_rgb(0, 255, 0));
            
            // Draw progress bar outline
            al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(255, 255, 255), 2);
        }
    }
}

void Food_destroy(Elements *self) {
    Food *food = (Food *)(self->pDerivedObj);
    al_destroy_bitmap(food->img);
    free(food->hitbox->pDerivedObj);
    free(food->hitbox);
    free(food);
    free(self);
}