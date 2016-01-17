/*
 * ECS.c
 *
 *  Created on: 16. jan. 2016
 *      Author: larso
 */

#include "ECS.h"

void ECS_InitWorld(ECS_World* world)
{
	if (world)
	{
		int i;
		for (i = 0; i < ECS_ENTITY_COUNT; i++)
		{
			world->mask[i] = ECS_COMPONENT_NONE;
			world->position[i].x = 0.f;
			world->position[i].y = 0.f;
			world->velocity[i].x = 0.f;
			world->velocity[i].y = 0.f;
		}
	}
}

int ECS_CreateEntity(ECS_World* world)
{
	if (world)
	{
		int entity;
		for (entity = 0; entity < ECS_ENTITY_COUNT; entity++)
		{
			if (world->mask[entity] == ECS_COMPONENT_NONE)
			{
				return entity;
			}
		}
		return -1;
	}
	else
	{
		return -2;
	}
}

void ECS_DestroyEntity(ECS_World* world, int entity)
{
	if (world && entity >= 0)
	{
		world->mask[entity] = ECS_COMPONENT_NONE;
		world->position[entity].x = 0.f;
		world->position[entity].y = 0.f;
		world->velocity[entity].x = 0.f;
		world->velocity[entity].y = 0.f;
	}
}

void ECS_Update(ECS_World* world, float delta)
{
	if (world)
	{
		int entity;
		for (entity = 0; entity < ECS_ENTITY_COUNT; entity++)
		{
			if ((world->mask[entity] & ECS_SYSTEM_MOVEMENT)
					== ECS_SYSTEM_MOVEMENT)
			{
				ECS_MovementFunction(world, entity, delta);
			}
		}
	}
}

void ECS_MovementFunction(ECS_World* world, int entity, float delta)
{
	if (world && entity >= 0)
	{
		ECS_ComponentPosition* s = &world->position[entity];
		ECS_ComponentVelocity* v = &world->velocity[entity];
		v->y -= ECS_GRAVITY * delta;
		s->x += v->x * delta;
		s->y += v->y * delta;
	}
}
