#pragma once
#include<SDL2/SDL.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct{
    float x,y,vx,vy;
    float size;
    float angle;
    SDL_Texture* texture;
}Bullet;

typedef struct _Node{
    struct _Node* next;
    Bullet* bullet;
}Node;

typedef struct{
    Node* head;
    Node* tail;
}LinkedList;

bool compare_bullets(Bullet* bullet1,Bullet* bullet2){
    if(bullet1->x == bullet2->x && bullet1->y == bullet2->y){
        return true;
    }
    return false;
}

extern inline void append(LinkedList* this,Bullet* bullet){
    Node* node = malloc(sizeof(Node));
    node->bullet = bullet;
    node->next = NULL;
    if(this->head == NULL){
        this->head = node;
        this->tail = node;
        return;
    }
    if(this->head->next == NULL){
        this->head->next = node;
        this->tail = node;
        return;
    }
    this->tail->next = node;
    this->tail = node;
    return;
}

extern inline Node* delete(LinkedList* this,Bullet* bullet){
    if(this->head == NULL){
        return NULL;
    }
    Node* prev_node = this->head;
    Node* current_node = this->head->next;
    if(compare_bullets(this->head->bullet,bullet)){
        this->head = this->head->next;
        return this->head;
    }
    while(current_node != NULL){
        if(compare_bullets(current_node->bullet,bullet)){
            prev_node->next = current_node->next;
            return NULL;
        }else{
            prev_node = current_node;
            current_node = current_node->next;
        }
    }
    return NULL;
}

extern inline char* to_string(LinkedList* this){
    char* res = calloc(200,1);
    if(this->head == NULL){
        strcpy(res,"void");
        return res;
    }
    Node* current_node = this->head;
    while(current_node->next != NULL){
        char* tmp = calloc(200,1);
        sprintf(tmp,"Node %f -> ",current_node->bullet->y);
        strcat(res,tmp);
        current_node = current_node->next;
        free(tmp);
    }
    char* tmp = calloc(200,1);
    sprintf(tmp,"Node: %f",this->tail->bullet->y);
    strcat(res,tmp);
    free(tmp);
    return res;
}
