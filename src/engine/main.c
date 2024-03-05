#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "import_math.h"
#include "import_graphic.h"
#include "import_engine.h"
#include "erreurs.h"


int main (int argc, char **argv)
{
    
    // initialisation
    SDL_Init(SDL_INIT_VIDEO);

    // creation de la fenetre et du rendu
    SDL_Window * window = NULL;
    window = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
    SDL_Renderer * renderer = NULL;
    renderer = SDL_CreateRenderer(window,-1,0);
    SDL_Texture *texture_menu;
    SDL_Surface * surface_menu  = NULL;
    surface_menu = IMG_Load("../src/images/background.png");
    if (surface_menu == NULL) SDL_ExitWithError("Impossible de mettre l'image sur la surface");

    char ** maps;
    maps = calloc(9,sizeof(char *));
    int nb_map=1;
    char map1[300] = "../src/maps/map1.txt";
    maps[0]=map1;
    char map2[300] = "../src/maps/map2.txt";
    maps[1]=map2;
    char map3[300] = "../src/maps/map3.txt";
    maps[2]=map3;
    char map4[300] = "../src/maps/map4.txt";
    maps[3]=map4;


    
    texture_menu = SDL_CreateTextureFromSurface(renderer,surface_menu);
    if(texture_menu == NULL) SDL_ExitWithError("Impossible de mettre la surface sur la texture");
    SDL_FreeSurface(surface_menu);
    SDL_Rect rectangle_menu;
    rectangle_menu.w=1000;
    rectangle_menu.h=600;
    rectangle_menu.x=0;
    rectangle_menu.y=0;
    
    // Mise en cache du fond d'écran:
    if(SDL_QueryTexture(texture_menu,NULL,NULL,&rectangle_menu.w,&rectangle_menu.h) !=0) 
                SDL_ExitWithErrorGraphic("Impossible de mettre en cache",window,renderer);
    if(SDL_RenderCopy(renderer,texture_menu,NULL,&rectangle_menu) !=0)
                SDL_ExitWithErrorGraphic("Impossible d'afficher'",window,renderer);
    
    SDL_RenderPresent(renderer);

    //gestion des textures
    SDL_Texture ** textures_a_free;

    textures_a_free = calloc(NB_Text,sizeof( SDL_Texture *));
    charger_numeros(renderer,&textures_a_free,window);
 
    //gestion du mode
    SDL_bool RL = SDL_TRUE;

    if(TTF_Init()<0)
    {
        printf("Impossible d'ouvrir le ttf : %s",TTF_GetError());
    }
    
    TTF_Font *font = TTF_OpenFont("../src/fonts/Roboto-Medium.ttf",20);
    if(!font)
    {
        printf("watf");
    }
        
    SDL_Color textColor;
    textColor.r=255;
    textColor.g=255;
    textColor.b=255;
    SDL_Texture *  texture_mode;
    Text texte_mode;
    texte_mode.color= textColor;
    texte_mode.font = font;
    texte_mode.texturee = texture_mode;
    strcpy(texte_mode.text,"Mode Apprentissage, pour passer en manuel appuyer sur M ");
    // gestion des paramètres du modèle
    SDL_Texture * texture_params;
    Text texte_param;
    texte_param.color = textColor;
    texte_param.font = font;
    texte_param.texturee = texture_params;
    strcpy(texte_param.text,"Pour reset les parametres du modele appuyer sur 0");

    // Boucle infini
    SDL_bool lauched = SDL_TRUE;
    while(lauched)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    jouer(window,renderer,texture_menu,rectangle_menu,&textures_a_free, maps[nb_map],RL);
                    if(SDL_RenderCopy(renderer,texture_menu,NULL,&rectangle_menu) !=0) SDL_ExitWithErrorGraphic("Impossible d'afficher'",window,renderer);
                    SDL_RenderPresent(renderer);
                    
                    free_textures(textures_a_free,NB_Text);
                    charger_numeros(renderer,&textures_a_free,window);
                    break;
                case SDLK_ESCAPE:
                    lauched = SDL_FALSE;
                    break;
                case SDLK_0:
                    FILE *file = fopen("../src/policy_params/sauv4.txt", "w");
                    FILE *file2 = fopen("../src/policy_params/moment1.txt", "w");
    
                    if (file == NULL) {
                        printf("Error opening file");
                        
                    }

                    // Close the file
                    fclose(file);
                    fclose(file2);
                    break;
                case SDLK_1:
                    //nb_map =0;
                    break;
                case SDLK_2:
                    nb_map =1;
                    break;
                case SDLK_3:
                    //nb_map =2;
                    break;
                case SDLK_4:
                    //nb_map =3;
                    printf("ok");
                    break;
                case SDLK_r:
                    RL = SDL_TRUE;
                    printf("mode: RL");
                    strcpy(texte_mode.text,"Mode Apprentissage, pour passer en manuel appuyer sur M ");
                    break;
                case SDLK_m:
                    RL = SDL_FALSE;
                    strcpy(texte_mode.text,"Mode Manuel, pour passer en apprentissage appuyer sur R ");
                    printf("MODE: MANU");
                    break;
                default:
                    break;
                }
                break;    
            case SDL_QUIT:
                lauched = SDL_FALSE;
                break;
            
            default:
                break;
            }
        }
        
        SDL_RenderCopy(renderer,texture_menu,NULL,&rectangle_menu);
        
        render_map_number(renderer,nb_map,&textures_a_free);
        render_texte(renderer,window,&texte_mode,500);
        render_texte(renderer,window,&texte_param,550);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texte_mode.texturee);
        SDL_DestroyTexture(texte_param.texturee);

    }
    
    TTF_CloseFont(font);
    TTF_Quit();
    free(textures_a_free);
    free(maps);
    SDL_DestroyTexture(texture_menu);
   
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}