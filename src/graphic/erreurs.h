#ifndef ERREURS_H
#define ERREURS_H
#include <SDL.h>

void SDL_ExitWithError(const char *message);
void SDL_ExitWithErrorGraphic(const char *message,SDL_Window *window,SDL_Renderer *renderer);




#endif 