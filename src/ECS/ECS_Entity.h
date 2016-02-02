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
};

void ECS_InitEntity(ECS_Entity* entity);

#endif /* SRC_ECS_ECS_ENTITY_H_ */
