#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<math.h>
#include"LinkedList.h"
#include"lodepng.h"
#include"enemy.h"

// #define MAX_BULLETS 100

const int WIDTH =  600;
const int HEIGHT = 600;

const int bullet_delay = 200;

typedef struct{
    float x,y,vx,vy;
    int size;
    SDL_Texture* texture;
    int bullet_quantity;
    LinkedList bullets;
}Player;

SDL_Texture* load_texture_from_png(SDL_Renderer* renderer,char* filename){
    uint8_t *image;
    unsigned int width,height;
    lodepng_decode32_file(&image,&width,&height,filename);

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(image,width,height,32,width*4,
                                0xff000000,
                                0x00ff0000,
                                0x0000ff00,
                                0x000000ff);
    return SDL_CreateTextureFromSurface(renderer,surface);
}

float angle_between_two_vectors2Df(float ux,float uy,float vx,float vy){
    float angle = acosf((ux*vx+uy*vy)/(sqrtf(ux*ux+uy*uy)*sqrtf(vx*vx+vy*vy)));
    //printf("res1: %f",(ux*vx+uy*vy)/(sqrtf(ux*ux+uy*uy)*sqrtf(vx*vx+vy*vy)));
    return angle*(180.f/M_PI);
}

void update_bullets(Player* player,float dt){
    Node* current_node = player->bullets.head;
    while(current_node != NULL){
        current_node->bullet->y += current_node->bullet->vy * dt / 10;
        current_node->bullet->x += current_node->bullet->vx * dt / 10;
        current_node->bullet->angle = angle_between_two_vectors2Df(current_node->bullet->vx,current_node->bullet->vy,1,0);
        if(current_node->bullet->y < 0){
            current_node = delete(&player->bullets,current_node->bullet);
        }else{
            current_node = current_node->next;
        }
    }
}
void render_bullets(SDL_Renderer* renderer,Player* player){
    SDL_Rect src_rect = {0,0,6,6};
    SDL_Point center = {3,3};
    Node* current_node = player->bullets.head;
    while(current_node != NULL){
        SDL_Rect dest_rect = {current_node->bullet->x,current_node->bullet->y,current_node->bullet->size,current_node->bullet->size};
        SDL_RenderCopyEx(renderer,current_node->bullet->texture,&src_rect,&dest_rect,current_node->bullet->angle - 90,&center,SDL_FLIP_NONE);

        current_node = current_node->next;
    }
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

void spawn_normal_bullets(SDL_Renderer* renderer,Player* player,SDL_Texture* bullet_texture){
    Bullet* bullet1 = malloc(sizeof(Bullet));
    bullet1->x = player->x + 4 * 70/32;
    bullet1->y = player->y + 21 * 70/32;
    bullet1->vy = -5;
    bullet1->vx = 0;
    bullet1->angle = 0;
    bullet1->texture = bullet_texture;
    bullet1->size = 6;

    Bullet* bullet2 = malloc(sizeof(Bullet));
    bullet2->x = player->x + 26 * 70/32;
    bullet2->y = player->y + 21 * 70/32;
    bullet2->vy = -5;
    bullet2->vx = 0;
    bullet2->angle = 0;
    bullet2->texture = bullet_texture;
    bullet2->size = 6;

    append(&player->bullets,bullet1);
    append(&player->bullets,bullet2);
}

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
int main(int argc,char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Space Invaders Clone",20,20,600,600,0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* player_texture = load_texture_from_png(renderer,"nave.png");
    SDL_Texture* bullet_texture = load_texture_from_png(renderer,"bullet.png");
    SDL_Texture* enemy1_texture = load_texture_from_png(renderer,"enemy1.png");
    Player player;
    memset(&player,0,sizeof(player));
    player.x = 50;
    player.y = 50;
    player.vy = 0;
    player.vx = 0;
    player.size = 70;
    player.texture = player_texture;
    player.bullet_quantity = 0;
    player.bullets.head = NULL;
    player.bullets.tail = NULL;

    Enemy* enemy = calloc(1,sizeof(Enemy));
    enemy->x = 50;
    enemy->y = 50;
    enemy->vy = 0.5;
    enemy->texture = enemy1_texture;
    enemy->size = 70;
    

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
    
    int current_bullet_delay = bullet_delay;

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
        }if(keyboard[SDL_SCANCODE_SPACE] && current_bullet_delay < 0){
            spawn_normal_bullets(renderer,&player,bullet_texture);
            current_bullet_delay = bullet_delay;
        }
        if(current_bullet_delay < -1){
            current_bullet_delay = -1;
        }
        if(player.vx == 0 && player.vy == 0)
            player.vy = 0.9;

        update_player(&player,dt);
        update_enemy(enemy,dt);
        update_bullets(&player,dt);

        for(int i = 0;i < 45;i++){
            update_star(stars[i],dt);
        }
        
        SDL_SetRenderDrawColor(renderer,27,58,89,255);
        SDL_RenderClear(renderer);
        
        for(int i = 0;i < 45;i++){
            render_star(renderer,stars[i]);
        }

        render_bullets(renderer,&player);

        render_player(renderer,&player);
        render_enemy(renderer,enemy);

        SDL_RenderPresent(renderer);

        dt = SDL_GetTicks() - begin;
        current_bullet_delay -= dt;
        player.vx = 0;
        player.vy = 0;
    }

    return 0;
}






