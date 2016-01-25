/*
 * ECS_Systems.c
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#include "ECS_Systems.h"

void ECS_ApplyMovement(ECS_Entity* entity, float delta)
{
	if(entity && (entity->mask & ECS_SYSTEM_MOVEMENT) == ECS_SYSTEM_MOVEMENT)
	{
		if(entity->mask & ECS_COMPONENT_ACCELERATION)
		{
			ECS_Vec2 v;
			v.x = entity->velocity.x + entity->acceleration.x * delta;
			v.y = entity->velocity.y + entity->acceleration.y * delta;
			entity->translation.x += ((entity->velocity.x + v.x)/2) * delta;
			entity->translation.y += ((entity->velocity.y + v.y)/2) * delta;
			entity->velocity = v;
		}
		else
		{
			entity->translation.x += entity->velocity.x * delta;
			entity->translation.y += entity->velocity.y * delta;
		}
	}
}

void ECS_ApplyRotation(ECS_Entity* entity, float delta)
{
	if(entity && (entity->mask & ECS_SYSTEM_ROTATION) == ECS_SYSTEM_ROTATION)
	{
		if(entity->mask & ECS_COMPONENT_ANGULAR_ACCELERATION)
		{
			float v;
			v = entity->angular_velocity + entity->angular_acceleration * delta;
			entity->angle += ((entity->angular_velocity + v)/2) * delta;
			entity->angular_velocity = v;
		}
		else
		{
			entity->angle += entity->angular_velocity * delta;
		}
	}
}

void ECS_RenderEntity(ECS_Entity* entity, ECS_Entity* camera, SDL_Renderer* renderer)
{
	if(entity && renderer && (entity->mask & ECS_SYSTEM_RENDERABLE) == ECS_SYSTEM_RENDERABLE)
	{
		SDL_Rect rect;
		rect.w = entity->sprite.rectangles[entity->sprite.current].w;
		rect.h = entity->sprite.rectangles[entity->sprite.current].h;
		rect.x = -rect.w/2;
		rect.y = -rect.h/2;
		if(entity->mask & ECS_COMPONENT_TRANSLATION)
		{
			rect.x += entity->translation.x;
			rect.y -= entity->translation.y;
		}
		if(camera && (camera->mask & ECS_SYSTEM_CAMERA) == ECS_SYSTEM_CAMERA)
		{
			if(camera->mask & ECS_COMPONENT_TRANSLATION)
			{
				rect.x -= camera->translation.x;
				rect.y += camera->translation.y;
			}
			rect.x += camera->camera.center_x;
			rect.y += camera->camera.center_y;
		}
		if(entity->mask & ECS_COMPONENT_ANGLE)
		{
			ECS_RenderSpriteAngle(&entity->sprite, &rect, entity->angle, renderer);
		}
		else
		{
			ECS_RenderSprite(&entity->sprite, &rect, renderer);
		}
	}
}

void ECS_UpdateCamera(ECS_Entity* entity, int w, int h)
{
	if(entity && (entity->mask & ECS_SYSTEM_CAMERA) == ECS_SYSTEM_CAMERA)
	{
		entity->camera.screen_w = w;
		entity->camera.screen_h = h;
		entity->camera.center_x = w/2;
		entity->camera.center_y = h/2;
	}
}
