/*
 * ECS.c
 *
 *  Created on: 4. feb. 2016
 *      Author: larso
 */

#include "ECS.h"

int ECS_Init()
{
	int error;

	error = SDL_Init(SDL_INIT_VIDEO);
	if (error)
	{
		ECS_ErrorMessage(SDL_GetError());
		return 1;
	}

	int imgmask = IMG_INIT_JPG | IMG_INIT_PNG;
	int imginit = IMG_Init(imgmask);
	if ((imginit & imginit) != imginit)
	{
		ECS_ErrorMessage(IMG_GetError());
		SDL_Quit();
		return 2;
	}

	return 0;
}

void ECS_Quit()
{
	IMG_Quit();
	SDL_Quit();
}

void ECS_UpdateEntities(ECS_Entity* entities, size_t count, float delta)
{
	if(entities)
	{
		size_t i;
		for(i = 0; i < count; i++)
		{
			ECS_ApplyMovement(entities + i, delta);
			ECS_ApplyRotation(entities + i, delta);
			ECS_ApplyAnimation(entities + i, delta);
			ECS_UpdateController(entities + i, delta);
		}
	}
}

void ECS_RenderEntities(ECS_Entity* entities, size_t count, ECS_Entity* camera, SDL_Renderer* renderer)
{
	if(entities && renderer)
	{
		size_t i;
		for(i = 0; i < count; i++)
		{
			ECS_RenderEntity(entities + i, camera, renderer);
		}
	}
}
