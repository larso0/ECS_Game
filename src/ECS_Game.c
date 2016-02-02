/*
 * ECS_Game.c
 *
 *  Created on: 16. jan. 2016
 *      Author: larso
 */

#include <SDL2/SDL.h>
#include "ECS/ECS_Error.h"
#include "ECS/ECS_Systems.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define ENTITY_COUNT 10
#define ENTITY_CAMERA 1
#define ENTITY_CHARACTER 1
#define ENTITY_STATIC_CHARACTER 2
#define WALK_SPEED 2.f
#define RUN_SPEED 4.f

static SDL_Window* window;
static SDL_Renderer* renderer;
static ECS_Entity entities[ENTITY_COUNT];
static ECS_Sprite character_sprite;
static ECS_Animation idle_animation = { 0.2f, 0, 4 };
static ECS_Animation walk_animation = { 0.1f, 24, 8 };
static ECS_Animation run_animation = { 0.1f, 4, 8 };
static ECS_Controller controller;

enum
{
	CONTROLLER_LEFT,
	CONTROLLER_RIGHT,
	CONTROLLER_RUN
};

typedef struct
{
	enum { IDLE_ANIM, WALK_ANIM, RUN_ANIM } animation;
	ECS_Animation* idle_animation;
	ECS_Animation* walk_animation;
	ECS_Animation* run_animation;
} PlayerData;

static PlayerData pd = {
		IDLE_ANIM,
		&idle_animation,
		&walk_animation,
		&run_animation
};

void ControllerFn(ECS_Controller* controller, ECS_Entity* entity, float delta, void* data)
{
	PlayerData* pd = (PlayerData*)data;
	SDL_Scancode left = ECS_GetControllerKey(controller, CONTROLLER_LEFT);
	SDL_Scancode right = ECS_GetControllerKey(controller, CONTROLLER_RIGHT);
	SDL_Scancode run = ECS_GetControllerKey(controller, CONTROLLER_RUN);
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[left] && !keys[right])
	{
		if (keys[run])
		{
			if (pd->animation != RUN_ANIM)
			{
				entity->animation = pd->run_animation;
				pd->animation = RUN_ANIM;
			}
			entity->velocity.x = -RUN_SPEED;
		}
		else
		{
			if (pd->animation != WALK_ANIM)
			{
				entity->animation = pd->walk_animation;
				pd->animation = WALK_ANIM;
			}
			entity->velocity.x = -WALK_SPEED;
		}
		entity->sprite_flip = SDL_FLIP_HORIZONTAL;
	}
	else if (!keys[left] && keys[right])
	{
		if (keys[run])
		{
			if (pd->animation != RUN_ANIM)
			{
				entity->animation = pd->run_animation;
				pd->animation = RUN_ANIM;
			}
			entity->velocity.x = RUN_SPEED;
		}
		else
		{
			if (pd->animation != WALK_ANIM)
			{
				entity->animation = pd->walk_animation;
				pd->animation = WALK_ANIM;
			}
			entity->velocity.x = WALK_SPEED;
		}
		entity->sprite_flip = SDL_FLIP_NONE;
	}
	else
	{
		if (pd->animation != IDLE_ANIM)
		{
			entity->animation = pd->idle_animation;
			pd->animation = IDLE_ANIM;
		}
		entity->velocity.x = 0.f;
	}
}

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
	WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

	ECS_InitSprite(&character_sprite);
	ECS_LoadSprite(&character_sprite,
			"resources/character_sprite.png",
			"resources/character_sprite.meta", renderer);
	ECS_InitController(&controller, 3);
	ECS_BindKey(&controller, SDL_SCANCODE_LEFT, CONTROLLER_LEFT);
	ECS_BindKey(&controller, SDL_SCANCODE_RIGHT, CONTROLLER_RIGHT);
	ECS_BindKey(&controller, SDL_SCANCODE_LALT, CONTROLLER_RUN);

	int i;
	for(i = 0; i < ENTITY_COUNT; i++)
	{
		ECS_InitEntity(entities + i);
	}

	return 0;
}

void InitEntities()
{
	entities[ENTITY_CAMERA].mask =
			ECS_COMPONENT_CAMERA |
			ECS_COMPONENT_TRANSLATION;
	entities[ENTITY_CHARACTER].mask |=
			ECS_COMPONENT_TRANSLATION |
			ECS_COMPONENT_VELOCITY |
			ECS_COMPONENT_ACCELERATION |
			ECS_COMPONENT_SPRITE |
			ECS_COMPONENT_ANIMATION |
			ECS_COMPONENT_CONTROLLER;
	entities[ENTITY_CHARACTER].sprite = &character_sprite;
	entities[ENTITY_CHARACTER].animation = &idle_animation;
	entities[ENTITY_CHARACTER].controller = &controller;
	entities[ENTITY_CHARACTER].controller_function = ControllerFn;
	entities[ENTITY_CHARACTER].controller_data = (void*)&pd;
	entities[ENTITY_STATIC_CHARACTER].mask =
			ECS_COMPONENT_TRANSLATION |
			ECS_COMPONENT_ANGLE |
			ECS_COMPONENT_ANGULAR_VELOCITY |
			ECS_COMPONENT_SPRITE |
			ECS_COMPONENT_ANIMATION;
	entities[ENTITY_STATIC_CHARACTER].sprite = &character_sprite;
	entities[ENTITY_STATIC_CHARACTER].animation = &idle_animation;
	entities[ENTITY_STATIC_CHARACTER].translation.x = 2.f;
	entities[ENTITY_STATIC_CHARACTER].translation.y = 2.f;
	entities[ENTITY_STATIC_CHARACTER].angular_velocity = 45.f;

	ECS_UpdateCamera(entities + ENTITY_CAMERA, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Quit()
{
	ECS_CleanController(&controller);
	ECS_CleanSprite(&character_sprite);
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

		ECS_ApplyAnimation(entities + ENTITY_CHARACTER, delta);
		ECS_ApplyAnimation(entities + ENTITY_STATIC_CHARACTER, delta);
		ECS_ApplyMovement(entities + ENTITY_CHARACTER, delta);
		ECS_ApplyRotation(entities + ENTITY_STATIC_CHARACTER, delta);
		ECS_UpdateController(entities + ENTITY_CHARACTER, delta);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		ECS_RenderEntity(
				entities + ENTITY_CHARACTER,
				entities + ENTITY_CAMERA,
				renderer);
		ECS_RenderEntity(
				entities + ENTITY_STATIC_CHARACTER,
				entities + ENTITY_CAMERA,
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
