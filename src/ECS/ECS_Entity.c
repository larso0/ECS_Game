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
		entity->controller = NULL;
		entity->controller_function = NULL;
		entity->controller_data = NULL;
		entity->z = 0;
	}
}

void ECS_SetComponentTranslation(ECS_Entity* entity, float x, float y)
{
	if(entity)
	{
		entity->translation.x = x;
		entity->translation.y = y;
	}
}

void ECS_SetComponentVelocity(ECS_Entity* entity, float x, float y)
{
	if(entity)
	{
		entity->velocity.x = x;
		entity->velocity.y = y;
	}
}

void ECS_SetComponentAcceleration(ECS_Entity* entity, float x, float y)
{
	if(entity)
	{
		entity->acceleration.x = x;
		entity->acceleration.y = y;
	}
}

void ECS_SetComponentAngle(ECS_Entity* entity, float angle)
{
	if(entity)
	{
		entity->angle = angle;
	}
}

void ECS_SetComponentAngularVelocity(ECS_Entity* entity, float velocity)
{
	if(entity)
	{
		entity->angular_velocity = velocity;
	}
}

void ECS_SetComponentAngularAcceleration(ECS_Entity* entity, float acceleration)
{
	if(entity)
	{
		entity->angular_acceleration = acceleration;
	}
}

void ECS_SetComponentSprite(ECS_Entity* entity, ECS_Sprite* sprite, size_t index)
{
	if(entity)
	{
		entity->sprite = sprite;
		entity->sprite_index = index;
	}
}

void ECS_SetComponentAnimation(ECS_Entity* entity, ECS_Animation* animation)
{
	if(entity)
	{
		entity->animation = animation;
	}
}

void ECS_SetComponentCamera(ECS_Entity* entity, int w, int h)
{
	if(entity)
	{
		entity->camera.screen_w = w;
		entity->camera.screen_h = h;
		entity->camera.center_x = w/2;
		entity->camera.center_y = h/2;
	}
}

void ECS_SetComponentController(ECS_Entity* entity, ECS_Controller* controller, ECS_ControllerFunction fn, void* data)
{
	if(entity)
	{
		entity->controller = controller;
		entity->controller_function = fn;
		entity->controller_data = data;
	}
}

void ECS_ToggleComponents(ECS_Entity* entity, ECS_ComponentMask mask)
{
	if(entity)
	{
		entity->mask ^= mask;
	}
}
