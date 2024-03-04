#ifndef RENDER_H
#define RENDER_H
#include <SDL.h>
#include "player.h"
#include "map.h"
#include <SDL_ttf.h>
;
typedef struct{
    TTF_Font * font;
    SDL_Color color;
    char text[40];
    SDL_Texture * texturee;
}Text;


SDL_Texture * renderBackground(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture ***textures,SDL_Rect rect_menu);
void create_player(Player *joueur,SDL_Renderer * renderer,SDL_Window *window, SDL_Texture ***textures);
void render_player(Player *joueur,SDL_Renderer * renderer,SDL_Texture ***textures);
void charger_textures(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *** textures);
void free_textures(SDL_Texture ** textures, int taille);
void afficher_sol(SDL_Renderer * renderer,SDL_Texture *** textures);
void jouer(SDL_Window *window,SDL_Renderer * renderer,SDL_Texture * texture_menu, SDL_Rect rect_menu, SDL_Texture *** textures_a_free, char  * map);
void charger_numeros(SDL_Renderer *renderer,SDL_Texture *** textures, SDL_Window * window);
void render_map_number(SDL_Renderer  *renderer,int nb_map, SDL_Texture *** textures);
void render_temps(SDL_Renderer * renderer, SDL_Window *window, Text * text );
void  * myTurn(void *);
void yourturn();
#endif