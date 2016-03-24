#include "ECS_Systems.h"
#include <math.h>

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
				&rect, (entity->mask & ECS_COMPONENT_ANGLE) ? (-entity->angle) : 0.f,
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

typedef struct { ECS_Vec2 ul, ur, ll, lr; } Hitbox;

static inline Hitbox CalculateHitbox(ECS_Entity* e)
{
    Hitbox hb;
    float halfw = e->size.w/2.f, halfh = e->size.h/2.f;
    hb.ul.x = hb.ll.x = -halfw;
    hb.ur.x = hb.lr.x = halfw;
    hb.ul.y = hb.ur.y = halfh;
    hb.ll.y = hb.lr.y = -halfh;
    if((e->mask & ECS_COMPONENT_ANGLE) == ECS_COMPONENT_ANGLE)
    {
        float c = cos(e->angle), s = sin(e->angle);
        float x, y;
        x = hb.ul.x; y = hb.ul.y;
        hb.ul.x = x*c - y*s; hb.ul.y = x*s + y*c;
        x = hb.ur.x; y = hb.ur.y;
        hb.ur.x = x*c - y*s; hb.ur.y = x*s + y*c;
        x = hb.ll.x; y = hb.ll.y;
        hb.ll.x = x*c - y*s; hb.ll.y = x*s + y*c;
        x = hb.lr.x; y = hb.lr.y;
        hb.lr.x = x*c - y*s; hb.lr.y = x*s + y*c;
    }
    float x = e->translation.x, y = e->translation.y;
    hb.ul.x += x; hb.ul.y += y;
    hb.ur.x += x; hb.ur.y += y;
    hb.ll.x += x; hb.ll.y += y;
    hb.lr.x += x; hb.lr.y += y;
    return hb;
}

static inline void ResolveCollision(ECS_Entity* a, ECS_Entity* b)
{
    //http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604
    Hitbox hba = CalculateHitbox(a);
    Hitbox hbb = CalculateHitbox(b);
    ECS_Vec2 axis1 = {
        hba.ur.x - hba.ul.x,
        hba.ur.y - hba.ul.y
    };
    ECS_Vec2 axis2 = {
        hba.ur.x - hba.lr.x,
        hba.ur.y - hba.lr.y
    };
    ECS_Vec2 axis3 = {
        hbb.ul.x - hbb.ll.x,
        hbb.ul.y - hbb.ll.y
    };
    ECS_Vec2 axis4 = {
        hba.ul.x - hba.ur.x,
        hba.ul.y - hba.ur.y
    };
}

void ECS_CalculateCollision(ECS_Entity* entities, size_t count)
{
    if(entities == NULL) return;
    size_t i, j;
    for(i = 0; i < count - 1; i++)
    {
        ECS_Entity* a = entities + i;
        if((a->mask & ECS_SYSTEM_COLLISION) != ECS_SYSTEM_COLLISION) continue;
        for(j = i + 1; j < count; j++)
        {
            ECS_Entity* b = entities + j;
            if((b->mask & ECS_SYSTEM_COLLISION) != ECS_SYSTEM_COLLISION) continue;
            ResolveCollision(a, b);
        }
    }
}
