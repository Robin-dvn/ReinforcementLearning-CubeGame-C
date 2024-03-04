#include <SDL.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"
#include <stdio.h>
#include <math.h>



SDL_bool ground(Player player, Vecf2 position)
{
    if((player.new_pos.y + 1>position.y) && (player.new_pos.y <position.y ))
    {
           
         return SDL_TRUE;
    }
     
    return SDL_FALSE;
}
SDL_bool ceilling(Player player)
{
    if((player.new_pos.y <0))
    {
           
         return SDL_TRUE;
    }
     
    return SDL_FALSE;
}

SDL_bool mur_droit(Player *player)
{
    Vecf2 mur_droit;
    mur_droit.x = WINDOW_WIDTH/CUBE ;
    mur_droit.y =0;
    if(player->new_pos.x+1>mur_droit.x )
    {
        player->enjeu=SDL_FALSE;
         
        return SDL_TRUE;
    }
     
    return SDL_FALSE;
}
SDL_bool mur_gauche(Player player)
{
    
    if(player.new_pos.x<0 )
    {
           
         return SDL_TRUE;
    }
     
    return SDL_FALSE;
}


void apply_colision_2(Player * player, Map *map)
{

    Vecf2 ground_pos = {player->new_pos.x,(WINDOW_HEIGHT-SOL_HEIGHT)/CUBE};

    if(ground(*player,ground_pos) && player->vitesse.y >0)
    {
        player->new_pos.y = floor(ground_pos.y-1);
       
        
        player->vitesse.y =0.;
        player->auSol = SDL_TRUE;
    }else player->auSol = SDL_FALSE;
      

    

    collision_pique2(player,map);
    
    if(player->enjeu)
    {
        // test pour tout ce qui est solide
    if(player->vitesse.x <=0)
    {   
        
        
        if(getTile(player->new_pos.x,player->position.y,map) == 1 || getTile(player->new_pos.x,player->position.y +0.9,map) == 1 )
        {
            player->new_pos.x = (int)player->new_pos.x+1;
            player->vitesse.x = 0;
        }
    }
    else
    {
        {
            if(getTile(player->new_pos.x+1,player->position.y,map) == 1 ||getTile(player->new_pos.x +1,player->position.y +0.9,map) == 1)
            {
                player->new_pos.x = (int)player->new_pos.x;
                player->vitesse.x = 0;
            }
        }
    }

    

    if(!player->auSol)
    {
        if(player->vitesse.y <=0)
        {
            
            if(getTile(player->new_pos.x,player->new_pos.y,map) == 1 ||getTile(player->new_pos.x+0.94,player->new_pos.y,map) == 1 )
            {
                player->new_pos.y = (int)player->new_pos.y+1;
                player->vitesse.y = 0;
                player->auplafond =SDL_TRUE;
                
            }else player->auplafond = SDL_FALSE;;
        }
        else
        {
            {
                if(getTile(player->new_pos.x,player->new_pos.y+1,map) == 1 ||getTile(player->new_pos.x+0.94,player->new_pos.y+0.95 ,map) == 1)
                {
                    
                    player->new_pos.y = (int)player->new_pos.y;
                    player->vitesse.y = 0;
                    player->surSol = SDL_TRUE;
                    player->auplafond=SDL_FALSE;
                    
                }else player->auplafond=SDL_FALSE;
            }
        }
    }
    
    
    
    if(mur_droit(player)) player->new_pos.x = WINDOW_WIDTH/CUBE-1;
    if(mur_gauche(*player)) player->new_pos.x = 0;
    if(ceilling(*player))player->new_pos.y = 0;
    }
       
}


void collision_pique2(Player * player, Map *Mape)
{
    //test pout tout ce qui est piques.

        int indice1 = coordonne_indice(player->new_pos.x*CUBE,(player->new_pos.y +0.95)*CUBE);
        int indice2 = coordonne_indice((player->new_pos.x+0.95)*CUBE,(player->new_pos.y +0.95)*CUBE);
        int indice3 = coordonne_indice((player->new_pos.x+0.95)*CUBE,(player->new_pos.y)*CUBE);
        int indice4 = coordonne_indice((player->new_pos.x+0.05)*CUBE,(player->new_pos.y+0.05)*CUBE);
        Tile tile1;
        Tile tile2;
        Tile tile3;
        Tile tile4;
        indice_coordonne(indice1,(WINDOW_HEIGHT-SOL_HEIGHT)/CUBE,&tile1);
        indice_coordonne(indice2,(WINDOW_HEIGHT-SOL_HEIGHT)/CUBE,&tile2);
        indice_coordonne(indice3,(WINDOW_HEIGHT-SOL_HEIGHT)/CUBE,&tile3);
        indice_coordonne(indice4,(WINDOW_HEIGHT-SOL_HEIGHT)/CUBE,&tile4);

        switch (getTileID(indice1,Mape))
        {
        case 2:
            
            if((float)tile1.x+1.-0.333 >player->new_pos.x )
            {
                player->enjeu=SDL_FALSE;
                player->pique_touche = SDL_TRUE;
                
            } 
            else if(player->vitesse.y >=0)
            {
                if(tile1.y+1-0.333<player->new_pos.y+0.95)
                {
                    player->pique_touche = SDL_TRUE;
                    player->enjeu=SDL_FALSE;
                }    
            }
        
        default:
            break;
        }
        
        switch (getTileID(indice2,Mape))
        {
        case 2:
            if((float)tile2.x+0.333 <player->new_pos.x+0.95 )
            {
                player->enjeu=SDL_FALSE;
                player->pique_touche = SDL_TRUE;
                
            } 
            else if(player->vitesse.y >=0)
            {
                if(tile2.y+1-0.333<player->new_pos.y+0.95)
                {
                    player->enjeu=SDL_FALSE;
                    player->pique_touche = SDL_TRUE;
                } 
            }


            break;
        
        default:
            break;
        }
        switch (getTileID(indice3,Mape))
        {
        case 3:
            if((float)tile3.x+0.333 <player->new_pos.x+0.95 )
            {
                player->enjeu=SDL_FALSE;
                player->pique_touche = SDL_TRUE;
                
            } 
            else 
            {
                if(tile3.y+0.333>player->new_pos.y-0.05)
                {
                    player->enjeu=SDL_FALSE;
                    player->pique_touche = SDL_TRUE;
                }
            }
            break;
        
        default:
            break;
        }
        switch (getTileID(indice4,Mape))
        {
        case 3:
            if((float)tile4.x+1-0.333 >player->new_pos.x+0.05 )
            {
                player->enjeu=SDL_FALSE;
                player->pique_touche = SDL_TRUE;
                
            } 
            else 
            {
                if(tile4.y+0.333>player->new_pos.y-0.05)
                {
                    player->enjeu=SDL_FALSE;
                    player->pique_touche = SDL_TRUE;
                }
            }
            break;
        
        default:
            break;
        }
}