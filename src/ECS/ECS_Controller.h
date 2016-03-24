#ifndef SRC_ECS_ECS_CONTROLLER_H_
#define SRC_ECS_ECS_CONTROLLER_H_

#include <SDL2/SDL.h>

typedef struct
{
	SDL_Scancode* key_bindings;
	size_t key_binding_count;
} ECS_Controller;

void ECS_InitController(ECS_Controller* controller, int n);
void ECS_CleanController(ECS_Controller* controller);

ECS_Controller* ECS_CreateController(int n);
void ECS_DestroyController(ECS_Controller* controller);

/*
 * Binds a key scancode to a controller index if not already bound to another index.
 * Returns the index that the key is bound to if there was no error.
 * Errors:
 * Returns -1 if the index was out of bounds.
 * Returns -2 if the controller pointer was NULL.
 */
int ECS_BindKey(ECS_Controller* controller, SDL_Scancode key, int index);

/*
 * Unbinds a controller index so that no key is bound to the index.
 * Same as using ECS_BindKey to bind SDL_SCANCODE_UNKNOWN, but with no error code returned.
 */
void ECS_UnBindKey(ECS_Controller* controller, size_t index);

/*
 * Gets the key scancode at a controller index.
 * Returns SDL_SCANCODE_UNKNOWN if the key is unbound or there was an error.
 */
SDL_Scancode ECS_GetControllerKey(ECS_Controller* controller, int index);

/*
 * Gets the index a particular key scancode is bound to.
 * Returns -1 if the key is not bound to any controller index.
 * Errors:
 * Returns -2 if the controller pointer was NULL.
 */
int ECS_GetControllerIndex(ECS_Controller* controller, SDL_Scancode key);

#endif /* SRC_ECS_ECS_CONTROLLER_H_ */
