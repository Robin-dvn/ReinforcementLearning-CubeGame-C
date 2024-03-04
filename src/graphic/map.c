#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"


void creer_map( Map * map1,int size_y, int size_x,char *path)
{
    FILE *file =  NULL;

    int i= 0;
    int compteur=0;
    file = fopen(path,"r");

       
    while (compteur <size_x*size_y)
    {  
      
      fscanf (file, "%d", &i);
      
      map1->map[compteur] = i;
      compteur++;    
    }
   
    fclose (file); 
    // for (int i= 0;i<size_y*size_x;i++)
    // {
    //     Tile tile;
       
    //     indice_coordonne(i,size_y,&tile);
      
    //     if(tile.y % 8 == 1 ) map1->map[i] = 1;
    //     else map1->map[i] = 0;
        
        
    // }
    map1->depart= 0;
    
   
}


void render_map(SDL_Renderer *renderer, Map * map,int size_y, int size_x,SDL_Texture ***textures)
{
    int compteur;
    compteur=0;
    for (int i= 0;i<size_x;i++)
    {
        for(int k =0;k<size_y;k++)
        {
            SDL_Rect rect;
            rect.w = CUBE;
            rect.h = CUBE;
            rect.x = CUBE * i+map->depart;
            rect.y = CUBE * k;

            if(map->map[compteur]!=0)
            {
                
                SDL_SetRenderDrawColor(renderer,map->map[compteur],255,255,SDL_ALPHA_OPAQUE);

                switch (map->map[compteur])
                {
                case sol:
                    SDL_RenderCopy(renderer,(*textures)[4],NULL,&rect);
                    break;
                case pique:
                    SDL_RenderCopy(renderer,(*textures)[0],NULL,&rect);
                    
                    break;
                case pique_inv:
                    SDL_RenderCopy(renderer,(*textures)[1],NULL,&rect);
                    
                    break;
                
                default:
                    break;
                }
                
            }
            //printf("Compteur : %d, tile: %d \n",compteur,map->map[compteur]);
            
            compteur++;
        }
       
        
    }

    
}

void indice_coordonne(int i, int nb_l, Tile * tile)
{
    int x;
    int y;

    y= i % nb_l ;
    x = i/nb_l;

    tile->x=x;
    tile->y=y;
    
}

int coordonne_indice(int x, int y)
{
    int tile_x  = x/CUBE;
    int tile_y = y/CUBE;

    return tile_x * (WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE) + tile_y;
}


void modifier_map(SDL_Renderer *renderer, Map *map,int indice, int type)
{
    SDL_Rect rect;
    rect.w = CUBE;
    rect.h = CUBE;
    Tile tile;
    indice_coordonne(indice,8,&tile);
    rect.x = CUBE * tile.x+map->depart;
    rect.y = CUBE * tile.y;
    map->map[indice] =type;
    
    SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer,&rect);
}

void enregistrer_map(Map * map,char *path)
{
    FILE *file;
    remove(path);
    file = fopen(path,"w");
    
       
    for(int i = 0;i< (WINDOW_WIDTH/CUBE)*(WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE);i++) 
    {  
      fprintf(file,"%d\n",map->map[i]);     
    }
    fclose (file); 
}

SDL_bool isValidTile(Veci2 position)
{
    return position.x *CUBE <WINDOW_WIDTH && position.y *CUBE < WINDOW_HEIGHT-SOL_HEIGHT && position.x*CUBE >=0 && position.y*CUBE >=0;
}

int getTileID(int indice, Map * map)
{
    return map->map[indice];
}

int getTile(float x, float y, Map * map)
{
    int indice = coordonne_indice(x*CUBE,y*CUBE);
    return getTileID(indice,map);
}