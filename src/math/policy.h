#ifndef POLICY_H
#define POLICY_H

#include "linear.h"

typedef struct policy
{
    LinearLayer * layers;
    int nb_layers;
    int nb_neuron_layer;

}Policy_CNN;


void FREE_policy(Policy_CNN *policy);
void forward_propagation(Policy_CNN *policy,Vecf **input);
float tanh_activation(Data d);
float softmax_activation(Data d);
Policy_CNN * INIT_CNN(int nb_layers, int nb_neuron_layer);
void back_propagation(Policy_CNN * policy, float loss_value,int action, Vecf * input);
float dtanh_activation(Data d);
float relu_activation(Data d);


void sauvegarder_params(Policy_CNN * policy, char * dest);
void importer_params(Policy_CNN * policy, char * src);
#endif