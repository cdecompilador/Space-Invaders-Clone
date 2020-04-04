#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<math.h>

#include"lodepng.h"

const int WIDTH =  600;
const int HEIGHT = 600;

typedef struct{
    float x,y,vx,vy;
    float size;
    float angle;
    SDL_Texture* texture;
}Bullet;
void update_bullet(Bullet* bullet){
    bullet->x += bullet->vx;
    bullet->vy += bullet->vy;
    bullet->angle = acosf(bullet->vx/sqrtf(pow(bullet->vx,2)+pow(bullet->vx,2)));
    printf("Angle: %f\n",bullet->angle);
}
void render_bullet(SDL_Renderer* renderer,Bullet* bullet){
    SDL_Rect src_rect = {0,0,6,6};
    SDL_Rect dest_rect = {bullet->x,bullet->y,bullet->size,bullet->size};
    SDL_RenderCopy(renderer,bullet->texture,&src_rect,&dest_rect);
}

typedef struct{
    float x,y,vy;
    float size;
    SDL_Texture* texture;
}Star;

void update_star(Star *star,float dt){
    star->y += star->vy * dt/5;

    if(star->y > WIDTH){
        star->y = rand()%WIDTH - WIDTH;
    }
}
void render_star(SDL_Renderer *renderer,Star *star){
    SDL_Rect src_rect = {0,0,6,6};
    SDL_Rect dest_rect = {star->x,star->y,star->size,star->size};
    SDL_SetTextureAlphaMod(star->texture,150);
    SDL_RenderCopy(renderer,star->texture,&src_rect,&dest_rect);
}

typedef struct{
    float x,y,vx,vy;
    int size;
    SDL_Texture* texture;
}Player;

void render_player(SDL_Renderer* renderer,Player* player){

    SDL_Rect src_rect = {0,0,32,32};
    SDL_Rect dest_rect = {player->x,player->y,player->size,player->size};
    SDL_RenderCopy(renderer,player->texture,&src_rect,&dest_rect);
    int alpha = 200;
    int line_y = 62;
    if(player->vy < 0){
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        for(int i = 0;i < 20;i++){
            SDL_SetRenderDrawColor(renderer,255,255,255,alpha);
            SDL_RenderDrawLine(renderer,player->x + 7,player->y + line_y,player->x+62,player->y + line_y);
            alpha -= 10;
            line_y += 1;
        }
    }

   

}
void update_player(Player* player,float dt){
    player->x += player->vx * dt/10;
    player->y += player->vy * dt/10;

    if(player->x + player->vx + player->size > WIDTH){
        player->x = WIDTH - player->size;
        player->vx = 0;
    }if(player->x < 0){
        player->x = 0;
        player->vx = 0;
    }if(player->y + player->vy + player->size > HEIGHT){
        player->y = HEIGHT - player->size;
        player->vy = 0;
    }if(player->y < 0){
        player->y = 0;
        player->vy = 0;
    }

}
SDL_Texture* load_texture_from_png(SDL_Renderer* renderer,char* filename){
    uint8_t *image;
    int width,height;
    int error = lodepng_decode32_file(&image,&width,&height,filename);

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
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *player_texture = load_texture_from_png(renderer,"nave.png");

    Player player;
    memset(&player,0,sizeof(player));
    player.x = 50;
    player.y = 50;
    player.size = 70;
    player.texture = player_texture;

    Star *stars[45];
    for(int i = 0;i < 15;i++){
        Star *star = malloc(sizeof(Star));
        star->x = rand()%WIDTH;
        star->y = rand()%WIDTH - WIDTH;
        star->vy = 1;
        star->size = 6;
        star->texture = load_texture_from_png(renderer,"star.png");
        stars[i] = star;
    }
    for(int i = 15;i < 30;i++){
        Star *star = malloc(sizeof(Star));
        star->x = rand()%WIDTH;
        star->y = rand()%WIDTH - WIDTH;
        star->vy = 0.6;
        star->size = 4;
        star->texture = load_texture_from_png(renderer,"star.png");
        stars[i] = star;
    }
    for(int i = 30;i < 45;i++){
        Star *star = malloc(sizeof(Star));
        star->x = rand()%WIDTH;
        star->y = rand()%WIDTH - WIDTH;
        star->vy = 0.3;
        star->size = 2;
        star->texture = load_texture_from_png(renderer,"star.png");
        stars[i] = star;
    }
    
    
    float dt = 7;
    bool running = true;
    
    SDL_Event event;
    while(running){
        float begin = SDL_GetTicks();
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:{
                return -1;
            }
        }
        const uint8_t *keyboard = SDL_GetKeyboardState(NULL);

        if(keyboard[SDL_SCANCODE_A]){
            player.vx = -5;
        }if(keyboard[SDL_SCANCODE_D]){
            player.vx = 5;
        }if(keyboard[SDL_SCANCODE_W]){
            player.vy = -5;
        }if(keyboard[SDL_SCANCODE_S]){
            player.vy = 5;
        }
        if(player.vx == 0 && player.vy == 0)
            player.vy = 0.9;

        update_player(&player,dt);
        for(int i = 0;i < 45;i++){
            update_star(stars[i],dt);
        }
        
        SDL_SetRenderDrawColor(renderer,27,58,89,255);
        SDL_RenderClear(renderer);
        
        for(int i = 0;i < 45;i++){
            render_star(renderer,stars[i]);
        }

        render_player(renderer,&player);

        SDL_RenderPresent(renderer);

        dt = SDL_GetTicks() - begin;

        player.vx = 0;
        player.vy = 0;
    }

    return 0;
}






