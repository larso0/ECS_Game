/*
 * ECS_Sprite.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_SPRITE_H_
#define SRC_ECS_ECS_SPRITE_H_

#include <SDL2/SDL_image.h>
#include <stdarg.h>

typedef struct
{
	SDL_Texture* texture;
	SDL_Rect* rectangles;
	size_t rectangle_count;
} ECS_Sprite;

void ECS_InitSprite(ECS_Sprite* sprite);
void ECS_CleanSprite(ECS_Sprite* sprite);
ECS_Sprite* ECS_CreateSprite(char* img, char* meta, SDL_Renderer* renderer);
void ECS_DestroySprite(ECS_Sprite* sprite);
int ECS_LoadSprite(ECS_Sprite* sprite, char* img, char* meta, SDL_Renderer* renderer);
void ECS_RenderSprite(ECS_Sprite* sprite, size_t index, SDL_Rect* dst, float angle, SDL_RendererFlip flip, SDL_Renderer* renderer);

typedef struct
{
	float delta;
	size_t* indices;
	size_t count;
} ECS_Animation;

ECS_Animation* ECS_CreateAnimation(float delta, size_t count, ...);
void ECS_DestroyAnimation(ECS_Animation* animation);

#endif /* SRC_ECS_ECS_SPRITE_H_ */
