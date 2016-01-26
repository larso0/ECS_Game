/*
 * ECS_Entity.c
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#include "ECS_Entity.h"

void ECS_InitEntity(ECS_Entity* entity)
{
	if(entity)
	{
		entity->mask = ECS_COMPONENT_NONE;
		entity->translation.x = 0.f;
		entity->translation.y = 0.f;
		entity->velocity.x = 0.f;
		entity->velocity.y = 0.f;
		entity->acceleration.x = 0.f;
		entity->acceleration.y = 0.f;
		entity->angle = 0.f;
		entity->angular_velocity = 0.f;
		entity->angular_acceleration = 0.f;
		ECS_InitSprite(&entity->sprite);
		entity->animation.begin = 0;
		entity->animation.count = 0;
		entity->animation.time = 0.f;
		entity->animation.delta = 0.1f;
		entity->camera.screen_w = 0;
		entity->camera.screen_h = 0;
		entity->camera.center_x = 0;
		entity->camera.center_y = 0;
	}
}

void ECS_CleanEntity(ECS_Entity* entity)
{
	if(entity)
	{
		if(entity->mask & ECS_COMPONENT_SPRITE)
		{
			ECS_CleanSprite(&entity->sprite);
		}
		ECS_InitEntity(entity);
	}
}
