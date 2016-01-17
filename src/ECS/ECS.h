/*
 * ECS.h
 *
 *  Created on: 16. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_H_
#define SRC_ECS_ECS_H_

#define ECS_ENTITY_COUNT 1024
#define ECS_GRAVITY 9.81

typedef enum
{
	ECS_COMPONENT_NONE = 0,
	ECS_COMPONENT_POSITION = 1,
	ECS_COMPONENT_VELOCITY = 1 << 1
} ECS_ComponentMask;

typedef enum
{
	ECS_SYSTEM_MOVEMENT = ECS_COMPONENT_POSITION | ECS_COMPONENT_VELOCITY
} ECS_SystemMask;

typedef struct
{
	float x, y;
} ECS_ComponentPosition;

typedef struct
{
	float x, y;
} ECS_ComponentVelocity;

typedef struct
{
	int mask[ECS_ENTITY_COUNT];
	ECS_ComponentPosition position[ECS_ENTITY_COUNT];
	ECS_ComponentVelocity velocity[ECS_ENTITY_COUNT];
} ECS_World;

void ECS_InitWorld(ECS_World* world);
int ECS_CreateEntity(ECS_World* world);
void ECS_DestroyEntity(ECS_World* world, int entity);

void ECS_Update(ECS_World* world, float delta);
void ECS_MovementFunction(ECS_World* world, int entity, float delta);

#endif /* SRC_ECS_ECS_H_ */
