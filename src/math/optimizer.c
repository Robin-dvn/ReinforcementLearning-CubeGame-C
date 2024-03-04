#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "import_math.h"
#include <math.h>




GradDescent * INNIT_GradDescent(Policy_CNN * policy, float learning_rate)
{
    GradDescent  * gd  = NULL;
    gd = malloc(sizeof(GradDescent));

    gd->learning_rate=learning_rate;
    gd->policy=policy;

    return gd;

}

void GRADD_Step(GradDescent * gd)
{
    float alpha = gd->learning_rate;
    Policy_CNN * policy= NULL;
    policy = gd->policy;

    for (int i = 0; i < policy->nb_layers ; i++)
    {
        if(!policy->layers[i].activation)
        {
            PRODCONST_MAT(policy->layers[i].grad_W,alpha);
            PRODCONST_Vec(policy->layers[i].grad_b,alpha);
            SUBSTRACT_Mat(policy->layers[i].weights, policy->layers[i].grad_W);
            SUBSTRACT_Vec(policy->layers[i].biases, policy->layers[i].grad_b);
        }
    }
    
}

void GRADD_zero(GradDescent * gd)
{

    for (int i = 0; i < gd->policy->nb_layers ; i++)
    {
        if(!gd->policy->layers[i].activation)
        {
            PRODCONST_MAT(gd->policy->layers[i].grad_W,0.);
            PRODCONST_Vec(gd->policy->layers[i].grad_b,0.);
            
        }
    }
    
}



ADAMoptimizer * INNIT_ADAMoptimizer(Policy_CNN * policy, float learning_rate, float eps, float beta1, float beta2)
{
    ADAMoptimizer * adam =NULL;
    adam = malloc(sizeof(ADAMoptimizer));

    adam->policy=policy;
    adam->learning_rate=learning_rate;
    adam->beta1=beta1;
    adam->beta2=beta2;
    adam->eps=eps;    
    adam->step = 1;

    return adam;
}


void ADAMgrad_zero(ADAMoptimizer * adam)
{
    for (int i = 0; i < adam->policy->nb_layers ; i++)
    {
        if(!adam->policy->layers[i].activation)
        {
            PRODCONST_MAT(adam->policy->layers[i].grad_W,0.);
            PRODCONST_Vec(adam->policy->layers[i].grad_b,0.);
            
            
        }
    }
}


void ADAM_step(ADAMoptimizer * adam)
{
    float alpha = adam->learning_rate;
    Policy_CNN * policy= NULL;
    policy = adam->policy;

    





    for (int i = 0; i < policy->nb_layers ; i++)
    {
        if(!policy->layers[i].activation)
        {
            // moment beta 1

            // W
            Matrice * un_moins_beta = INIT_Matrice(policy->layers[i].sizeOutput,policy->layers[i].sizeInput);
            COPY_Mat(policy->layers[i].grad_W,un_moins_beta);
            PRODCONST_MAT(un_moins_beta, 1-adam->beta1);
            PRODCONST_MAT(policy->layers[i].Vdw,adam->beta1);
            ADD_Mat(policy->layers[i].Vdw,un_moins_beta);
            // printf("Matrice Vdw de la couche %d, pour l'episode %d",i,adam->step);
           
            FREE_Matrice(un_moins_beta);
            free(un_moins_beta);
            // b
            Vecf * un_moins_beta2 = INIT_Vecf(policy->layers[i].sizeOutput);
            COPY_Vecf(policy->layers[i].grad_b,un_moins_beta2,un_moins_beta2->size);
            PRODCONST_Vec(un_moins_beta2,1-adam->beta1);
            PRODCONST_Vec(policy->layers[i].Vdb,adam->beta1);
            ADD_Vecf(policy->layers[i].Vdb,un_moins_beta2);
            FREE_Vecf(un_moins_beta2);
            free(un_moins_beta2);

            //rms beta 2

            //W
            // Matrice * un_moins_beta3 = INIT_Matrice(policy->layers[i].sizeOutput,policy->layers[i].sizeInput);
            // COPY_Mat(policy->layers[i].grad_W,un_moins_beta3);
            // CARRE_Mat(un_moins_beta3);
            // PRODCONST_MAT(un_moins_beta3, 1-adam->beta2);
            // PRODCONST_MAT(policy->layers[i].Sdw,adam->beta1);
            // ADD_Mat(policy->layers[i].Sdw,un_moins_beta3);
            // FREE_Matrice(un_moins_beta3);
            // free(un_moins_beta3);

            //b
            // Vecf * un_moins_beta4 = INIT_Vecf(policy->layers[i].sizeOutput);
            // COPY_Vecf(policy->layers[i].grad_b,un_moins_beta4,un_moins_beta4->size);
            // CARRE_Vecf(un_moins_beta4);
            // PRODCONST_Vec(un_moins_beta4,1-adam->beta2);
            // PRODCONST_Vec(policy->layers[i].Sdb,adam->beta1);
            // ADD_Vecf(policy->layers[i].Sdb,un_moins_beta4);
            // FREE_Vecf(un_moins_beta4);
            // free(un_moins_beta4);

            // correction avant l'ubdate
            Matrice * Vdw_corr = INIT_Matrice(policy->layers[i].sizeOutput,policy->layers[i].sizeInput);
            // Matrice * Sdw_corr = INIT_Matrice(policy->layers[i].sizeOutput,policy->layers[i].sizeInput);
            Vecf * Vdb_corr = INIT_Vecf(policy->layers[i].sizeOutput);
            // Vecf * Sdb_corr = INIT_Vecf(policy->layers[i].sizeOutput);

            COPY_Mat(policy->layers[i].Vdw,Vdw_corr);
            // COPY_Mat(policy->layers[i].Sdw,Sdw_corr);
            COPY_Vecf(policy->layers[i].Vdb,Vdb_corr,Vdb_corr->size);
            // COPY_Vecf(policy->layers[i].Sdb,Sdb_corr,Vdb_corr->size);

            
            
            //PRODCONST_MAT(Vdw_corr,1/(1-powf(adam->beta1,adam->step)));
            // PRODCONST_MAT(Sdw_corr,1/(1-powf(adam->beta2,adam->step)));
            //PRODCONST_Vec(Vdb_corr,1/(1-powf(adam->beta1,adam->step)));
            // PRODCONST_Vec(Sdb_corr,1/(1-powf(adam->beta2,adam->step)));

            //mise a jour des paramètres

           
            // Matrice * den2 = INIT_Matrice(policy->layers[i].sizeOutput,policy->layers[i].sizeInput);
            // Vecf * den4 = INIT_Vecf(policy->layers[i].sizeOutput);

            
            // COPY_Mat(Sdw_corr,den2);
            // COPY_Vecf(Sdb_corr,den4,den4->size);

            // RACINE2_Mat(den2);
            // RACINE2_Vecf(den4);


            // ADDCONST_Mat(den2,adam->eps);
            // ADDCONST_Vecf(den4,adam->eps);

            // DIVISER_Termes_a_Termes(Vdw_corr,den2);
            // DIV_Vecf(Vdb_corr,den4);
            
            PRODCONST_MAT(Vdw_corr,alpha);     
            PRODCONST_Vec(Vdb_corr,alpha);

            SUBSTRACT_Mat(policy->layers[i].weights, Vdw_corr);
            SUBSTRACT_Vec(policy->layers[i].biases, Vdb_corr);


            // libération des ressources

            FREE_Matrice(Vdw_corr);
            free(Vdw_corr);
            // FREE_Matrice(Sdw_corr);
            // free(Sdw_corr);
            // FREE_Matrice(den2);
            // free(den2);

            FREE_Vecf(Vdb_corr);
            free(Vdb_corr);
            // FREE_Vecf(Sdb_corr);
            // free(Sdb_corr);
            // FREE_Vecf(den4);
            // free(den4);

            
        }
    }

    adam->step+=1;
    
}


void sauvegarder_momentum(Policy_CNN * policy, char * dest)
{
    FILE * fichier= NULL;
    fichier = fopen(dest,"w");

    for (int i = 0; i < policy->nb_layers; i++)
    {
        if (!(policy->layers[i].activation))
        {
            //mettre les biais
            Vecf * biases = policy->layers[i].Vdb;
            for (int j = 0; j < biases->size ; j++)
            {
                fprintf(fichier,"%f\n",biases->vec[j]);
            }
            //mettre les weights
            Matrice * weights = policy->layers[i].Vdw;
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


void importer_momentum(Policy_CNN * policy, char * src)
{
    FILE * fichier= NULL;
    fichier = fopen(src,"r");

    float val= 0;
    
    

       
    


    for (int i = 0; i < policy->nb_layers; i++)
    {
        if (!(policy->layers[i].activation))
        {
            //mettre les biais
            Vecf * biases = policy->layers[i].Vdb;
            for (int j = 0; j < biases->size ; j++)
            {
                fscanf(fichier,"%f",&val);
                biases->vec[j]= val;
            }
            //mettre les weights
            Matrice * weights = policy->layers[i].Vdw;
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