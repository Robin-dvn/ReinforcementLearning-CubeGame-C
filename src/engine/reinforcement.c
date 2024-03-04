#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<windows.h>
#include <time.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"

Data_step  * step(int  * action,Env *env)
{



    SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    printf("ca marche");
                    break;
                
                default:
                    break;
                }
                break;    
            case SDL_QUIT:
                
                break;
            
            default:
                break;
            }
        }

    
    
    int start = env->step_start;

    
    
    SDL_Color textColor;
    textColor.r=255;
    textColor.g=255;
    textColor.b=255;
    SDL_Texture *  texture_temps;
    Text texte_temps;
    
    texte_temps.color= textColor;
    texte_temps.font = env->font;
    texte_temps.texturee = texture_temps;
    
    Data_step  *data= NULL;
    data = calloc(NB_PLAYERS, sizeof (Data_step));
    for (int i = 0; i < NB_PLAYERS; i++)
    {
        data[i].fini=SDL_FALSE;
        data[i].truncated=SDL_FALSE;
    }
    
    for (int i = 0; i < NB_PLAYERS; i++)
    {
        
        if (env->liste_players[i]->enjeu)
        {
            if(env->liste_players[i]->num_frame %5==0)
            {
                reset_flags(env->liste_players[i]);
                env->liste_players[i]->rien=SDL_FALSE;
                switch (action[i])
                {
                case rien:
                    env->liste_players[i]->rien=SDL_TRUE;
                    break;
                
                case droite:
                    env->liste_players[i]->inputFlags.right = SDL_TRUE;
                    break;
                case gauche:
                    env->liste_players[i]->inputFlags.left = SDL_TRUE;
                    break;
                case saut:
                    env->liste_players[i]->inputFlags.space = SDL_TRUE;
                    break;
                case sautDroite:
                    env->liste_players[i]->inputFlags.right = SDL_TRUE;
                    env->liste_players[i]->inputFlags.space = SDL_TRUE;
                    break;
                case sautGauche:
                    env->liste_players[i]->inputFlags.left = SDL_TRUE;
                    env->liste_players[i]->inputFlags.space = SDL_TRUE;
                    break;
                
                default:
                    break;
                }
            }
            

            update_pos_player(env->liste_players[i],env->map);
            APPEND_ListeFloat(env->liste_players[i]->vitesse.x,&(env->liste_players[i]->vitesse_x));
            
            int end =  SDL_GetTicks();
            float temps_de_jeu = end-env->time_debut;
            env->liste_players[i]->temps_de_jeu = temps_de_jeu;
            SDL_bool hors_temps=SDL_FALSE;
                
            if(temps_de_jeu > TEMPS)
            {
                
                env->liste_players[i]->enjeu =SDL_FALSE;
                data[i].truncated=SDL_TRUE;

                
                
                
                hors_temps = SDL_TRUE;
               
                
                strcat(texte_temps.text," ");  
                
                render_temps(env->renderer,env->window, &texte_temps);
            }    
            else
            {
                strcpy(texte_temps.text," ");
                char temps_restant_char[3];
                int temps_restant = (int) floor(TEMPS-temps_de_jeu)/1000 +1;
                sprintf(temps_restant_char,"%d",temps_restant);
                strcat(texte_temps.text,temps_restant_char);  
                
                render_temps(env->renderer,env->window, &texte_temps);
            } 
                
            
            
        
            if (env->liste_players[i]->position.x > env->liste_players[i]->max_x)
            {
                env->liste_players[i]->max_x = env->liste_players[i]->position.x;
            }else if (env->liste_players[i]->position.x > env->liste_players[i]->max_x)
            {
                env->liste_players[i]->max_x = env->liste_players[i]->max_x+0.0001;
            }
            
            if(!(env->liste_players[i]->enjeu) && hors_temps)
            {
                data[i].truncated=SDL_TRUE;
                
            
            }
            if(!(env->liste_players[i]->enjeu) && !hors_temps)
            {
                data[i].fini=SDL_TRUE;
                data[i].reward = reward_pos_x(env,i) ;

            }else data[i].reward = reward_pos_x(env,i);
            
            Vecf  * newstate =NULL;
            
            if(hors_temps){
                newstate= getStateFromPlayer(env,TEMPS,i);
            }else{
                newstate= getStateFromPlayer(env,temps_de_jeu,i);
            }
            
            data[i].new_state = newstate;
            
            end =SDL_GetTicks();
            env->liste_players[i]->eTime =(end-start) ;


            

            
        
            env->liste_players[i]->num_frame =env->liste_players[i]->num_frame+1;

            
            
        }else{
            
            data[i].truncated = SDL_TRUE;
            data[i].fini = SDL_FALSE;
        }

        
        
        
    }
    
    
    renderBackground(env->renderer,env->window,env->textures_a_free,env->rect_menu);
    render_map(env->renderer,env->map,WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE,WINDOW_WIDTH/CUBE,env->textures_a_free);
    afficher_sol(env->renderer,env->textures_a_free);

    for (int i = 0; i < NB_PLAYERS; i++)
    {
        if (env->liste_players[i]->enjeu)
        {
            
            render_player(env->liste_players[i],env->renderer,env->textures_a_free);
        }
        
        
    }
    
    

    int end =  SDL_GetTicks();
    float temps_de_jeu = end-env->time_debut;
    SDL_bool hors_temps=SDL_FALSE;
            
     
     
    
    strcpy(texte_temps.text," ");
    char temps_restant_char[3];
    if(temps_de_jeu<=TEMPS){
        int temps_restant = (int) floor(TEMPS-temps_de_jeu)/1000 +1;
        sprintf(temps_restant_char,"%d",temps_restant);
        strcat(texte_temps.text,temps_restant_char);
        render_temps(env->renderer,env->window, &texte_temps);
    }else{
        // for (int i = 0; i < NB_PLAYERS; i++)
        //     {
        //         env->liste_players[i]->enjeu =SDL_FALSE;
        //         data[i].truncated=SDL_TRUE;
        //         printf("%d ,",i);

        //     }
        //     printf("\n");
        strcat(texte_temps.text," ");  
                
        render_temps(env->renderer,env->window, &texte_temps);
          
    }
    

    

    SDL_RenderPresent(env->renderer);
    SDL_DestroyTexture(texte_temps.texturee);
    
    
    if ((int)floor(FPS - env->liste_players[NB_PLAYERS-1]->eTime)>0)  
    {
            
        SDL_Delay((int)floor(FPS - env->liste_players[NB_PLAYERS-1]->eTime));
    }
    
    
   
    return data;
}

Env *  create_env(SDL_Renderer *renderer, SDL_Window * window,SDL_Texture * texture_menu, SDL_Rect rect_menu, SDL_Texture *** textures_a_free, char *map_chemin)
{
    Env *env = malloc(sizeof(Env));

    env->map_chemin=map_chemin;
    env->rect_menu = rect_menu;
    env->renderer=renderer;
    env->texture_menu=texture_menu;
    env->textures_a_free=textures_a_free;
    env->window=window;
    env->time_debut= SDL_GetTicks();
    TTF_Font *font = TTF_OpenFont("../src/fonts/Roboto-Medium.ttf",35);
    env->font =font;


    Map * map=NULL;
    map=malloc(sizeof(Map));
    // Player  * player=NULL;
    // player = malloc(sizeof(Player));
    Player ** players = NULL;
    players = calloc(NB_PLAYERS,sizeof( Player *  ));
    

    creer_map(map,WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE,WINDOW_WIDTH/CUBE,map_chemin);
 

    charger_textures(renderer,window,textures_a_free);
    
    for(int i = 0; i < NB_PLAYERS;i++ )
    {
        players[i] =(Player *) malloc(sizeof(Player));
       
        create_player(players[i], renderer,window,textures_a_free);
        
    }
    // create_player(player,renderer,window,textures_a_free);
    env->liste_players = players;
    // env->player =player;
    env->map=map;
    

    return env;
}

void reset(Env * env)
{
    
    env->time_debut = SDL_GetTicks();
    
    
    for(int i = 0; i < NB_PLAYERS;i++ )
    {
        
        
        initialiser_player(env->liste_players[i],env->liste_players[i]->texture);
    }
    
    
    renderBackground(env->renderer,env->window,env->textures_a_free,env->rect_menu);
    
    render_map(env->renderer,env->map,WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE,WINDOW_WIDTH/CUBE,env->textures_a_free);
    afficher_sol(env->renderer,env->textures_a_free);
    
    for(int i = 0; i < NB_PLAYERS;i++ )
    {
        render_player(env->liste_players[i],env->renderer,env->textures_a_free);
    }
    
    SDL_RenderPresent(env->renderer);
   
    

}

void FREE_env(Env * env)
{
    for (int i = 0; i < NB_PLAYERS; i++)
    {
        free(env->liste_players[i]);
    }
    
    
    free(env->map);
}



Vecf * getStateFromPlayer(Env *env, float time,  int nb_player )
{

    
    Vecf * state = NULL;
    state =malloc(sizeof(Vecf));
    int map_size = WINDOW_WIDTH/CUBE*(WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE);
    state = INIT_Vecf(53);
    
    state->vec[0] = time;
  
    state->vec[1] = env->liste_players[nb_player]->position.x;
  
    state->vec[2] = env->liste_players[nb_player]->position.y;
   
    state->vec[3] = env->liste_players[nb_player]->vitesse.x;
    
    state->vec[4] = env->liste_players[nb_player]->vitesse.y;
    
    int pos_x = (int) floor(env->liste_players[nb_player]->position.x) ;
    int pos_y = (int) floor(env->liste_players[nb_player]->position.y);
    
    int compteur = 5;
    for (int i = 0; i < 4; i++)
    {
        for (int j = -3; j < 4; j++)
        {
            if(!(j==0 && i==0))
            {
                Veci2 pos= {pos_x+i,pos_y+j};
                if(!isValidTile(pos))
                {
                    
                    if (pos.x<pos_x)
                    {
                        Veci2 pos_int= {pos_x+i,pos_y};
                        if (!isValidTile(pos_int))
                        {
                            state->vec[compteur] = -1;
                        }else
                        {
                          if (pos.y<pos_y)
                          {
                            state->vec[compteur] = -3;
                          }else state->vec[compteur] = -4;
                           
                        } 
                        
                    }else
                    {
                        Veci2 pos_int= {pos_x+i,pos_y};
                        if (!isValidTile(pos_int))
                        {
                            state->vec[compteur] = -2;
                        }else
                        {
                        if (pos.y<pos_y)
                        {
                            state->vec[compteur] = -3;
                        }else state->vec[compteur] = -4;
                           
                        } 
                    }

                    
                }else{
                    //si la position est valide
                    int indice =coordonne_indice(pos.x,pos.y);
                    state->vec[compteur] = env->map->map[indice];

                }
            }else compteur--;
            compteur++;
        }
        
    }
    

    
    
    return state;
    
}


float reward_pos_x(Env * env, int nb_player)
{
    float pos_x = env->liste_players[nb_player]->position.x;
    float max = env->liste_players[nb_player]->max_x;

    
    float temps = env->liste_players[nb_player]->temps_de_jeu;
    float prob_temps = temps_avitesse_nulle(env->liste_players[nb_player])/env->liste_players[nb_player]->temps_de_jeu;

    
    
    if (env->liste_players[nb_player]->enjeu)
    {
        return 0;
    
    } else
    {
        int un=1;
        if (prob_temps >0.3)
        {
            un=-1;
        
        }
        
        if (env->liste_players[nb_player]->pique_touche)
        {
            float res = pos_x/(WINDOW_WIDTH/CUBE) *un* (1-prob_temps) * temps/TEMPS * 1000;
            
            return  pos_x*100/(WINDOW_WIDTH/CUBE);
        }else return pos_x*100/(WINDOW_WIDTH/CUBE) ;
        
        
    } 
    
    
    
}

float reward_pique(Env * env)
{
    if(env->player->pique_touche)
    {
        return -2 * env->player->max_x;
    }
    else{
        return 2* env->player->max_x;
    } 
    
}

int getBestPlayerFromEnv(Env * env)
{
    float best = -1;
    int id =0;
    for (int i= 0; i < NB_PLAYERS; i++)
    {
        if (env->liste_players[i]->position.x > best)
        {
            // printf("position du nouveau best: %f\n", env->liste_players[i]->position.x);
            best = env->liste_players[i]->position.x;
            id=i;
        }
        
    }
    // printf("position du meilleur :%f\n",env->liste_players[id]->position.x);
    return id;
}

float temps_avitesse_nulle(Player*player)
{
    int nb = 0;
    for (int i = 0; i < getListFloatLength(player->vitesse_x); i++)
    {
        if (getListFloat(player->vitesse_x,i) > -0.02 && getListFloat(player->vitesse_x,i) < 0.02 )
        {
            nb++;
        }
    }
    return nb*FPS;
}


int getBestPlayerReward(Env * env)
{
    int best = 0;
    float reward = -20000;
    for (int i = 0; i < NB_PLAYERS; i++)
    {
        float ret = getListFloat(env->liste_players[i]->rewards,getListFloatLength(env->liste_players[i]->rewards)-1);
        // printf("newe  return :%f\n", ret);
        if ( ret> reward)
        {
            best=i;
            reward=ret;
            // printf("newe best return :%f\n", ret);
        }
        
    }
    return best;
}