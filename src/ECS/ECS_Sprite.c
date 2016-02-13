/*
 * ECS_Sprite.c
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#include "ECS_Sprite.h"
#include "ECS_Error.h"
#include <stdio.h>

void ECS_InitSprite(ECS_Sprite* sprite)
{
	if(sprite)
	{
		sprite->texture = NULL;
		sprite->rectangles = NULL;
		sprite->rectangle_count = 0;
	}
}

void ECS_CleanSprite(ECS_Sprite* sprite)
{
	if(sprite)
	{
		if(sprite->texture)
		{
			SDL_DestroyTexture(sprite->texture);
		}
		if(sprite->rectangles)
		{
			free(sprite->rectangles);
		}
	}
}

ECS_Sprite* ECS_CreateSprite(char* img, char* meta, SDL_Renderer* renderer)
{
	ECS_Sprite* sprite = malloc(sizeof(ECS_Sprite));
	if(sprite)
	{
		ECS_InitSprite(sprite);
		int error = ECS_LoadSprite(sprite, img, meta, renderer);
		if(error)
		{
			free(sprite);
			sprite = NULL;
		}
	}
	else
	{
		ECS_ErrorMessage("Could not allocate memory for sprite.");
	}
	return sprite;
}

void ECS_DestroySprite(ECS_Sprite* sprite)
{
	if(sprite)
	{
		ECS_CleanSprite(sprite);
		free(sprite);
	}
}

int ECS_LoadSprite(ECS_Sprite* sprite, char* img, char* meta, SDL_Renderer* renderer)
{
	if(!sprite)
	{
		ECS_ErrorMessage("Invalid argument. Recieved NULL instead of sprite.");
		return -1;
	}
	if(!img)
	{
		ECS_ErrorMessage("Invalid argument. Recieved NULL instead of sprite image file name.");
		return -2;
	}
	if(!meta)
	{
		ECS_ErrorMessage("Invalid argument. Recieved NULL instead of sprite meta file name.");
		return -3;
	}
	if(!renderer)
	{
		ECS_ErrorMessage("Invalid argument. Recieved NULL instead of renderer.");
		return -4;
	}

	SDL_Surface* surf = IMG_Load(img);
    if(!surf)
    {
    	ECS_ErrorMessage("Could not load sprite image file.");
    	return -5;
    }
    sprite->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if(!sprite->texture)
    {
    	ECS_ErrorMessage("Could not create texture for sprite.");
    	return -6;
    }

    FILE* file = fopen(meta, "r");
    if(!file)
    {
    	SDL_DestroyTexture(sprite->texture);
    	ECS_ErrorMessage("Could not open meta file.");
    	return -7;
    }

    SDL_Rect rect;
    int ret = 0;
    int res = fscanf(file, "%d %d %d %d\n", &rect.x, &rect.y, &rect.w, &rect.h);
    while(res != EOF)
    {
    	if(res < 4)
    	{
    		ECS_WarningMessage("Invalid sprite meta file format, skipping line.");
    		ret = 1;
    	}
    	else
    	{
    		SDL_Rect* tmp = realloc(sprite->rectangles, sizeof(SDL_Rect)*(sprite->rectangle_count + 1));
    		if(!tmp)
    		{
    			fclose(file);
    			ECS_WarningMessage("Could not allocate memory for sprite rectangles, skipping rest of file.");
    			return 2;
    		}
    		sprite->rectangles = tmp;
    		sprite->rectangles[sprite->rectangle_count++] = rect;
    	}
        res = fscanf(file, "%d %d %d %d\n", &rect.x, &rect.y, &rect.w, &rect.h);
    }
    fclose(file);
	return ret;
}

void ECS_RenderSprite(ECS_Sprite* sprite, size_t index, SDL_Rect* dst, float angle, SDL_RendererFlip flip, SDL_Renderer* renderer)
{
	if(sprite && renderer && index < sprite->rectangle_count)
	{
		SDL_RenderCopyEx(renderer, sprite->texture,
				sprite->rectangles + index,
				dst, angle, NULL, flip);
	}
}

ECS_Animation* ECS_CreateAnimation(float delta, size_t count, ...)
{
	ECS_Animation* anim = malloc(sizeof(ECS_Animation));
	if(anim)
	{
		va_list args;
		anim->indices = malloc(sizeof(size_t)*count);
		if(anim->indices)
		{
			va_start(args, count);
			size_t i;
			for(i = 0; i < count; i++)
			{
				anim->indices[i] = va_arg(args, size_t);
			}
			va_end(args);
			anim->count = count;
			anim->delta = delta;
		}
		else
		{
			free(anim);
			anim = NULL;
		}
	}
	return anim;
}

void ECS_DestroyAnimation(ECS_Animation* animation)
{
	if(animation)
	{
		free(animation->indices);
		free(animation);
	}
}
