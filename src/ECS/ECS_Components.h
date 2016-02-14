/*
 * ECS_Components.h
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#ifndef SRC_ECS_ECS_COMPONENTS_H_
#define SRC_ECS_ECS_COMPONENTS_H_

#include "ECS_Sprite.h"
#include "ECS_Camera.h"
#include "ECS_Controller.h"

#define ECS_PIXELS_PER_METER 32

typedef enum
{
	ECS_COMPONENT_NONE = 0,
	ECS_COMPONENT_TRANSLATION = 1,
	ECS_COMPONENT_SIZE = 1 << 1,
	ECS_COMPONENT_VELOCITY = 1 << 2,
	ECS_COMPONENT_ACCELERATION = 1 << 3,
	ECS_COMPONENT_ANGLE = 1 << 4,
	ECS_COMPONENT_ANGULAR_VELOCITY = 1 << 5,
	ECS_COMPONENT_ANGULAR_ACCELERATION = 1 << 6,
	ECS_COMPONENT_SPRITE = 1 << 7,
	ECS_COMPONENT_ANIMATION = 1 << 8,
	ECS_COMPONENT_CAMERA = 1 << 9,
	ECS_COMPONENT_CONTROLLER = 1 << 10
} ECS_ComponentMask;

typedef struct
{
	float x, y;
} ECS_Vec2;

#endif /* SRC_ECS_ECS_COMPONENTS_H_ */
