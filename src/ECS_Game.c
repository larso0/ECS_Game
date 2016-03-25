#include "ECS/ECS.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define ENTITY_COUNT 10
#define ENTITY_CAMERA 1
#define ENTITY_CHARACTER 1
#define ENTITY_STATIC_CHARACTER 2
#define ENTITY_BLOCK1 3
#define ENTITY_BLOCK2 4
#define ENTITY_BLOCK3 5
#define WALK_SPEED 2.f
#define RUN_SPEED 4.f

static SDL_Window* window;
static SDL_Renderer* renderer;
static ECS_Entity entities[ENTITY_COUNT];
static ECS_Sprite* character_sprite;
static ECS_Sprite* blocks_sprite;
static ECS_Animation* idle_animation;
static ECS_Animation* walk_animation;
static ECS_Animation* run_animation;
static ECS_Controller* controller;

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

static PlayerData pd;

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

	error = ECS_Init();
	if(error) return error;

	window = SDL_CreateWindow("ECS_Game",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
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

	character_sprite = ECS_CreateSprite("resources/character_sprite.png", "resources/character_sprite.meta", renderer);
	if(character_sprite == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
		return 5;
    }

    blocks_sprite = ECS_CreateSprite("resources/blocks_sprite.bmp", "resources/blocks_sprite.meta", renderer);
	if(character_sprite == NULL)
    {
        ECS_DestroySprite(character_sprite);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
		return 5;
    }

	idle_animation = ECS_CreateAnimation(0.2f, 4, 0, 1, 2, 3);
	if(idle_animation == NULL)
    {
        ECS_DestroySprite(blocks_sprite);
        ECS_DestroySprite(character_sprite);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
		return 6;
    }

	walk_animation = ECS_CreateAnimation(0.1f, 8, 24, 25, 26, 27, 28, 29, 30, 31);
    if(walk_animation == NULL)
    {
        ECS_DestroyAnimation(idle_animation);
        ECS_DestroySprite(blocks_sprite);
        ECS_DestroySprite(character_sprite);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
		return 6;
    }

	run_animation = ECS_CreateAnimation(0.1f, 8, 4, 5, 6, 7, 8, 9, 10, 11);
    if(run_animation == NULL)
    {
        ECS_DestroyAnimation(walk_animation);
        ECS_DestroyAnimation(idle_animation);
        ECS_DestroySprite(blocks_sprite);
        ECS_DestroySprite(character_sprite);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
		return 6;
    }

	controller = ECS_CreateController(3);
    if(controller == NULL)
    {
        ECS_DestroyAnimation(run_animation);
        ECS_DestroyAnimation(walk_animation);
        ECS_DestroyAnimation(idle_animation);
        ECS_DestroySprite(blocks_sprite);
        ECS_DestroySprite(character_sprite);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		SDL_Quit();
		return 7;
    }

	ECS_BindKey(controller, SDL_SCANCODE_LEFT, CONTROLLER_LEFT);
	ECS_BindKey(controller, SDL_SCANCODE_RIGHT, CONTROLLER_RIGHT);
	ECS_BindKey(controller, SDL_SCANCODE_LALT, CONTROLLER_RUN);
	pd.idle_animation = idle_animation;
	pd.walk_animation = walk_animation;
	pd.run_animation = run_animation;
	pd.animation = IDLE_ANIM;

	int i;
	for(i = 0; i < ENTITY_COUNT; i++)
	{
		ECS_InitEntity(entities + i);
	}

	return 0;
}

void InitEntities()
{
	ECS_SetComponentSprite(entities + ENTITY_CHARACTER, character_sprite, 0);
	ECS_SetComponentAnimation(entities + ENTITY_CHARACTER, idle_animation);
	ECS_SetComponentController(entities + ENTITY_CHARACTER, controller, ControllerFn, &pd);
	ECS_EnableComponents(entities + ENTITY_CHARACTER,
			ECS_COMPONENT_TRANSLATION |
			ECS_COMPONENT_VELOCITY |
			ECS_COMPONENT_SPRITE |
			ECS_COMPONENT_ANIMATION |
			ECS_COMPONENT_CONTROLLER);

	ECS_SetComponentCamera(entities + ENTITY_CAMERA, WINDOW_WIDTH, WINDOW_HEIGHT);
	ECS_EnableComponents(entities + ENTITY_CAMERA,
			ECS_COMPONENT_CAMERA | ECS_COMPONENT_TRANSLATION);

	ECS_SetComponentTranslation(entities + ENTITY_STATIC_CHARACTER, 2.f, 2.f);
	ECS_SetComponentAngularVelocity(entities + ENTITY_STATIC_CHARACTER, 45.f);
	ECS_SetComponentSprite(entities + ENTITY_STATIC_CHARACTER, character_sprite, 0);
	ECS_SetComponentAnimation(entities + ENTITY_STATIC_CHARACTER, idle_animation);
	ECS_EnableComponents(entities + ENTITY_STATIC_CHARACTER,
			ECS_COMPONENT_TRANSLATION |
			ECS_COMPONENT_ANGLE |
			ECS_COMPONENT_ANGULAR_VELOCITY |
			ECS_COMPONENT_SPRITE |
			ECS_COMPONENT_ANIMATION |
			ECS_COMPONENT_SIZE);
    ECS_SetComponentTranslation(entities + ENTITY_BLOCK1, -0.25f, -1.25f);
    ECS_SetComponentSprite(entities + ENTITY_BLOCK1, blocks_sprite, 5);
    ECS_EnableComponents(entities + ENTITY_BLOCK1, ECS_COMPONENT_TRANSLATION | ECS_COMPONENT_SPRITE);
    ECS_SetComponentTranslation(entities + ENTITY_BLOCK2, 0.25f, -1.25f);
    ECS_SetComponentSprite(entities + ENTITY_BLOCK2, blocks_sprite, 7);
    ECS_EnableComponents(entities + ENTITY_BLOCK2, ECS_COMPONENT_TRANSLATION | ECS_COMPONENT_SPRITE);
	ECS_SetComponentTranslation(entities + ENTITY_BLOCK3, 3.f, 0.25f);
	ECS_SetComponentSprite(entities + ENTITY_BLOCK3, blocks_sprite, 8);
	ECS_EnableComponents(entities + ENTITY_BLOCK3, ECS_COMPONENT_TRANSLATION | ECS_COMPONENT_SPRITE);
}

void Quit()
{
	ECS_DestroyController(controller);
	ECS_DestroyAnimation(idle_animation);
	ECS_DestroyAnimation(walk_animation);
	ECS_DestroyAnimation(run_animation);
	ECS_DestroySprite(character_sprite);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	ECS_Quit();
}

int main(int argc, char** argv)
{
	int error = Init();
	if (error)
	{
		return error;
	}

	InitEntities();

	int running = 1, test_collision = 0;
	float time = SDL_GetTicks() / 1000.f;
	while (running)
	{
		float ntime = SDL_GetTicks() / 1000.f;
		float delta = ntime - time;
		time = ntime;

		ECS_UpdateEntities(entities, ENTITY_COUNT, delta);
		if(test_collision) ECS_CalculateCollision(entities, ENTITY_COUNT);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		ECS_RenderEntities(entities, ENTITY_COUNT, entities + ENTITY_CAMERA, renderer);

		SDL_RenderPresent(renderer);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED ||
						event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					ECS_SetComponentCamera(entities + ENTITY_CAMERA, event.window.data1, event.window.data2);
				}
				break;
				case SDL_KEYUP:
					if(event.key.keysym.scancode == SDL_SCANCODE_C)
					{
						test_collision = test_collision ? 0:1;
					}
					break;
			default:
				break;
			}
		}
	}

	Quit();
	return 0;
}
