#include <SDL.h>
#include <stdio.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"


void initialiser_player(Player *player, SDL_Texture * texture)
{
    
    player->auSol =SDL_FALSE;
    
    player->num_frame=0;
    Vecf2 position = {3.,13.};
    player->position = position;
    Vecf2 vitesse = {0.,0.};
    player->vitesse = vitesse;
    player->texture = texture;
    
    player->last_time = SDL_GetTicks();
    player->inputFlags.down=SDL_FALSE;
    player->inputFlags.right=SDL_FALSE;
    player->inputFlags.up=SDL_FALSE;
    player->inputFlags.left=SDL_FALSE;
    player->inputFlags.space=SDL_FALSE;
    Vecf2 newposition = {0,13.};
    player->new_pos=newposition;
    player->surSol =SDL_FALSE;
    player->auplafond = SDL_FALSE;
    player->max_x=0;
    player->pique_touche = SDL_FALSE;
    player->a_modif=SDL_TRUE;
    player->actions =NULL;
    player->returns = NULL;
    player->rewards = NULL;
    player->states = NULL;
    player->probs = NULL;
    player->vitesse_x=NULL;
    

}

void update_flags_down(Player *player, SDL_KeyCode key_code, Map *map )
{
    
   
    
    switch (key_code)
    {
    case SDLK_RIGHT:
        player->inputFlags.right =SDL_TRUE;
       
        
        break;
    case SDLK_LEFT:
        player->inputFlags.left =SDL_TRUE;
        
        break;
    case SDLK_UP:
        player->inputFlags.up =SDL_TRUE;
        
        break;
    case SDLK_DOWN:
        player->inputFlags.down =SDL_TRUE;
     
        
        break;
    case SDLK_SPACE:
        player->inputFlags.space =SDL_TRUE;
    
    default:
        break;
    }

    


    
    
    

   
    
   
    
    
}

void update_flags_up(Player *player, SDL_KeyCode key_code, Map *map )
{
    
   
    
    switch (key_code)
    {
    case SDLK_RIGHT:
        player->inputFlags.right =SDL_FALSE;
    
        
        break;
    case SDLK_LEFT:
        player->inputFlags.left =SDL_FALSE;
        
        break;
    case SDLK_UP:
        player->inputFlags.up =SDL_FALSE;
        break;
    case SDLK_DOWN:
        player->inputFlags.down =SDL_FALSE;
        
        break;
    
    default:
        break;
    }

    


    if((key_code == SDLK_SPACE))
    {
        player->inputFlags.space =SDL_FALSE;
        
    }
    

   
    
   
    
    
}

void update_pos_player(Player *player, Map *map )
{
    
    if(player->inputFlags.right)player->vitesse.x=8;
    if(player->inputFlags.left)player->vitesse.x=-8;
    if(player->inputFlags.up)player->vitesse.y=-8;
    if(player->inputFlags.down)player->vitesse.y=8;

    if(player->inputFlags.left && player->inputFlags.right)
    {
        player->vitesse.x=0;
    }
    if(!player->surSol &&!player->auSol&& player->inputFlags.right && !player->inputFlags.space)
    {
        player->vitesse.x=3;
    }
    if(!player->surSol &&!player->auSol&& player->inputFlags.left && !player->inputFlags.space)
    {
        player->vitesse.x=-3;
    }

       

    
    

    if((player->vitesse.y==0) && (player->inputFlags.space) && !(player->auplafond))
    {
        player->vitesse.y = -14.2;
    }
    

    update_pos(player,map);
    
}


void update_pos(Player *player,Map*map)
{
    Uint32 current = SDL_GetTicks();
    float dt = (current - player->last_time)/1000.;
    player->last_time = current;
    
    if((player->surSol || player->auSol) && player->vitesse.x !=0)
    {
        
        if (player->vitesse.x<0)
        {
            player->vitesse.x += -20 * player->vitesse.x * 0.02  ;
           
        }else
        {
            player->vitesse.x -= 20 * player->vitesse.x  * 0.02;
          
        } 

        if(abs(player->vitesse.x)<0.01 )
        {
            player->vitesse.x =0;
        }
        
        
    }
    player->vitesse.y += 35.5 *dt; 
    player->new_pos.y =player->position.y + player->vitesse.y *dt;
    player->new_pos.x =player->position.x + player->vitesse.x *dt;
    apply_colision_2(player,map);
    if(player->enjeu)
    {
        player->position.y = player->new_pos.y;
   
        player->position.x = player->new_pos.x;
        
    }
    
    
}

SDL_Rect getRectPlayer(Player * player)
{
    SDL_Rect rect;
    rect.w=CUBE;
    rect.h=CUBE;
    
    rect.x= player->position.x*CUBE;
    
    rect.y = player->position.y*CUBE ;
    //printf(" Rect :%d,%d\n",rect.x,rect.y);

    return rect;
}

void reset_flags(Player * player)
{
    player->inputFlags.down=SDL_FALSE;
    player->inputFlags.right=SDL_FALSE;
    player->inputFlags.up=SDL_FALSE;
    player->inputFlags.left=SDL_FALSE;
    player->inputFlags.space=SDL_FALSE;
}

