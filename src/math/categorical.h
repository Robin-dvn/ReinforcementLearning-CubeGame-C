#ifndef CATEGORICAL_H
#define CATEGORICAL_H
#include "vecteur.h"

typedef struct
{
    Vecf  * probabilites;
    Vecf * phi;
    int size;
}Categorical;

int SET_CatVec(Vecf * new_prob,Categorical * cat );
Vecf * GET_CatVec(Categorical * cat );

void INIT_Phi(Vecf * probs, Vecf * phi);
Categorical * INIT_Categorical(Vecf  * probabilite);
void FREE_Categorical(Categorical * ptCat);
int SET_CatVec(Vecf * new_prob,Categorical * cat );
Vecf * GET_CatVec(Categorical * cat );
int SAMPLE_Cat(Categorical * cat);
void UPDATE_Cat(Categorical * cat, Vecf * new);
void SET_phi(Vecf * probs, Categorical * cat);
#endif
