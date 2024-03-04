#ifndef VECTEUR_H
#define VECTEUR_H

typedef struct vecteur
{
   float x;
   float y;
}Vecf2;

typedef struct vecteuri 
{
   int x;
   int y;
}Veci2;

typedef struct vecteurf 
{
   float *vec;
   int size;
   
}Vecf;

typedef struct data{
    float x1;
    float x2;
    float x3;
    float x4;
}Data;





void add_c(float cst, Vecf2 *vec);
void mul_c(float cst, Vecf2 *vec);
Vecf2 add(Vecf2 vec1, Vecf2 vec2);
void veciToVecf(Vecf2 * vecf, Veci2  veci);
Vecf * INIT_Vecf(int size);
void COPY_Vecf(Vecf  * src, Vecf * dest, int size);
void FREE_Vecf(Vecf * ptVec);
int ADD_Vecf(Vecf *vec1, Vecf *vec2);
void PRINT_Vecf(Vecf * vec);
typedef float (*CallbackFunction)(Data);
void FUNC_Vecf(Vecf * vec, CallbackFunction callback);
void SUBSTRACT_Vec(Vecf * vec1, Vecf * vec2);
void PRODCONST_Vec(Vecf * vec, float c);
void CARRE_Vecf(Vecf *vec);
void ADDCONST_Vecf(Vecf *vec,float cst);
void RACINE2_Vecf(Vecf *vec);
void DIV_Vecf(Vecf *vec1, Vecf *vec2);

typedef struct listeint{
   struct listeint * next;
   int valeur;
   int id;
}ListeInt;

ListeInt* INNIT_listeInt(int premier_terme);
void APPEND_ListeInt(int valeur, ListeInt** l);
void displayListInt(ListeInt* head);
void freeListInt(ListeInt** head);
int getListInt(ListeInt* head, int id);
int getListIntLength(ListeInt* head);


typedef struct ListeFloat {
   float valeur;
   int id;
   struct ListeFloat* next;
} ListeFloat;

ListeFloat* INNIT_listeFloat(float premier_terme);
void APPEND_ListeFloat(float valeur, ListeFloat** l);
void displayListFloat(ListeFloat* head);
void freeListFloat(ListeFloat** head);
float getListFloat(ListeFloat* head, int id);
int getListFloatLength(ListeFloat* head);
float moyenne_list_float(ListeFloat * liste);


typedef struct ListeVecf {
   Vecf valeur;
   int id;
   struct ListeVecf* next;
} ListeVecf;

ListeVecf* INNIT_listeVecf(Vecf premier_terme);
void APPEND_listeVecf(Vecf valeur, ListeVecf** l);
void displayListVecf(ListeVecf* head);
void freeListVecf(ListeVecf** head);
Vecf * getListVecf(ListeVecf* head, int id);
int getListVecfLength(ListeVecf* head);



#endif