/*
 * ECS_Systems.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_SYSTEMS_H_
#define SRC_ECS_ECS_SYSTEMS_H_

#include "ECS_Entity.h"

#define ECS_PIXELS_PER_METER 40

typedef enum
{
	ECS_SYSTEM_MOVEMENT = ECS_COMPONENT_TRANSLATION | ECS_COMPONENT_VELOCITY,
	ECS_SYSTEM_ROTATION = ECS_COMPONENT_ANGLE | ECS_COMPONENT_ANGULAR_VELOCITY,
	ECS_SYSTEM_RENDERABLE = ECS_COMPONENT_SPRITE,
	ECS_SYSTEM_ANIMATION = ECS_SYSTEM_RENDERABLE | ECS_COMPONENT_ANIMATION,
	ECS_SYSTEM_CAMERA = ECS_COMPONENT_CAMERA
} ECS_SystemMask;

void ECS_ApplyMovement(ECS_Entity* entity, float delta);
void ECS_ApplyRotation(ECS_Entity* entity, float delta);
void ECS_RenderEntity(ECS_Entity* entity, ECS_Entity* camera, SDL_Renderer* renderer);
void ECS_ApplyAnimation(ECS_Entity* entity, float delta);
void ECS_UpdateCamera(ECS_Entity* entity, int w, int h);

#endif /* SRC_ECS_ECS_SYSTEMS_H_ */
