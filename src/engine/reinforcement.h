#ifndef REINFORCEMENT_H
#define REINFORCEMENT_H
#include <SDL.h>
#include <SDL_ttf.h>
#include "player.h"
#include "map.h"
#include "import_math.h"
typedef struct data_step 
{
    float reward;
    Vecf * new_state;
    SDL_bool fini;
    SDL_bool truncated;
    
}Data_step;

typedef struct environment{
    SDL_Renderer *renderer;
    SDL_Window * window;
    SDL_Texture * texture_menu;
    SDL_Rect rect_menu;
    SDL_Texture *** textures_a_free;
    char * map_chemin;
    Player * player;
    Map * map;
    float time_debut;
    TTF_Font * font;
    float step_start;
    float max_x;
    Player ** liste_players;


}Env;

Env *  create_env(SDL_Renderer *renderer, SDL_Window * window,SDL_Texture * texture_menu, SDL_Rect rect_menu, SDL_Texture *** textures_a_free, char *map_chemin);
void reset(Env * env);
void FREE_env(Env * env);
Data_step * step(int  * action,Env *env);
float reward_pos_x(Env * env, int nb_player);
float reward_pique(Env * env);
Vecf * getStateFromPlayer(Env *env, float time,  int nb_player );

int getBestPlayerFromEnv(Env * env);
float temps_avitesse_nulle(Player*player);
int getBestPlayerReward(Env * env);
#endif