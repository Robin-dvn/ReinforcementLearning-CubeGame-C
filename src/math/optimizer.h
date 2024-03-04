#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "policy.h"


typedef struct optimizer
{
    Policy_CNN * policy;
    float learning_rate;

}GradDescent;


GradDescent * INNIT_GradDescent(Policy_CNN * policy, float learning_rate);
void GRADD_zero(GradDescent * gd);
void GRADD_Step(GradDescent * gd);


typedef struct Adamoptimizer
{
    Policy_CNN * policy;
    float learning_rate;
    float eps;
    float beta1;
    float beta2;
    int step;


}ADAMoptimizer;

ADAMoptimizer * INNIT_ADAMoptimizer(Policy_CNN * policy, float learning_rate, float eps, float beta1, float beta2);
void ADAMgrad_zero(ADAMoptimizer * adam);
void ADAM_step(ADAMoptimizer * adam);
void sauvegarder_momentum(Policy_CNN * policy, char * dest);
void importer_momentum(Policy_CNN * policy, char * src);

#endif