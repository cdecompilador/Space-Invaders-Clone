#pragma once
#include<SDL2/SDL.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct{
    float x, y, size;
    SDL_Texture* texture;
}Enemy;