#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <SDL_ttf.h>
#include <string.h>
#include <time.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"
#include "reinforcement.h"
#include <pthread.h>
#include <windows.h>


void  * myTurn(void * ptr)
{
    for (int i = 0; i < 8; i++)
    {
        Sleep(1000);
        printf("MyTurn\n");
    }
    return NULL; 
}
void yourturn()
{
    for(int i = 0; i< 3;i++)
    {
        Sleep(2000);
        printf("yourturn\n");
    }
    
}

void jouer(SDL_Window *window,SDL_Renderer * renderer,SDL_Texture * texture_menu, SDL_Rect rect_menu, SDL_Texture *** textures_a_free, char *map_chemin,SDL_bool RL)
{

    

    SDL_bool jeu_classique;
    jeu_classique = SDL_FALSE;



    
    

    if(RL)
    {

        
        if(TTF_Init()<0)
        {
            printf("Impossible d'ouvrir le ttf : %s",TTF_GetError());
        }
        srand(time(NULL));

        // création de l'environement
        Env * env = create_env(renderer,window, texture_menu, rect_menu, textures_a_free, map_chemin);
        reset(env);
        
        //crétion du Neural net
        Policy_CNN * policy = NULL;
        policy = INIT_CNN(6,3);
        
        INIT_Inside_Layer(policy->layers,53,64);
        INIT_Activation_layer(policy->layers+1,64,64,tanh_activation);
        INIT_Inside_Layer(policy->layers+2,64,64);
        INIT_Activation_layer(policy->layers+3,64,64,tanh_activation);
        INIT_Inside_Layer(policy->layers+4,64,6);
        INIT_Activation_layer(policy->layers+5,6,6,softmax_activation);
        ListeFloat * posx=NULL;
        ListeFloat * listeLoss =NULL;

        importer_params(policy,"../src/policy_params/sauv4.txt");

        ListeVecf  * probs_res=NULL;

        ADAMoptimizer * adam=NULL;
        adam = INNIT_ADAMoptimizer(policy,0.01,0.00000001,0.9,0.999);
        importer_momentum(policy,"../src/policy_params/moment1.txt");
        printf("avant episode");
        
        for (int ep = 0; ep < 10; ep++) //boucle des épisodes
        {
            printf("debut episode");
            reset(env);
            
            for (int i = 0; i < NB_PLAYERS; i++)
            {
                Vecf * state=NULL; //premier state du player
            
                state = getStateFromPlayer(env,SDL_GetTicks(),i);
                state->vec[0] =0.;
                APPEND_listeVecf(*state,&(env->liste_players[i]->states));
                env->liste_players[i]->enjeu= SDL_TRUE;
            }

            Vecf * transition[NB_PLAYERS];

            GradDescent * gd;
            Categorical cat[NB_PLAYERS];
            
            for (int i = 0; i < NB_PLAYERS; i++)
            {
                cat[i].probabilites = INIT_Vecf(6);
                cat[i].phi = INIT_Vecf(6);
                cat[i].size=6;
            }
            
            
            
            
            gd = INNIT_GradDescent(policy,0.01);


            SDL_bool done= SDL_FALSE;
            
            while(!done)
            {
                
                env->step_start = SDL_GetTicks();
                Data_step * data;
                
                for (int i = 0; i < NB_PLAYERS; i++)
                {
                   
                    transition[i] = INIT_Vecf(53);
                    
                    COPY_Vecf(getListVecf(env->liste_players[i]->states,getListVecfLength(env->liste_players[i]->states)-1),transition[i],53);
                    
                    forward_propagation(policy,&(transition[i]));
                }
                
               
                
                
                
                
                
                Vecf * probs[NB_PLAYERS];
                
                int  actions[NB_PLAYERS];
                
                for (int i = 0; i < NB_PLAYERS; i++)
                {
                    probs[i]=NULL;
                    probs[i] =  INIT_Vecf(6);
                    
                    COPY_Vecf(transition[i],probs[i],6);
                    SET_CatVec(transition[i],&(cat[i]));
                    SET_phi(transition[i],&(cat[i]));
                    actions[i] = SAMPLE_Cat(&(cat[i]));
                    
                }
                
                
                data = step(actions,env);
               

                done = SDL_TRUE;
                
                for (int i = 0; i < NB_PLAYERS; i++)
                {

                    if (!(data[i].fini || data[i].truncated))
                    {
                        
                        done =SDL_FALSE;
                    }
                    
                }

                if(done){
                    printf("done\n");
                    SDL_Delay(1000);
                }
                
        
                for (int j = 0; j < NB_PLAYERS; j++)
                {
                    printf("boucle : %d\n",j);
                    if (env->liste_players[j]->enjeu)
                    {
                        APPEND_ListeInt(actions[j],&(env->liste_players[j]->actions));
                        APPEND_ListeFloat(probs[j]->vec[actions[j]],&(env->liste_players[j]->probs));
                        APPEND_listeVecf(*(data[j].new_state),&(env->liste_players[j]->states));
                        APPEND_ListeFloat(data[j].reward,&(env->liste_players[j]->rewards));
                        if(done){
                            printf("wtf%d\n",j);
                            
                        }
                        

                    }else if(env->liste_players[j]->a_modif)
                    {
                        
                        APPEND_ListeInt(actions[j],&(env->liste_players[j]->actions));
                        if(done){
                            printf("int%d\n",j);
                            
                        }
                        APPEND_ListeFloat(probs[j]->vec[actions[j]],&(env->liste_players[j]->probs));
                        if(done){
                            printf("float%d\n",j);
                            
                        }
                        
                        Vecf vec = *(data[j].new_state);
                        
                        APPEND_listeVecf(*(data[j].new_state),&(env->liste_players[j]->states));
                        if(done){
                            printf("vecf%d\n",j);
                            
                        }
                        APPEND_ListeFloat(data[j].reward,&(env->liste_players[j]->rewards));
                        env->liste_players[j]->a_modif =SDL_FALSE;
                        if(done){
                            printf("%d\n",j);
                            
                        }

                    }else{
                        printf("le dernier else:%d\n",j);
                    }
                    
                    
                    
                    FREE_Vecf(probs[j]);
                    free(probs[j]);
                    probs[j]=NULL;
                    FREE_Vecf(transition[j]);
                    free(transition[j]);
                    transition[j] =NULL;
                }
                
                
                

            
                
            }
            printf("apres le done");
            for (int i = 0; i < NB_PLAYERS; i++)
            {
                APPEND_ListeFloat(env->liste_players[i]->max_x,&posx);
                if (i== NB_PLAYERS-1)
                {
                    
                    Vecf  * valeur;
                    valeur = INIT_Vecf(6);
                    COPY_Vecf(cat[i].probabilites,valeur,6);
                    APPEND_listeVecf(*valeur,&probs_res);

                    
                }
                FREE_Categorical(&(cat[i]));
            }
            
            if(!jeu_classique)
            {
                // calcul de la liste des Gt
                int best = 0;
                best = getBestPlayerReward(env);
                
                for (int k = 0; k < 1; k++)
                {
                    
                    int i = best;
                    APPEND_ListeFloat(getListFloat(env->liste_players[i]->rewards,0),&(env->liste_players[i]->returns));
                    
                    
                    float gamma=0.99;
                    for (int j = 1; j < getListIntLength(env->liste_players[i]->actions); j++)
                    {
                        
                    
                        float new = getListFloat(env->liste_players[i]->rewards,j) + getListFloat(env->liste_players[i]->returns, getListFloatLength(env->liste_players[i]->returns)-1) * gamma;
                        APPEND_ListeFloat(new,&(env->liste_players[i]->returns));
                        
                    }
                    
                    // GRADD_zero(gd);
                    ADAMgrad_zero(adam);
                    for (int t = 0; t < getListIntLength(env->liste_players[i]->actions); t++)
                    {
                        float loss = 0;
                        loss = getListFloat(env->liste_players[i]->returns,t) * logf(getListFloat(env->liste_players[i]->probs,t));
                        APPEND_ListeFloat(loss,&listeLoss);
                        back_propagation(policy,loss,getListInt(env->liste_players[i]->actions,t),getListVecf(env->liste_players[i]->states,t));
                        
                    }
                    
                    // GRADD_Step(gd);
                    ADAM_step(adam);

                
                    
                
                }
            }
            
            
            if (ep %10 ==0)
            {
                displayListVecf(probs_res);
            }
            
            
            

            for (int i = 0; i < NB_PLAYERS; i++)
            {
                freeListFloat(&(env->liste_players[i]->returns));
                freeListFloat(&(env->liste_players[i]->rewards));
                freeListVecf(&(env->liste_players[i]->states));
                freeListInt(&(env->liste_players[i]->actions));
                freeListFloat(&(env->liste_players[i]->probs));
                freeListFloat(&(env->liste_players[i]->vitesse_x));
                

                
            }
            
            printf("fin episode");
            

        }
        // PRINT_Matrice(policy->layers[policy->nb_layers-2].weights);
        // PRINT_Vecf(policy->layers[policy->nb_layers-2].biases);
        printf("avant sauv params");
        sauvegarder_params(policy,"../src/policy_params/sauv4.txt");
        
        sauvegarder_momentum(policy,"../src/policy_params/moment1.txt");
        printf("La moyenne du max de x est : %f\n", moyenne_list_float(posx));
        printf("La moyenne de la loss est : %f\n", moyenne_list_float(listeLoss));

       
        freeListVecf(&probs_res);
        freeListFloat(&posx);
        FREE_policy(policy);
        
        FREE_env(env);
        TTF_CloseFont(env->font);
        
        free(env);
        printf("fin");





    }else{
        if(TTF_Init()<0)
        {
            printf("Impossible d'ouvrir le ttf : %s",TTF_GetError());
        }
        
    
        TTF_Font *font = TTF_OpenFont("../src/fonts/Roboto-Medium.ttf",35);
        if(!font)
        {
            printf("watf");
        }
        printf("watf");
        SDL_Color textColor;
        textColor.r=255;
        textColor.g=255;
        textColor.b=255;
        SDL_Texture *  texture_temps;
        Text texte_temps;
        printf("ici");
        texte_temps.color= textColor;
        texte_temps.font = font;
        texte_temps.texturee = texture_temps;


        Map map;
        Player player;
        creer_map(&map,WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE,WINDOW_WIDTH/CUBE,map_chemin);
        
        int debut = SDL_GetTicks();
    
        
        charger_textures(renderer,window,textures_a_free);
        
        render_map(renderer,&map,WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE,WINDOW_WIDTH/CUBE,textures_a_free);
        
        create_player(&player,renderer,window,textures_a_free);

        SDL_RenderPresent(renderer);
        player.enjeu = SDL_TRUE;

        while(player.enjeu)
        {
            int start = SDL_GetTicks();
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                switch (event.type)
                {   
                case SDL_QUIT:
                    player.enjeu = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_m:
                        ;
                        int x=0;
                        int y=0;
                        SDL_GetMouseState(&x,&y);
                        int indice = coordonne_indice(x,y);
                        modifier_map(renderer,&map,indice,1);
                        break;
                    case SDLK_p:
                        ;
                        
                        SDL_GetMouseState(&x,&y);
                        indice = coordonne_indice(x,y);
                        modifier_map(renderer,&map,indice,2);
                        break;
                    case SDLK_i:
                        ;
                        
                        SDL_GetMouseState(&x,&y);
                        indice = coordonne_indice(x,y);
                        modifier_map(renderer,&map,indice,3);
                        break;
                    case SDLK_l:
                        ;
                        
                        SDL_GetMouseState(&x,&y);
                        indice = coordonne_indice(x,y);
                        modifier_map(renderer,&map,indice,0);
                        break;
                    
                    case SDLK_e:
                        
                        enregistrer_map(&map,map_chemin);
                        break;
                    case SDLK_SPACE:
                        update_flags_down(&player,SDLK_SPACE,&map);
                        
                        break;
                    case SDLK_RIGHT:
                        update_flags_down(&player,SDLK_RIGHT,&map);
                    
                        break;
                    case SDLK_LEFT:
                        update_flags_down(&player,SDLK_LEFT,&map);
                    
                        break;
                    case SDLK_UP:
                        update_flags_down(&player,SDLK_UP,&map);
                    
                        break;
                    case SDLK_DOWN:
                        update_flags_down(&player,SDLK_DOWN,&map);
                        
                        break;
                    
                    default:
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        update_flags_up(&player,SDLK_SPACE,&map);
                        
                        break;
                    case SDLK_RIGHT:
                        update_flags_up(&player,SDLK_RIGHT,&map);
                        
                        break;
                    case SDLK_LEFT:
                        update_flags_up(&player,SDLK_LEFT,&map);
                        
                        break;
                    case SDLK_UP:
                        update_flags_up(&player,SDLK_UP,&map);
                        
                        break;
                    case SDLK_DOWN:
                        update_flags_up(&player,SDLK_DOWN,&map);
                        
                        break;
                    
                    default:
                        break;
                    }
                    break;
                
                default:
                    break;
                }
            }
            renderBackground(renderer,window,textures_a_free,rect_menu);
            update_pos_player(&player,&map);
            render_map(renderer,&map,WINDOW_HEIGHT/CUBE-SOL_HEIGHT/CUBE,WINDOW_WIDTH/CUBE,textures_a_free);
            afficher_sol(renderer,textures_a_free);
            render_player(&player,renderer,textures_a_free);
            
            int end =  SDL_GetTicks();
            int temps_de_jeu = end-debut;
            
            if(temps_de_jeu > TEMPS)player.enjeu = SDL_FALSE;
            else
            {
                strcpy(texte_temps.text," ");
                
                char temps_restant_char[3];
                int temps_restant = (int) floor(TEMPS-temps_de_jeu)/1000 +1;
    
                sprintf(temps_restant_char,"%d",temps_restant);
                
                
                strcat(texte_temps.text,temps_restant_char);
                
            
                
                render_temps(renderer,window, &texte_temps);
            } 

            SDL_RenderPresent(renderer);
            if(player.enjeu)SDL_DestroyTexture(texte_temps.texturee);
            end =SDL_GetTicks();
            player.eTime =(end-start) ;

            


            
            
            if ((int)floor(FPS - player.eTime)>0)
            {
                SDL_Delay((int)floor(FPS - player.eTime));
                
            }
            
            
            


        }

        TTF_CloseFont(font);
    
    }

    
    
    

    
    TTF_Quit();
    
    

}

void afficher_sol(SDL_Renderer * renderer,SDL_Texture *** textures)
{
    SDL_Rect rect;
    rect.w =WINDOW_WIDTH;
    rect.h = SOL_HEIGHT;
    rect.x =0;
    rect.y =WINDOW_HEIGHT-SOL_HEIGHT;

    SDL_RenderCopy(renderer, (*textures)[3],NULL,&rect);

    
    
}

SDL_Texture * renderBackground(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture ***textures,SDL_Rect rectangle_menu)
{
    // Création du fond d'écran menu
    
    // centrage de l'image
    rectangle_menu.x =0;
    rectangle_menu.y =0;

    
    //affichage
    if(SDL_RenderCopy(renderer,(*textures)[2],NULL,&rectangle_menu) !=0)
                SDL_ExitWithErrorGraphic("Impossible d'afficher'",window,renderer);

    return (*textures)[2];
}

void create_player(Player *joueur,SDL_Renderer * renderer,SDL_Window *window, SDL_Texture ***textures)
{
    SDL_Surface  * surface_joueur = IMG_Load("../src/images/cube.png");
    if(surface_joueur == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image c",window,renderer);
    SDL_Texture * texture_joueur = SDL_CreateTextureFromSurface(renderer,surface_joueur);
    if(texture_joueur == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture",window,renderer);
   
    initialiser_player(joueur,texture_joueur);
    
    (*textures)[5] = texture_joueur;
    SDL_FreeSurface(surface_joueur);
    
    SDL_Rect rect = getRectPlayer(joueur);
    SDL_QueryTexture(texture_joueur,NULL,NULL,NULL,NULL);
    SDL_RenderCopy(renderer,texture_joueur,NULL,&(rect));
}

void render_player(Player *joueur,SDL_Renderer * renderer,SDL_Texture ***textures)
{
    
    SDL_Rect rect = getRectPlayer(joueur);
    
    // SDL_QueryTexture(joueur->texture,NULL,NULL,NULL,NULL);
    SDL_RenderCopy(renderer,(*textures)[5],NULL,&(rect));
}

void charger_textures(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *** textures)
{


    SDL_Texture *texture_menu_2;
    SDL_Surface * surface_menu  = NULL;
    surface_menu = IMG_Load("../src/images/fond.png");
    if (surface_menu == NULL) SDL_ExitWithError("Impossible de mettre l'image sur la surface ");
    texture_menu_2 = SDL_CreateTextureFromSurface(renderer,surface_menu);
    if(texture_menu_2 == NULL) SDL_ExitWithError("Impossible de mettre la surface sur la texture");
    
    (*textures)[2]=texture_menu_2;
    
    SDL_FreeSurface(surface_menu);


    SDL_Surface  * surface_sol = IMG_Load("../src/images/sol.png");
    if(surface_sol == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image s",window,renderer);
    SDL_Texture * texture_sol = SDL_CreateTextureFromSurface(renderer,surface_sol);
    if(texture_sol == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture s",window,renderer);
    SDL_QueryTexture(texture_sol,NULL,NULL,NULL,NULL);
    (*textures)[3]= texture_sol;
    SDL_FreeSurface(surface_sol);


    SDL_Surface  * surface_pique = IMG_Load("../src/images/pique.png");
    if(surface_pique == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image p",window,renderer);
    SDL_Texture * texture_pique = SDL_CreateTextureFromSurface(renderer,surface_pique);
    if(texture_pique == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture p",window,renderer);
    SDL_QueryTexture(texture_pique,NULL,NULL,NULL,NULL);
    (*textures)[0]= texture_pique;
   
    SDL_FreeSurface(surface_pique);

    SDL_Surface  * surface_pique_inv = IMG_Load("../src/images/pique_inverse.png");
    if(surface_pique_inv == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image pi",window,renderer);
    SDL_Texture * texture_pique_inv = SDL_CreateTextureFromSurface(renderer,surface_pique_inv);
    if(texture_pique_inv == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture p",window,renderer);
    SDL_QueryTexture(texture_pique_inv,NULL,NULL,NULL,NULL);
    (*textures)[1]= texture_pique_inv;
   
    SDL_FreeSurface(surface_pique_inv);

    SDL_Surface  * surface_solide_1 = IMG_Load("../src/images/solide1.png");
    if(surface_solide_1 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image s1",window,renderer);
    SDL_Texture * texture_solide_1 = SDL_CreateTextureFromSurface(renderer,surface_solide_1);
    
    if(texture_solide_1 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture s1",window,renderer);
    SDL_QueryTexture(texture_solide_1,NULL,NULL,NULL,NULL);
    (*textures)[4]= texture_solide_1;
    SDL_FreeSurface(surface_solide_1);

    

   
}

void free_textures(SDL_Texture ** textures, int taille)
{
    for(int i =0; i<taille;i++)
    {
        if(i!=7)
        {
            SDL_DestroyTexture(textures[i]);
        }
        
    }
}

void render_map_number(SDL_Renderer  *renderer,int nb_map, SDL_Texture *** textures)
{
    SDL_Rect rect;
    rect.w = 50;
    rect.h = 50;
    rect.x = 440;
    rect.y = 95;
    switch (nb_map)
    {
    case 0:
        SDL_RenderCopy(renderer,(*textures)[6],NULL,&rect);
        
        break;
    
    case 1:
        SDL_RenderCopy(renderer,(*textures)[7],NULL,&rect);
        break;
    
    case 2:
        SDL_RenderCopy(renderer,(*textures)[8],NULL,&rect);
        break;
    
    case 3:
        SDL_RenderCopy(renderer,(*textures)[9],NULL,&rect);
        break;
    
    default:
        break;
    }
}

void charger_numeros(SDL_Renderer *renderer,SDL_Texture *** textures, SDL_Window * window)
{
    SDL_Surface  * surface_1 = IMG_Load("../src/images/1.png");
    if(surface_1 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image 1",window,renderer);
    SDL_Texture * texture_1 = SDL_CreateTextureFromSurface(renderer,surface_1);
    if(texture_1 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture 1",window,renderer);
    SDL_QueryTexture(texture_1,NULL,NULL,NULL,NULL);
    (*textures)[6]= texture_1;
    SDL_FreeSurface(surface_1);
    
    SDL_Surface  * surface_2 = IMG_Load("../src/images/2.png");
    if(surface_2 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image 2",window,renderer);
    SDL_Texture * texture_2 = SDL_CreateTextureFromSurface(renderer,surface_2);
    if(texture_2 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture 2",window,renderer);
    SDL_QueryTexture(texture_2,NULL,NULL,NULL,NULL);
    (*textures)[7]= texture_2;
    SDL_FreeSurface(surface_2);

    SDL_Surface  * surface_3 = IMG_Load("../src/images/3.png");
    if(surface_3 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image 3",window,renderer);
    SDL_Texture * texture_3 = SDL_CreateTextureFromSurface(renderer,surface_3);
    if(texture_3 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture 3",window,renderer);
    SDL_QueryTexture(texture_3,NULL,NULL,NULL,NULL);
    (*textures)[8]= texture_3;
    SDL_FreeSurface(surface_3);

    SDL_Surface  * surface_4 = IMG_Load("../src/images/4.png");
    if(surface_4 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image 4",window,renderer);
    SDL_Texture * texture_4 = SDL_CreateTextureFromSurface(renderer,surface_4);
    if(texture_4 == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture 4",window,renderer);
    SDL_QueryTexture(texture_4,NULL,NULL,NULL,NULL);
    (*textures)[9]= texture_4;
    SDL_FreeSurface(surface_4);
}

void render_temps(SDL_Renderer * renderer, SDL_Window *window, Text * text )
{
    SDL_Surface  * surface_temps = TTF_RenderText_Solid(text->font,text->text,text->color);
    
    if(surface_temps == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image t",window,renderer);
    text->texturee = SDL_CreateTextureFromSurface(renderer,surface_temps);
    if(text->texturee== NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture t",window,renderer);
    SDL_QueryTexture(text->texturee,NULL,NULL,NULL,NULL);
    SDL_Rect rect ={500,500,surface_temps->w,surface_temps->h};
    rect.x =WINDOW_WIDTH/2-surface_temps->w/2;
    
    
    SDL_FreeSurface(surface_temps);
    SDL_QueryTexture(text->texturee,NULL,NULL,NULL,NULL);
    SDL_RenderCopy(renderer,text->texturee,NULL,&rect);

}

void render_mode(SDL_Renderer * renderer, SDL_Window *window, Text * text )
{
    SDL_Surface  * surface_mode = TTF_RenderText_Solid(text->font,text->text,text->color);
    
    if(surface_mode == NULL) SDL_ExitWithErrorGraphic("Impossible de mettre image mode",window,renderer);
    text->texturee = SDL_CreateTextureFromSurface(renderer,surface_mode);
    if(text->texturee== NULL) SDL_ExitWithErrorGraphic("Impossible de mettre texture t",window,renderer);
    SDL_QueryTexture(text->texturee,NULL,NULL,NULL,NULL);
    SDL_Rect rect ={500,500,surface_mode->w,surface_mode->h};
    rect.x =WINDOW_WIDTH/2-surface_mode->w/2;
    
    
    SDL_FreeSurface(surface_mode);
    SDL_QueryTexture(text->texturee,NULL,NULL,NULL,NULL);
    SDL_RenderCopy(renderer,text->texturee,NULL,&rect);

}