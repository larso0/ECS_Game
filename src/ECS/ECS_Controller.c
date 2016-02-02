/*
 * ECS_Controller.c
 *
 *  Created on: 2. feb. 2016
 *      Author: larso
 */

#include "ECS_Controller.h"

void ECS_InitController(ECS_Controller* controller, int n)
{
	if(controller && n > 0)
	{
		controller->key_binding_count = n;
		controller->key_bindings = malloc(sizeof(SDL_Scancode)*n);
	}
}

void ECS_CleanController(ECS_Controller* controller)
{
	if(controller && controller->key_bindings) free(controller->key_bindings);
}

ECS_Controller* ECS_CreateController(int n)
{
	ECS_Controller* controller = NULL;
	if(n > 0)
	{
		controller = malloc(sizeof(ECS_Controller));
		if(controller)
		{
			controller->key_binding_count = n;
			controller->key_bindings = malloc(sizeof(SDL_Scancode)*n);
			if(!controller->key_bindings)
			{
				free(controller);
				controller = NULL;
			}
		}
	}
	return controller;
}

void ECS_DestroyController(ECS_Controller* controller)
{
	if(controller)
	{
		if(controller->key_bindings) free(controller->key_bindings);
		free(controller);
	}
}

int ECS_BindKey(ECS_Controller* controller, SDL_Scancode key, int index)
{
	if(controller)
	{
		if(index >= 0 && index < controller->key_binding_count)
		{
			int i = ECS_GetControllerIndex(controller, key);
			if(i >= 0)
			{
				return i;
			}
			else
			{
				controller->key_bindings[index] = key;
				return index;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -2;
	}
}

SDL_Scancode ECS_GetControllerKey(ECS_Controller* controller, int index)
{
	if(controller && index >= 0 && index < controller->key_binding_count)
	{
		return controller->key_bindings[index];
	}
	else
	{
		return SDL_SCANCODE_UNKNOWN;
	}
}

int ECS_GetControllerIndex(ECS_Controller* controller, SDL_Scancode key)
{
	if(controller)
	{
		int i;
		for(i = 0; i < controller->key_binding_count && controller->key_bindings[i] != key; i++);
		if(i == controller->key_binding_count) return -1;
		else return i;
	}
	else
	{
		return -2;
	}
}
