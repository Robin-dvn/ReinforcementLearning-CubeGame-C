#ifndef MAP_H
#define MAP_H
#include <SDL.h>
#include "import_math.h"
#include "constantes.h"

#define TAILLE_X = WINDOW_WIDTH/CUBE
#define TAILLE_Y = WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE

;
typedef struct map{
    float depart;
    int map[WINDOW_WIDTH/CUBE*(WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE)];

}Map;

typedef struct Tile{
    int x;
    int y;

}Tile;

void creer_map( Map * map1,int size_y, int size_x,char *path);
void render_map(SDL_Renderer *renderer, Map * map,int size_y, int size_x,SDL_Texture ***textures);
void indice_coordonne(int i, int nb_l, Tile * tile);
int coordonne_indice(int x, int y);
void modifier_map(SDL_Renderer *renderer, Map *map,int indice, int type);
void enregistrer_map(Map * map,char *path);
int getTileID(int indice, Map *map);
SDL_bool isValidTile(Veci2 position);
int getTile(float x, float y, Map * map);

#endif
