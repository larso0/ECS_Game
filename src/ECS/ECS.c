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

static inline void ECS_SwapEntities(ECS_Entity* a, ECS_Entity* b)
{
	ECS_Entity tmp;
	ECS_CopyEntity(a, &tmp);
	ECS_CopyEntity(b, a);
	ECS_CopyEntity(&tmp, b);
}

static inline size_t ECS_QSPartition(ECS_Entity* es, size_t lo, size_t hi)
{
	ECS_Entity pivot;
	ECS_CopyEntity(es + hi, &pivot);
	size_t i = lo, j = lo;
	for(; j < hi; j++)
	{
		if(es[j].z < pivot.z)
		{
			ECS_SwapEntities(es + i, es + j);
			i++;
		}
	}
	ECS_SwapEntities(es + i, es + hi);
	return i;
}

static inline void ECS_QSEntities(ECS_Entity* es, size_t lo, size_t hi)
{
	if(lo < hi)
	{
		size_t p = ECS_QSPartition(es, lo, hi);
		ECS_QSEntities(es, lo, p - 1);
		ECS_QSEntities(es, p + 1, hi);
	}
}

void ECS_SortEntities(ECS_Entity* entities, size_t count)
{
	if(entities && count > 0)
	{
		ECS_QSEntities(entities, 0, count - 1);
	}
}
