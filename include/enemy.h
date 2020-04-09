#pragma once
#include<SDL2/SDL.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct{
    float x, y, vx,vy,size;
    SDL_Texture* texture;
    void(*update)();
}Enemy;

extern inline void render_enemy(SDL_Renderer* renderer,Enemy* enemy){
    SDL_Rect src_rect = {0,0,32,32};
    SDL_Rect dest_rect = {enemy->x,enemy->y,enemy->size,enemy->size};
    SDL_RenderCopy(renderer,enemy->texture,&src_rect,&dest_rect);
}

extern inline void update_enemy(Enemy* enemy,float dt){
    //enemy->update(enemy);
    enemy->x += enemy->vx * dt/10;
    enemy->y += enemy->vy * dt/10;
    
}