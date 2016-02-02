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
		entity->sprite = NULL;
		entity->sprite_index = 0;
		entity->sprite_flip = SDL_FLIP_NONE;
		entity->animation = NULL;
		entity->animation_time = 0.f;
		entity->camera.screen_w = 0;
		entity->camera.screen_h = 0;
		entity->camera.center_x = 0;
		entity->camera.center_y = 0;
	}
}
