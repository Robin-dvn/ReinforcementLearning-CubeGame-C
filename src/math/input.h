#ifndef INPUT_H
#define INPUT_H
#include <SDL.h>

typedef struct input_flags{
    SDL_bool right;
    SDL_bool left;
    SDL_bool up;
    SDL_bool down;
    SDL_bool space;
    

}InputFlags;

#endif