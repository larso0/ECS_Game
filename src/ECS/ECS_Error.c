/*
 * ECS_Error.c
 *
 *  Created on: 25. jan. 2016
 *      Author: larso
 */

#include "ECS_Error.h"
#include <SDL2/SDL.h>

void ECS_ErrorMessage(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message, NULL);
}

void ECS_WarningMessage(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", message, NULL);
}

void ECS_InformationMessage(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message,
			NULL);
}
