#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "import_math.h"
#include <math.h>


Policy_CNN * INIT_CNN(int nb_layers, int nb_neuron_layer)
{
    Policy_CNN * policy;
    policy = malloc(sizeof(Policy_CNN));
    policy->nb_layers=nb_layers;
    policy->layers=calloc(nb_layers , sizeof(LinearLayer));
    return policy;
}

void FREE_policy(Policy_CNN * policy)
{
    
    for(int i = 0;i<policy->nb_layers;i++)
    {
        FREE_Layer(policy->layers+i);
    }
    free(policy->layers);
    
    
   
}


void forward_propagation(Policy_CNN *policy,Vecf **input)
{
    
    for(int i =0; i <policy->nb_layers;i++)
    {
        if(!policy->layers[i].activation)
        {
            DOT_MatVec(input,policy->layers[i].weights);
            ADD_Vecf(*input,policy->layers[i].biases);
            COPY_Vecf(*input,policy->layers[i].output,(*input)->size);
        }else{
            
            FUNC_Vecf(*input,policy->layers[i].activation_func);
            COPY_Vecf(*input,policy->layers[i].output,(*input)->size);
        }
        
        
        
        
    }
    
}

float tanh_activation(Data d) {
    return (exp(d.x1) - exp(-d.x1)) / (exp(d.x1) + exp(-d.x1));
}

float dtanh_activation(Data d)
{
    return 1.-tanh_activation(d)*tanh_activation(d);
}




float softmax_activation(Data data) {
    return (exp(data.x1)/data.x2);
}


void back_propagation(Policy_CNN * policy, float loss_value,int action, Vecf * input)
{
    // Calcul du premier terme d'erreur
    Vecf * output = policy->layers[policy->nb_layers-1].output;
    
    Vecf * erreur;
    erreur = INIT_Vecf(output->size);
    COPY_Vecf(output,erreur,output->size);

    Vecf   * vec_action;
    vec_action = INIT_Vecf(output->size);
    for(int i = 0; i < output->size; i++)
    {
        if(i==action)
        {
            vec_action->vec[i] = -1;
        }else vec_action->vec[i] = 0;
        
    }
    
    


    ADD_Vecf(erreur,vec_action );
    
    ADD_Vecf(policy->layers[policy->nb_layers-2].grad_b,erreur);
    

    // création du grad de W
    Matrice * active_mat = NULL;
    active_mat = VEC_TO_MAT(policy->layers[policy->nb_layers-3].output);
    
    Matrice * active_trans = NULL;
    active_trans = TRANSPOSE_Mat(active_mat);
    


    Matrice * erreur_mat = NULL;
    erreur_mat = VEC_TO_MAT(erreur);
    
    
    DOT_MatMat(&erreur_mat,active_trans);
    ADD_Mat(policy->layers[policy->nb_layers-2].grad_W,erreur_mat);
    
    
    
    FREE_Matrice(erreur_mat);
    free(erreur_mat);
    FREE_Matrice(active_trans);
    free(active_trans);
    FREE_Matrice(active_mat);
    free(active_mat);
    FREE_Vecf(vec_action);
    free(vec_action);
    

    
    
    
    
    for (int i = policy->nb_layers-4; i > -1; i--)
    {
        if (!policy->layers[i].activation)
        {
            
            //Calcul de la nouvelle errreur
            Matrice * transpose_W_l =NULL;
            transpose_W_l = TRANSPOSE_Mat(policy->layers[i+2].weights);
            


            erreur_mat = NULL;
            erreur_mat = VEC_TO_MAT(erreur);//dL/dz(l)
            
           
        
      
            DOT_MatMat_Droit(transpose_W_l,&erreur_mat);
            
            
            
            Vecf * derive=NULL;
            derive = INIT_Vecf(policy->layers[i].output->size);// z(l-1)
            COPY_Vecf(policy->layers[i].output,derive,derive->size);
            FUNC_Vecf(derive,dtanh_activation); //g'(z(l-1))

            Matrice * derive_mat =NULL;
            derive_mat = VEC_TO_MAT(derive);
            


            PRODUIT_MatTermesAT(erreur_mat, derive_mat); //dL/dz(l-1)
            //mettre erreur mat dans erreur pour la prochaine boucle
            FREE_Vecf(erreur);
            free(erreur);
            erreur =NULL;
            erreur = Mat_to_Vec(erreur_mat);
            
            

            // Calcul du gradient de b
            ADD_Vecf(policy->layers[i].grad_b,erreur);
            
            

            // Calcul du gradient de W
            active_mat = NULL;
            if(i==0) active_mat = VEC_TO_MAT(input);
            else active_mat = VEC_TO_MAT(policy->layers[i-1].output);

            
            
            active_trans = NULL;
            active_trans = TRANSPOSE_Mat(active_mat);
            
            DOT_MatMat(&erreur_mat,active_trans);

            ADD_Mat(policy->layers[i].grad_W,erreur_mat);

            FREE_Matrice(erreur_mat);
            free(erreur_mat);
            FREE_Matrice(active_trans);
            free(active_trans);
            FREE_Matrice(active_mat);
            free(active_mat);
            

        }
        
    }
    
    FREE_Vecf(erreur);
    free(erreur);


}



void sauvegarder_params(Policy_CNN * policy, char * dest)
{
    FILE * fichier= NULL;
    fichier = fopen(dest,"w");

    for (int i = 0; i < policy->nb_layers; i++)
    {
        if (!(policy->layers[i].activation))
        {
            //mettre les biais
            Vecf * biases = policy->layers[i].biases;
            printf("ici");
            for (int j = 0; j < biases->size ; j++)
            {
                fprintf(fichier,"%f\n",biases->vec[j]);
            }
            //mettre les weights
            Matrice * weights = policy->layers[i].weights;
            for (int k = 0; k < weights->nb_lignes  ; k++)
            {
                for (int l = 0; l < weights->nb_colonnes; l++)
                {
                    fprintf(fichier,"%f\n",weights->mat[l]->vec[k]);
                }
                
            }
        }
        
        
    }
    fclose(fichier);
    
}

void importer_params(Policy_CNN * policy, char * src)
{
    FILE * fichier= NULL;
    fichier = fopen(src,"r");

    float val= 0;
    
    

       
    


    for (int i = 0; i < policy->nb_layers; i++)
    {
        if (!(policy->layers[i].activation))
        {
            //mettre les biais
            Vecf * biases = policy->layers[i].biases;
            for (int j = 0; j < biases->size ; j++)
            {
                fscanf(fichier,"%f",&val);
                biases->vec[j]= val;
            }
            //mettre les weights
            Matrice * weights = policy->layers[i].weights;
            for (int k = 0; k < weights->nb_lignes  ; k++)
            {
                for (int l = 0; l < weights->nb_colonnes; l++)
                {

                    fscanf(fichier,"%f",&val);
                    weights->mat[l]->vec[k]= val;
                    
                }
                
            }
        }
        
        
    }
    fclose(fichier);
}