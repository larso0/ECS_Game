/*
 * ECS_Systems.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_SYSTEMS_H_
#define SRC_ECS_ECS_SYSTEMS_H_

#include "ECS_Entity.h"

typedef enum
{
	ECS_SYSTEM_MOVEMENT = ECS_COMPONENT_TRANSLATION | ECS_COMPONENT_VELOCITY,
	ECS_SYSTEM_ROTATION = ECS_COMPONENT_ANGLE | ECS_COMPONENT_ANGULAR_VELOCITY,
	ECS_SYSTEM_RENDERABLE = ECS_COMPONENT_SPRITE,
	ECS_SYSTEM_ANIMATION = ECS_SYSTEM_RENDERABLE | ECS_COMPONENT_ANIMATION,
	ECS_SYSTEM_CAMERA = ECS_COMPONENT_CAMERA,
	ECS_SYSTEM_CONTROLLER = ECS_COMPONENT_CONTROLLER,
	ECS_SYSTEM_COLLISION = ECS_COMPONENT_TRANSLATION | ECS_COMPONENT_SIZE | ECS_COMPONENT_HITBOX
} ECS_SystemMask;

void ECS_ApplyMovement(ECS_Entity* entity, float delta);
void ECS_ApplyRotation(ECS_Entity* entity, float delta);
void ECS_RenderEntity(ECS_Entity* entity, ECS_Entity* camera, SDL_Renderer* renderer);
void ECS_ApplyAnimation(ECS_Entity* entity, float delta);
void ECS_UpdateController(ECS_Entity* entity, float delta);
void ECS_CalculateCollision(ECS_Entity* entities, size_t count, float delta);

#endif /* SRC_ECS_ECS_SYSTEMS_H_ */
