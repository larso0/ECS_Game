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
		while(entity->angle > 360.f) entity->angle -= 360.f;
		while(entity->angle < 0.f) entity->angle += 360.f;
	}
}

void ECS_RenderEntity(ECS_Entity* entity, ECS_Entity* camera, SDL_Renderer* renderer)
{
	if(entity && renderer &&
			(entity->mask & ECS_SYSTEM_RENDERABLE) == ECS_SYSTEM_RENDERABLE &&
			entity->sprite && entity->sprite_index < entity->sprite->rectangle_count)
	{
		SDL_Rect rect;
		if(entity->mask & ECS_COMPONENT_SIZE)
		{
			rect.w = entity->size.w * ECS_PIXELS_PER_METER;
			rect.h = entity->size.h * ECS_PIXELS_PER_METER;
		}
		else
		{
			rect.w = entity->sprite->rectangles[entity->sprite_index].w;
			rect.h = entity->sprite->rectangles[entity->sprite_index].h;
		}
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
		entity->sprite_index = anim->indices[(size_t)(entity->animation_time / anim->delta)];
	}
}

void ECS_UpdateController(ECS_Entity* entity, float delta)
{
	if(entity && (entity->mask & ECS_SYSTEM_CONTROLLER) == ECS_SYSTEM_CONTROLLER &&
			entity->controller && entity->controller_function)
	{
		(*entity->controller_function)(entity->controller, entity, delta, entity->controller_data);
	}
}

static inline ECS_Vec2 ECS_ResultPosition(ECS_Entity* e, float delta)
{
	if((e->mask & ECS_COMPONENT_VELOCITY) != ECS_COMPONENT_VELOCITY) return e->translation;
	ECS_Vec2 t = e->translation;
	ECS_Vec2 v = e->velocity;
	ECS_Vec2 a = e->acceleration;
	ECS_Vec2 d;

	d.x = t.x + v.x*delta;
	d.y = t.y + v.y*delta;
	if((e->mask & ECS_COMPONENT_ACCELERATION) == ECS_COMPONENT_ACCELERATION)
	{
		d.x += 0.5*a.x*delta*delta;
		d.y += 0.5*a.y*delta*delta;
	}

	return d;
}

static inline void ECS_ResolveCollosion(ECS_Entity* a, ECS_Entity* b, float delta)
{
	ECS_Vec2 astart = a->translation;
	ECS_Vec2 bstart = b->translation;
	ECS_Vec2 aend = ECS_ResultPosition(a, delta);
	ECS_Vec2 bend = ECS_ResultPosition(b, delta);
}

void ECS_CalculateCollision(ECS_Entity* entities, size_t count, float delta)
{
	if(entities)
	{
		size_t i, j;
		for(i = 0; i < count; i++)
		{
			ECS_Entity* a = entities + i;
			if((a->mask & ECS_SYSTEM_COLLISION) != ECS_SYSTEM_COLLISION) continue;
			for(j = 0; j < count; j++)
			{
				if(j == i) continue;
				ECS_Entity* b = entities + j;
				if((b->mask & ECS_SYSTEM_COLLISION) != ECS_SYSTEM_COLLISION) continue;
				ECS_ResolveCollosion(a, b, delta);
			}
		}
	}
}
