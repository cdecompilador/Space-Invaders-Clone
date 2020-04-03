#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
//hello
void draw_triangle(SDL_Renderer* renderer){
    for(int y = 0;y<600;y++){
        for(int x = 0;x<600;x++){
            if(y % (33*6) == 0 || x % (33*6) == 0){
                SDL_RenderDrawPoint(renderer,x,y);
            }
            
        }
    }
}

int main(int argc,char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Hola SDL",20,20,600,600,0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

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
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,0,255,0,255);

        draw_triangle(renderer);

        SDL_RenderPresent(renderer);
    }

    return 0;
}






