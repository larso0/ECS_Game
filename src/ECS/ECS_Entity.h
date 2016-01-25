/*
 * ECS_Entity.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_ENTITY_H_
#define SRC_ECS_ECS_ENTITY_H_

#include "ECS_Components.h"

typedef struct
{
	int mask;
	ECS_Vec2 translation;
	ECS_Vec2 velocity;
	ECS_Vec2 acceleration;
	float angle;
	float angular_velocity;
	float angular_acceleration;
	ECS_Sprite sprite;
	ECS_Camera camera;
} ECS_Entity;

void ECS_InitEntity(ECS_Entity* entity);
void ECS_CleanEntity(ECS_Entity* entity);

#endif /* SRC_ECS_ECS_ENTITY_H_ */
