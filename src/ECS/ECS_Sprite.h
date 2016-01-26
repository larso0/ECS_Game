/*
 * ECS_Sprite.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_SPRITE_H_
#define SRC_ECS_ECS_SPRITE_H_

#include <SDL2/SDL_image.h>

typedef struct
{
	SDL_Texture* texture;
	SDL_Rect* rectangles;
	size_t rectangle_count;
	size_t current;
	SDL_RendererFlip flip;
} ECS_Sprite;

void ECS_InitSprite(ECS_Sprite* sprite);
void ECS_CleanSprite(ECS_Sprite* sprite);
ECS_Sprite* ECS_CreateSprite(char* img, char* meta, SDL_Renderer* renderer);
void ECS_DestroySprite(ECS_Sprite* sprite);
int ECS_LoadSprite(ECS_Sprite* sprite, char* img, char* meta, SDL_Renderer* renderer);
void ECS_RenderSprite(ECS_Sprite* sprite, SDL_Rect* dst, SDL_Renderer* renderer);
void ECS_RenderSpriteAngle(ECS_Sprite* sprite, SDL_Rect* dst, float angle, SDL_Renderer* renderer);

typedef struct
{
	float time;
	float delta;
	size_t begin;
	size_t count;
} ECS_Animation;

#endif /* SRC_ECS_ECS_SPRITE_H_ */
