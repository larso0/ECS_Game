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
	if(entity && renderer &&
			(entity->mask & ECS_SYSTEM_RENDERABLE) == ECS_SYSTEM_RENDERABLE &&
			entity->sprite && entity->sprite_index < entity->sprite->rectangle_count)
	{
		SDL_Rect rect;
		rect.w = entity->sprite->rectangles[entity->sprite_index].w;
		rect.h = entity->sprite->rectangles[entity->sprite_index].h;
		rect.x = -rect.w/2;
		rect.y = -rect.h/2;
		if(entity->mask & ECS_COMPONENT_TRANSLATION)
		{
			rect.x += entity->translation.x *  ECS_PIXELS_PER_METER;
			rect.y -= entity->translation.y * ECS_PIXELS_PER_METER;
		}
		if(camera && (camera->mask & ECS_SYSTEM_CAMERA) == ECS_SYSTEM_CAMERA)
		{
			if(camera->mask & ECS_COMPONENT_TRANSLATION)
			{
				rect.x -= camera->translation.x * ECS_PIXELS_PER_METER;
				rect.y += camera->translation.y * ECS_PIXELS_PER_METER;
			}
			rect.x += camera->camera.center_x;
			rect.y += camera->camera.center_y;
		}

		ECS_RenderSprite(entity->sprite, entity->sprite_index,
				&rect, (entity->mask & ECS_COMPONENT_ANGLE) ? entity->angle : 0.f,
				entity->sprite_flip, renderer);
	}
}

void ECS_ApplyAnimation(ECS_Entity* entity, float delta)
{
	if(entity && (entity->mask & ECS_SYSTEM_ANIMATION) == ECS_SYSTEM_ANIMATION)
	{
		ECS_Animation* anim = entity->animation;
		entity->animation_time += delta;
		float maxtime = anim->count*anim->delta;
		while(entity->animation_time > maxtime) entity->animation_time -= maxtime;
		entity->sprite_index = anim->begin + (size_t)(entity->animation_time / anim->delta);
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
