/*
 * ECS_Entity.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_ENTITY_H_
#define SRC_ECS_ECS_ENTITY_H_

#include "ECS_Components.h"

struct _ECS_Entity;
typedef struct _ECS_Entity ECS_Entity;

typedef void (*ECS_ControllerFunction)(ECS_Controller*, ECS_Entity*, float, void*);

struct _ECS_Entity
{
	int mask;
	ECS_Vec2 translation;
	struct { float w; float h; } size;
	ECS_Vec2 velocity;
	ECS_Vec2 acceleration;
	float angle;
	float angular_velocity;
	float angular_acceleration;
	ECS_Sprite* sprite;
	size_t sprite_index;
	SDL_RendererFlip sprite_flip;
	ECS_Animation* animation;
	float animation_time;
	ECS_Camera camera;
	ECS_Controller* controller;
	ECS_ControllerFunction controller_function;
	void* controller_data;
	int z;
};

void ECS_InitEntity(ECS_Entity* entity);
void ECS_SetComponentTranslation(ECS_Entity* entity, float x, float y);
void ECS_SetComponentSize(ECS_Entity* entity, float w, float h);
void ECS_SetComponentSizeFromSprite(ECS_Entity* entity);
void ECS_SetComponentVelocity(ECS_Entity* entity, float x, float y);
void ECS_SetComponentAcceleration(ECS_Entity* entity, float x, float y);
void ECS_SetComponentAngle(ECS_Entity* entity, float angle);
void ECS_SetComponentAngularVelocity(ECS_Entity* entity, float velocity);
void ECS_SetComponentAngularAcceleration(ECS_Entity* entity, float acceleration);
void ECS_SetComponentSprite(ECS_Entity* entity, ECS_Sprite* sprite, size_t index);
void ECS_SetComponentAnimation(ECS_Entity* entity, ECS_Animation* animation);
void ECS_SetComponentCamera(ECS_Entity* entity, int w, int h);
void ECS_SetComponentController(ECS_Entity* entity, ECS_Controller* controller, ECS_ControllerFunction fn, void* data);
void ECS_EnableComponents(ECS_Entity* entity, ECS_ComponentMask mask);
void ECS_DisableComponents(ECS_Entity* entity, ECS_ComponentMask mask);
void ECS_ToggleComponents(ECS_Entity* entity, ECS_ComponentMask mask);
void ECS_CopyEntity(ECS_Entity* src, ECS_Entity* dst);

#endif /* SRC_ECS_ECS_ENTITY_H_ */
