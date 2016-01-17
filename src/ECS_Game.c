/*
 * ECS_Game.c
 *
 *  Created on: 16. jan. 2016
 *      Author: larso
 */

#include <SDL2/SDL.h>
#include "ECS/ECS.h"

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480
#define GAME_PIXELS_PER_METER 20

static SDL_Window* window;
static SDL_Renderer* renderer;
static ECS_World world;

void ErrorMessage(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message, NULL);
}

void WarningMessage(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", message, NULL);
}

void InformationMessage(const char* message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information", message,
			NULL);
}

int Init()
{
	int error;

	error = SDL_Init(SDL_INIT_VIDEO);
	if (error)
	{
		ErrorMessage(SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("ECS_Game",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		ErrorMessage(SDL_GetError());
		SDL_Quit();
		return 2;
	}

	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		ErrorMessage(SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 3;
	}

	ECS_InitWorld(&world);

	return 0;
}

void Quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char** argv)
{
	int error = Init();
	if (error)
	{
		return error;
	}

	int boxEntity = ECS_CreateEntity(&world);
	world.mask[boxEntity] = ECS_SYSTEM_MOVEMENT;
	float yThrust = 20.f;
	float xThrust = 10.f;

	int running = 1;
	float time = SDL_GetTicks() / 1000.f;
	while (running)
	{
		float ntime = SDL_GetTicks() / 1000.f;
		float delta = ntime - time;
		time = ntime;

		ECS_ComponentVelocity* v = &world.velocity[boxEntity];
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_UP])
		{
			v->y += yThrust * delta;
		}
		if (keys[SDL_SCANCODE_LEFT])
		{
			v->x -= xThrust * delta;
		}
		if (keys[SDL_SCANCODE_RIGHT])
		{
			v->x += xThrust * delta;
		}

		ECS_Update(&world, delta);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		float size = 1.f;
		SDL_Rect box;
		ECS_ComponentPosition* s = &world.position[boxEntity];
		box.x = GAME_WINDOW_WIDTH / 2+ (s->x - size/2)*GAME_PIXELS_PER_METER;
		box.y = GAME_WINDOW_HEIGHT
				/ 2+ ((-s->y) - size/2)*GAME_PIXELS_PER_METER;
		box.w = box.h = size * GAME_PIXELS_PER_METER;
		SDL_RenderFillRect(renderer, &box);

		SDL_RenderPresent(renderer);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			default:
				break;
			}
		}
	}

	Quit();
	return 0;
}
