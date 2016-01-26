/*
 * ECS_Game.c
 *
 *  Created on: 16. jan. 2016
 *      Author: larso
 */

#include <SDL2/SDL.h>
#include "ECS/ECS_Error.h"
#include "ECS/ECS_Systems.h"

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480
#define GAME_ENTITY_COUNT 10
#define GAME_ENTITY_CAMERA 0
#define GAME_ENTITY_CHARACTER 1

static SDL_Window* window;
static SDL_Renderer* renderer;
static ECS_Entity entities[GAME_ENTITY_COUNT];
static enum { IDLE_ANIM, WALK_ANIM, RUN_ANIM } animation = IDLE_ANIM;
static const ECS_Animation IDLE_ANIMATION = { 0.f, 0.2f, 0, 4 };
static const ECS_Animation WALK_ANIMATION = { 0.f, 0.1f, 24, 8 };
static const ECS_Animation RUN_ANIMATION = { 0.f, 0.1f, 4, 8 };

int Init()
{
	int error;

	error = SDL_Init(SDL_INIT_VIDEO);
	if (error)
	{
		ECS_ErrorMessage(SDL_GetError());
		return 1;
	}

	int imgmask = IMG_INIT_JPG | IMG_INIT_PNG;
	int imginit = IMG_Init(imgmask);
	if((imginit & imginit) != imginit)
	{
		ECS_ErrorMessage(IMG_GetError());
		SDL_Quit();
		return 2;
	}

	window = SDL_CreateWindow("ECS_Game",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 0);
	if (window == NULL)
	{
		ECS_ErrorMessage(SDL_GetError());
		SDL_Quit();
		return 3;
	}

	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		ECS_ErrorMessage(SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 4;
	}

	int i;
	for(i = 0; i < GAME_ENTITY_COUNT; i++)
	{
		ECS_InitEntity(entities + i);
	}

	return 0;
}

void InitEntities()
{
	entities[GAME_ENTITY_CAMERA].mask =
			ECS_COMPONENT_CAMERA |
			ECS_COMPONENT_TRANSLATION;
	entities[GAME_ENTITY_CHARACTER].mask =
			ECS_COMPONENT_TRANSLATION |
			ECS_COMPONENT_VELOCITY |
			ECS_COMPONENT_ACCELERATION |
			ECS_COMPONENT_ANGLE |
			ECS_COMPONENT_SPRITE |
			ECS_COMPONENT_ANIMATION;
	ECS_LoadSprite(
			&entities[GAME_ENTITY_CHARACTER].sprite,
			"resources/character_sprite.png",
			"resources/character_sprite.meta",
			renderer);
	entities[GAME_ENTITY_CHARACTER].animation = IDLE_ANIMATION;

	ECS_UpdateCamera(entities + GAME_ENTITY_CAMERA, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
}

void Quit()
{
	int i;
	for(i = 0; i < GAME_ENTITY_COUNT; i++)
	{
		ECS_CleanEntity(entities + i);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char** argv)
{
	int error = Init();
	if (error)
	{
		return error;
	}

	InitEntities();

	int running = 1;
	float time = SDL_GetTicks() / 1000.f;
	while (running)
	{
		float ntime = SDL_GetTicks() / 1000.f;
		float delta = ntime - time;
		time = ntime;

		ECS_ApplyAnimation(entities + GAME_ENTITY_CHARACTER, delta);
		ECS_ApplyMovement(entities + GAME_ENTITY_CHARACTER, delta);

		const Uint8* keys = SDL_GetKeyboardState(NULL);
		if(keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT])
		{
			if(keys[SDL_SCANCODE_LALT])
			{
				if(animation != RUN_ANIM)
				{
					entities[GAME_ENTITY_CHARACTER].animation = RUN_ANIMATION;
					animation = RUN_ANIM;
				}
				entities[GAME_ENTITY_CHARACTER].velocity.x = -100.f;
			}
			else
			{
				if(animation != WALK_ANIM)
				{
					entities[GAME_ENTITY_CHARACTER].animation = WALK_ANIMATION;
					animation = WALK_ANIM;
				}
				entities[GAME_ENTITY_CHARACTER].velocity.x = -50.f;
			}
			entities[GAME_ENTITY_CHARACTER].sprite.flip = SDL_FLIP_HORIZONTAL;
		}
		else if(!keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT])
		{
			if(keys[SDL_SCANCODE_LALT])
			{
				if(animation != RUN_ANIM)
				{
					entities[GAME_ENTITY_CHARACTER].animation = RUN_ANIMATION;
					animation = RUN_ANIM;
				}
				entities[GAME_ENTITY_CHARACTER].velocity.x = 100.f;
			}
			else
			{
				if(animation != WALK_ANIM)
				{
					entities[GAME_ENTITY_CHARACTER].animation = WALK_ANIMATION;
					animation = WALK_ANIM;
				}
				entities[GAME_ENTITY_CHARACTER].velocity.x = 50.f;
			}
			entities[GAME_ENTITY_CHARACTER].sprite.flip = SDL_FLIP_NONE;
		}
		else
		{
			if(animation != IDLE_ANIM)
			{
				entities[GAME_ENTITY_CHARACTER].animation = IDLE_ANIMATION;
				animation = IDLE_ANIM;
			}
			entities[GAME_ENTITY_CHARACTER].velocity.x = 0.f;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		ECS_RenderEntity(
				entities + GAME_ENTITY_CHARACTER,
				entities + GAME_ENTITY_CAMERA,
				renderer);

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
