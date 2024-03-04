#ifndef LINEAR_H
#define LINEAR_H
#include "vecteur.h"
#include "matrices.h"


typedef struct linear
{
    Matrice * weights;
    Vecf * biases;
    Vecf * output;
    SDL_bool activation;
    CallbackFunction activation_func;
    int sizeInput;
    int sizeOutput;
    Matrice * grad_W;
    Vecf * grad_b;

    Matrice * Vdw;
    Matrice * Sdw;
    Vecf * Vdb;
    Vecf * Sdb;
    

}LinearLayer;

void INIT_Inside_Layer(LinearLayer * layer, int sizeInput, int sizeOutput);
void INIT_Activation_layer(LinearLayer * layer, int sizeInput, int sizeOutput, CallbackFunction callback);
void FREE_Layer(LinearLayer * layer);
void FREE_PtLayer(LinearLayer  * layer);


#endif