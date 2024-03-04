#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "import_math.h"


Matrice * INIT_Matrice(int nb_lignes, int nb_colonnes)
{
    Matrice * matrice = malloc(sizeof(Matrice));

    matrice->mat=calloc(nb_colonnes, sizeof(Vecf *));
    matrice->nb_colonnes =nb_colonnes;
    matrice->nb_lignes=nb_lignes;

    for(int i = 0; i<nb_colonnes; i++)
    {
        Vecf * colonne = NULL;
        colonne = INIT_Vecf(nb_lignes);
        matrice->mat[i] = colonne;
    }

    return matrice;
}

void FREE_Matrice(Matrice * mat)
{
   
   
    for(int i = 0; i<mat->nb_colonnes;i++)
    {
        
        
        FREE_Vecf(mat->mat[i]);
        
    }
    
    free(mat->mat);
   
}

int DOT_MatVec(Vecf ** ptVecteur, Matrice *mat)
{
    
    if((*ptVecteur)->size != mat->nb_colonnes) return 1;

    Vecf * res=NULL;
    res=INIT_Vecf(mat->nb_lignes);
    float somme=0;
    
    for(int ligne =0;ligne<mat->nb_lignes;ligne++)
    {
        somme=0;
        for (int colonne = 0; colonne < mat->nb_colonnes; colonne++)
        {
            somme = somme+ (*ptVecteur)->vec[colonne] * mat->mat[colonne]->vec[ligne];
        }
        res->vec[ligne] =somme;
    }
    FREE_Vecf(*ptVecteur);
    free(*ptVecteur);
    (*ptVecteur) = NULL;
    (*ptVecteur) = res;
    
    
    return 0;
    

}

void PRINT_Matrice(Matrice * mat)
{
    
    for (int i = 0; i < mat->nb_lignes; i++)
    {
        printf("|");
        for (int j = 0; j < mat->nb_colonnes; j++)
        {
            printf("%.4f |",mat->mat[j]->vec[i]);
        }
        printf("\n");
    }
    
}


int DOT_MatMat(Matrice ** Mat_gauche, Matrice * MAT_droite)
{
    
    if((*Mat_gauche)->nb_colonnes != MAT_droite->nb_lignes) return 1;

    Matrice * res=NULL;
    res=INIT_Matrice((*Mat_gauche)->nb_lignes,MAT_droite->nb_colonnes);
    
    float somme=0;
    
    for (int i = 0; i < MAT_droite->nb_colonnes; i++)
    {
        for(int ligne =0;ligne<(*Mat_gauche)->nb_lignes;ligne++)
        {
            somme=0;
            for (int colonne = 0; colonne < MAT_droite->nb_lignes; colonne++)
            {
                somme = somme+ (*Mat_gauche)->mat[colonne]->vec[ligne] * MAT_droite->mat[i]->vec[colonne];
            }
            res->mat[i]->vec[ligne] =somme;
        }
    }
    
    
    FREE_Matrice(*Mat_gauche);
    free(*Mat_gauche);
    (*Mat_gauche) = NULL;
    (*Mat_gauche) = res;
    
    return 0;
    

}

void ONES_Mat(Matrice * mat)
{
    for (int ligne = 0; ligne < mat->nb_lignes; ligne++)
    {
        for (int colonne = 0; colonne < mat->nb_colonnes; colonne++)
        {
            mat->mat[colonne]->vec[ligne] = 1;
        }
        
    }
    
}


Matrice * VEC_TO_MAT(Vecf * vec)
{
    Matrice * res = NULL;
    res = INIT_Matrice(vec->size,1);
    for (int i = 0; i < vec->size; i++)
    {
        res->mat[0]->vec[i]=vec->vec[i];
    }
    
    return res;
}

Matrice * TRANSPOSE_Mat(Matrice * src)
{
    Matrice * res = NULL;
    res = INIT_Matrice(src->nb_colonnes,src->nb_lignes);
    for (int i = 0; i < src->nb_lignes; i++)
    {
        for (int j = 0; j < src->nb_colonnes; j++)
        {
            res->mat[i]->vec[j] = src->mat[j]->vec[i];
        }
        
    }
    return res;
}


void COPY_Mat(Matrice * src, Matrice *dest)
{
    
    for (int i = 0; i < src->nb_lignes; i++)
    {
        for (int j = 0; j < src->nb_colonnes; j++)
        {
            dest->mat[j]->vec[i] = src->mat[j]->vec[i];
        }
        
    }
}



int DOT_MatMat_Droit(Matrice * Mat_gauche, Matrice ** MAT_droite)
{
    
    if(Mat_gauche->nb_colonnes != (*MAT_droite)->nb_lignes) return 1;

    Matrice * res=NULL;
    res=INIT_Matrice(Mat_gauche->nb_lignes,(*MAT_droite)->nb_colonnes);
   
    float somme=0;
    
    for (int i = 0; i < (*MAT_droite)->nb_colonnes; i++)
    {
        for(int ligne =0;ligne<Mat_gauche->nb_lignes;ligne++)
        {
            somme=0;
            for (int colonne = 0; colonne < (*MAT_droite)->nb_lignes; colonne++)
            {
                somme = somme+ Mat_gauche->mat[colonne]->vec[ligne] * (*MAT_droite)->mat[i]->vec[colonne];
            }
            res->mat[i]->vec[ligne] =somme;
        }
    }
    
    
    FREE_Matrice(*MAT_droite);
    free(*MAT_droite);
    (*MAT_droite) = NULL;
    (*MAT_droite) = res;
    
    return 0;
    

}


void * PRODUIT_MatTermesAT(Matrice * mat1, Matrice *mat2)
{
    if (!(mat1->nb_colonnes == mat2->nb_colonnes && mat1->nb_lignes == mat2->nb_lignes))
    {
       printf("Pas les memes tailles de matrices");
       return NULL;
    }
    
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] *= mat2->mat[j]->vec[i];
        }
        
    }
    

}

Vecf * Mat_to_Vec(Matrice * mat)
{
    Vecf * res = NULL;
    res = INIT_Vecf(mat->nb_lignes);

    for (int i = 0; i < mat->nb_lignes; i++)
    {
        res->vec[i] = mat->mat[0]->vec[i];
    }
    return res;
    
}

 void SUBSTRACT_Mat(Matrice * mat1, Matrice * mat2)
 {
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] -= mat2->mat[j]->vec[i];
        }
        
    }
    
 }

 void PRODCONST_MAT(Matrice * mat1, float c)
 {
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] *= c;
        }
        
    }
    
 }

 void ADD_Mat(Matrice * mat1, Matrice * mat2)
 {
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] += mat2->mat[j]->vec[i];
        }
        
    }
 }

void DIVISER_Termes_a_Termes(Matrice *mat1, Matrice *mat2)
{
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] = mat1->mat[j]->vec[i]/ mat2->mat[j]->vec[i];
        }
        
    }
}

void RACINE2_Mat(Matrice *mat1)
{
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] = sqrtf(mat1->mat[j]->vec[i]);
        }
        
    }
}

void CARRE_Mat(Matrice *mat1)
{
    for (int i = 0; i < mat1->nb_lignes; i++)
    {
        for (int j = 0; j < mat1->nb_colonnes; j++)
        {
            mat1->mat[j]->vec[i] *= mat1->mat[j]->vec[i];
        }
        
    }
}

void ADDCONST_Mat(Matrice * mat, float cst)
{
    for (int i = 0; i < mat->nb_lignes; i++)
    {
        for (int j = 0; j < mat->nb_colonnes; j++)
        {
            mat->mat[j]->vec[i] += cst;
        }
        
    }
}