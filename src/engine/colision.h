#ifndef COLISION_H
#define COLISION_H
#include "player.h"
#include <SDL.h>
#include "map.h"

void collision_pique(Player * player, Map * Mape);
SDL_bool ground(Player player, Vecf2 position);
void apply_colision_2(Player * player, Map *map);
void collision_pique2(Player * player, Map * Mape);

SDL_bool mur_gauche(Player player);
SDL_bool mur_droit(Player *player);

#endif