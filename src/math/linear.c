#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "import_math.h"


void INIT_Inside_Layer(LinearLayer * layer, int sizeInput, int sizeOutput)
{
    layer->sizeInput=sizeInput;
    layer->sizeOutput=sizeOutput;
    layer->weights = INIT_Matrice(sizeOutput,sizeInput);
    layer->grad_W = INIT_Matrice(sizeOutput,sizeInput);
    layer->Vdw = INIT_Matrice(sizeOutput,sizeInput);
    layer->Sdw = INIT_Matrice(sizeOutput,sizeInput);
    layer->biases = INIT_Vecf(sizeOutput);
    layer->grad_b = INIT_Vecf(sizeOutput);
    layer->Vdb = INIT_Vecf(sizeOutput);
    layer->Sdb = INIT_Vecf(sizeOutput);

    layer->output = INIT_Vecf(sizeOutput);
    layer->activation = SDL_FALSE;

    for(int c = 0; c<layer->weights->nb_colonnes;c++)
    {
        
        for(int i=0; i<layer->weights->nb_lignes;i++)
        {
            double randmax = RAND_MAX;
            rand();
            if(rand()/randmax<0.5)layer->weights->mat[c]->vec[i] = rand()/ randmax;
            else layer->weights->mat[c]->vec[i] = -rand()/ randmax;
            layer->weights->mat[c]->vec[i]=0.;
            layer->grad_W->mat[c]->vec[i]=0.;
            layer->Vdw->mat[c]->vec[i]=0.;
            layer->Sdw->mat[c]->vec[i]=0.;
        }
        
    }

    for(int j =0; j< sizeOutput;j++)
    {
        double randmax = RAND_MAX;
        rand();
        if(rand()/randmax<0.5)layer->biases->vec[j]=rand()/ randmax/10;
        else  layer->biases->vec[j]=-rand()/ randmax/10;
        layer->grad_b->vec[j]=0.;
        layer->Vdb->vec[j] = 0.;
        layer->Sdb->vec[j] = 0.;

        
    }



}

void FREE_Layer(LinearLayer * layer)
{
    
    FREE_Vecf(layer->biases);
    free(layer->biases);
    FREE_Vecf(layer->grad_b);
    free(layer->grad_b);
    FREE_Vecf(layer->Sdb);
    free(layer->Sdb);
    FREE_Vecf(layer->Vdb);
    free(layer->Vdb);
    
    
    
    FREE_Matrice(layer->grad_W);
    free(layer->grad_W);
    FREE_Matrice(layer->weights);
    free(layer->weights);
    FREE_Matrice(layer->Sdw);
    free(layer->Sdw);
    FREE_Matrice(layer->Vdw);
    free(layer->Vdw);

    FREE_Vecf(layer->output);
    free(layer->output);
    
    
    
}

void FREE_PtLayer(LinearLayer  * layer)
{
    
    FREE_Vecf(layer->biases);
    FREE_Matrice(layer->weights);
    free(layer);
    
    
}

void INIT_Activation_layer(LinearLayer * layer, int sizeInput, int sizeOutput, CallbackFunction callback)
{
    layer->weights = INIT_Matrice(sizeOutput,sizeInput);
    layer->grad_W = INIT_Matrice(sizeOutput,sizeInput);
    layer->Vdw = INIT_Matrice(sizeOutput,sizeInput);
    layer->Sdw = INIT_Matrice(sizeOutput,sizeInput);
    layer->biases = INIT_Vecf(sizeOutput);
    layer->grad_b = INIT_Vecf(sizeOutput);
    layer->Vdb = INIT_Vecf(sizeOutput);
    layer->Sdb = INIT_Vecf(sizeOutput);
    layer->sizeInput = sizeInput;
    layer->sizeOutput = sizeOutput;
    layer->activation = SDL_TRUE;
    layer->activation_func = callback;
    layer->output = INIT_Vecf(sizeOutput);
    
}