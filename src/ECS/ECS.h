#ifndef SRC_ECS_ECS_H_
#define SRC_ECS_ECS_H_

#include "ECS_Systems.h"
#include "ECS_Error.h"
#include <SDL2/SDL.h>

int ECS_Init();
void ECS_Quit();

void ECS_UpdateEntities(ECS_Entity* entities, size_t count, float delta);
void ECS_RenderEntities(ECS_Entity* entities, size_t count, ECS_Entity* camera, SDL_Renderer* renderer);
void ECS_SortEntities(ECS_Entity* entities, size_t count);

#endif /* SRC_ECS_ECS_H_ */
