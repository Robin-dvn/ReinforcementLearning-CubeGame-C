#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"


void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR: %s > %s \n",message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void SDL_ExitWithErrorGraphic(const char *message,SDL_Window *window,SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_ExitWithError(message);
}

