#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "import_math.h"




Categorical * INIT_Categorical(Vecf  * probabilite)
{
    Categorical * ptcategorical = NULL;
    ptcategorical = malloc(sizeof(Categorical));
    if(!ptcategorical)return NULL;

    Vecf * probs=NULL;
    probs = INIT_Vecf(probabilite->size);
    if(!probs)return NULL;

    
    PRINT_Vecf(probabilite);
    printf("avant\n");
    COPY_Vecf(probabilite,probs, probabilite->size);
    PRINT_Vecf(probs);
    Vecf * phis=NULL;
    
    phis = INIT_Vecf(probabilite->size);
    if(!phis)
    {
        
        return NULL;
    }    
    INIT_Phi(probs,phis);
    ptcategorical->phi = phis;
    ptcategorical->probabilites = probs;
    ptcategorical->size = probabilite->size;
    return ptcategorical;
}

void FREE_Categorical(Categorical * ptCat)
{
    FREE_Vecf(ptCat->probabilites);
    FREE_Vecf(ptCat->phi);
    free(ptCat);
}

int SET_CatVec(Vecf * new_prob,Categorical * cat )
{

    
    if(new_prob->size != cat->size) return 1;
    
    COPY_Vecf(new_prob,cat->probabilites,new_prob->size);
    return 0;
}

Vecf * GET_CatVec(Categorical * cat )
{
    return cat->probabilites;
}



void INIT_Phi(Vecf * probs, Vecf * phi)
{
    float somme=0;
    for(int i = 0; i < probs->size;i++)
    {
        somme=somme+ probs->vec[i];
        
        phi->vec[i] = somme;
        
    }
}

void SET_phi(Vecf * probs, Categorical * cat)
{
    float somme=0;
    for(int i = 0; i < probs->size;i++)
    {
        somme=somme+ probs->vec[i];
       
        cat->phi->vec[i] = somme;
        
    }
}


int SAMPLE_Cat(Categorical * cat)
{
    double rand_max = RAND_MAX;
    rand();
    double random = rand()/ rand_max ;
   
    
    
    if (random < cat->phi->vec[0])
    {
       return 0;
    }
    
    for(int i = 1; i<cat->size;i++)
    {
        
        if(random >cat->phi->vec[i-1] && random <cat->phi->vec[i] )
        {
            return i;
        }
    }
}

void UPDATE_Cat(Categorical * cat, Vecf * new)
{
    SET_CatVec(new,cat);
    INIT_Phi(new,cat->phi);
}