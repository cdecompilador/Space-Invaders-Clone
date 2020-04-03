#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<SDL2/SDL.h>

#include"lodepng.h"
#include"glm/glm.h"

typedef struct{
    glm_vec2 pos;
    glm_vec2 vel;
    int size;
    SDL_Texture* texture;
}Player;

void render_player(SDL_Renderer* renderer,Player* player){
    SDL_Rect src_rect = {0,0,32,32};
    SDL_Rect dest_rect = {player->pos.x,player->pos.y,player->size,player->size};
    SDL_RenderCopy(renderer,player->texture,&src_rect,&dest_rect);
}

SDL_Texture* load_texture_from_png(SDL_Renderer* renderer,char* filename){
    uint8_t *image;
    int width,height;
    int error = lodepng_decode32_file(&image,&width,&height,filename);

    if(error){
        printf("lodepng error");
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(image,width,height,32,width*4,
                                0xff000000,
                                0x00ff0000,
                                0x0000ff00,
                                0x000000ff);
    return SDL_CreateTextureFromSurface(renderer,surface);
}

int main(int argc,char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Space Invaders Clone",20,20,600,600,0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    SDL_Texture *player_texture = load_texture_from_png(renderer,"nave.png");

    Player player;
    memset(&player,0,sizeof(player));
    player.pos.x = 50;
    player.pos.y = 50;
    player.size = 70;
    player.texture = player_texture;

    bool running = true;

    SDL_Event event;
    while(running){

        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:{
                return -1;
            }
        }
        SDL_SetRenderDrawColor(renderer,27,58,89,255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,0,255,0,255);

        render_player(renderer,&player);

        SDL_RenderPresent(renderer);
    }

    return 0;
}






