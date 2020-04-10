#pragma once
#include<SDL2/SDL.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

const float dir_down_delay = 500;
const float dir_right_left_delay = 1500;

typedef struct{
    float x,y,vx,vy;
    int size;
    SDL_Texture* texture;
    int bullet_quantity;
    LinkedList bullets;
}Player;

typedef enum{
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_DOWN2
}Current_state_e1;

typedef struct{
    float x, y, vx,vy,size;
    SDL_Texture* texture;
    Current_state_e1 state;
    float current_time_elapsed;
}Enemy;

extern inline void render_enemy(SDL_Renderer* renderer,Enemy* enemy){
    SDL_Rect src_rect = {0,0,32,32};
    SDL_Rect dest_rect = {enemy->x,enemy->y,enemy->size,enemy->size};
    SDL_RenderCopy(renderer,enemy->texture,&src_rect,&dest_rect);
}

extern inline void update_enemy(Enemy* enemy,float dt){
    if(enemy == NULL){
        return;
    }
    enemy->current_time_elapsed += dt;
    //enemy->update(enemy);
    enemy->x += enemy->vx * dt/10;
    enemy->y += enemy->vy * dt/10;
    enemy->vx = 0;
    enemy->vy = 0;
    switch (enemy->state)
    {
        case DIR_LEFT:{
            if(enemy->current_time_elapsed >= dir_right_left_delay){
                enemy->state++;
                enemy->current_time_elapsed = 0;
            }else{
                enemy->vx = -2;
            }
        }break;
        case DIR_RIGHT:{
            if(enemy->current_time_elapsed >= dir_right_left_delay){
                enemy->state++;
                enemy->current_time_elapsed = 0;
            }else{
                enemy->vx = 2;
            }   
        }break;
        case DIR_DOWN:{
            if(enemy->current_time_elapsed >= dir_down_delay){
                enemy->state++;
                enemy->current_time_elapsed = 0;
            }else{
                 enemy->vy = 1;
            }
        }break;
        case DIR_DOWN2:{
            if(enemy->current_time_elapsed >= dir_down_delay){
                enemy->state++;
                enemy->current_time_elapsed = 0;
            }else{
                 enemy->vy = 1;
            }
        }break;
        default:{
            fprintf(stderr,"Enemy Error: Unknown animation state\n");
        }
    }
    enemy->state %= 4;
}
const int delay_between_spawn = 100;

typedef struct{
    Enemy* enemies[5000];
    int enemies_spawned;
    int elapsed_between_spawn;
}Enemy_spawner1;

extern inline void spawn_enemy(Enemy_spawner1* this,SDL_Texture* texture){
    Enemy* new_enemy = malloc(sizeof(Enemy));
    new_enemy->x = 50;
    new_enemy->y = -70;
    new_enemy->size = 70;
    new_enemy->current_time_elapsed = 0;
    new_enemy->vx = 0;
    new_enemy->vy = 0;
    new_enemy->texture = texture;
    new_enemy->state = DIR_RIGHT;
    this->enemies[this->enemies_spawned] = new_enemy;
}

extern inline void start_spawn(Enemy_spawner1* this,float dt,SDL_Texture* texture){
    if(this->enemies_spawned < 5000){
        this->elapsed_between_spawn += dt;
        if(this->elapsed_between_spawn > delay_between_spawn){
            spawn_enemy(this,texture);
            this->elapsed_between_spawn = 0;
            this->enemies_spawned++;
        }
        return;
    }
    return;
}
extern inline bool check_bullet_collide_enemy(Enemy* enemy,Player* player){
    if(player->bullets.head == NULL){
        return false;
    }
    Node* current_node = player->bullets.head;
    while(current_node != NULL){
        SDL_Rect bullet_rect = {current_node->bullet->x,current_node->bullet->y,current_node->bullet->size,current_node->bullet->size};
        SDL_Rect enemy_rect = {enemy->x,enemy->y,enemy->size,enemy->size};
        if(SDL_HasIntersection(&bullet_rect,&enemy_rect)){
            delete(&player->bullets,current_node->bullet);
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}
extern inline void update_enemies(Enemy_spawner1* this,float dt,Player* player){
    for(int i = 0;i < this->enemies_spawned;i++){
        if(this->enemies[i] == NULL)
            continue;
        if(check_bullet_collide_enemy(this->enemies[i],player)){
            this->enemies[i] = NULL;
        }
        update_enemy(this->enemies[i],dt);
    } 
}
extern inline void render_enemies(SDL_Renderer* renderer,Enemy_spawner1* this){
    for(int i = 0;i < this->enemies_spawned;i++){
        if(this->enemies[i] == NULL)
            continue;
        render_enemy(renderer,this->enemies[i]);
    } 
}

extern inline void delete_enemy(Enemy_spawner1* this){
    this->enemies[0] = NULL;
}