#ifndef MATRICES_H
#define MATRICES_H
#include "vecteur.h"

typedef struct matrice
{
    Vecf ** mat;
    int nb_lignes;
    int nb_colonnes;
}Matrice;

Matrice * INIT_Matrice(int nb_lignes, int nb_colonnes);
void FREE_Matrice(Matrice * mat);
int DOT_MatVec(Vecf ** ptVecteur, Matrice *mat);
int DOT_MatMat(Matrice ** Mat_gauche, Matrice * MAT_droite);
void PRINT_Matrice(Matrice * mat);
void ONES_Mat(Matrice * mat);
Matrice * VEC_TO_MAT(Vecf * vec);
Matrice * TRANSPOSE_Mat(Matrice * src);
void COPY_Mat(Matrice * src, Matrice *dest);
int DOT_MatMat_Droit(Matrice * Mat_gauche, Matrice ** MAT_droite);
void * PRODUIT_MatTermesAT(Matrice * mat1, Matrice *mat2);
Vecf * Mat_to_Vec(Matrice * mat);
void SUBSTRACT_Mat(Matrice * mat1, Matrice * mat2);
void PRODCONST_MAT(Matrice * mat1, float c);
void ADD_Mat(Matrice * mat1, Matrice * mat2);
void RACINE2_Mat(Matrice *mat1);
void DIVISER_Termes_a_Termes(Matrice *mat1, Matrice *mat2);
void CARRE_Mat(Matrice *mat1);
void ADDCONST_Mat(Matrice * mat, float cst);
#endif
