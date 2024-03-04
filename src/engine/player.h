#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include "import_math.h"


typedef struct player
{
    Vecf2  position;
    Vecf2  new_pos;
    Vecf2 vitesse;
    SDL_Texture  * texture;
    SDL_bool auSol;
    SDL_bool surSol;
    float eTime;
    Uint32 last_time;
    InputFlags inputFlags;
    SDL_bool auplafond;
    SDL_bool enjeu;
    int player_map;
    int num_frame;
    float max_x;
    SDL_bool pique_touche;
    ListeFloat *rewards;
    ListeFloat *returns;
    ListeFloat *probs;
    ListeVecf  *states;
    ListeInt  *actions;
    float temps_de_jeu;
    SDL_bool rien;
    ListeFloat *vitesse_x;
    SDL_bool   a_modif;
    
}Player;

void initialiser_player(Player *player,SDL_Texture * texture);
void update_pos_player(Player *player, Map *map );
void update_flags_up(Player *player, SDL_KeyCode key_code, Map *map );
void update_flags_down(Player *player, SDL_KeyCode key_code, Map *map );
void update_pos(Player *player,Map*map);
SDL_Rect getRectPlayer(Player * player);
void reset_flags(Player * player);


#endif