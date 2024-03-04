#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "import_math.h"
#include <math.h>




void add_c(float cst, Vecf2 *vec)
{
    vec->x = vec->x+ cst;
    vec->y = vec->y + cst;
}

void mul_c(float cst, Vecf2 *vec)
{
    vec->x = vec->x* cst;
    vec->y = vec->y * cst;
}

Vecf2 add(Vecf2 vec1, Vecf2 vec2)
{
    Vecf2 res;
    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    return res;
}

void veciToVecf(Vecf2 * vecf, Veci2  veci)
{
    vecf->x= (float)veci.x;
    vecf->y= (float)veci.y;
}


Vecf * INIT_Vecf(int size)
{
    Vecf * ptVec = NULL;
    ptVec = malloc(sizeof(Vecf));
    ptVec->vec = calloc(size,sizeof(float));

    if(!ptVec || !(ptVec->vec))
    {
        printf("pas reussi a intialiser ");
        return NULL;
    } 
    ptVec->size = size;

    return ptVec;

}

void COPY_Vecf(Vecf  * src, Vecf * dest, int size)
{
    
    for(int i = 0; i< size;i++)
    {
       
        dest->vec[i] = src->vec[i];
    }
}

void FREE_Vecf(Vecf * ptVec)
{
    
    free(ptVec->vec);
    
}

int ADD_Vecf(Vecf *vec1, Vecf *vec2)
{
    if(vec1->size != vec2->size) return 0;
    for (int i = 0; i < vec1->size; i++)
    {
        vec1->vec[i] = vec1->vec[i] + vec2->vec[i];
    }
    return 0;
    
}

void PRINT_Vecf(Vecf * vec)
{
    printf("\n---\n");
    for (int i = 0; i < vec->size; i++)
    {
        printf("%f\n",vec->vec[i]);
    }
    printf("---\n");
    
}

void FUNC_Vecf(Vecf * vec, CallbackFunction callback)
{
    float sum=0;
    for (int i = 0; i < vec->size; i++)
    {
        sum =sum+ exp(vec->vec[i]);
    }
    for (int i = 0; i < vec->size; i++)
    {
        Data d;
        d.x1 = vec->vec[i];
        d.x2 = sum;
        vec->vec[i]= callback(d);
    }
    
}

void SUBSTRACT_Vec(Vecf * vec1, Vecf * vec2)
{
    for (int i = 0; i < vec1->size; i++)
    {
        vec1->vec[i] -= vec2->vec[i];
    }
    
}

void PRODCONST_Vec(Vecf * vec, float c)
{
    for (int i = 0; i < vec->size; i++)
    {
        vec->vec[i] *= c;
    }
}


ListeInt * INNIT_listeInt(int premier_terme)
{
    ListeInt * l = NULL;
    l = malloc(sizeof(ListeInt));
    l->valeur = premier_terme;
    l->next = NULL;
    return l;

}

void APPEND_ListeInt(int valeur, ListeInt ** l)
{
    ListeInt * newInt = INNIT_listeInt(valeur);

    if(*l == NULL)
    {
        *l = newInt;
        (*l)->id=0;
    }else
    {
        ListeInt * next = *l;
        int id=0;
        while (next->next != NULL)
        {
            
            next = next->next;
            id= next->id;
        }
         
        next->next = newInt;
        newInt->id = id+1;
    }
    
}


void displayListInt(ListeInt * head) {
    ListeInt * temp = head;
    printf("[");
    while (temp != NULL) {
        printf("%d ", temp->valeur);
        temp = temp->next;
    }
    printf("]");
    printf("\n");
}

void freeListInt(ListeInt ** head) {
    ListeInt* current = *head;
    ListeInt * next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    *head = NULL; // Set the head to NULL after freeing all nodes
}

int getListInt(ListeInt* head, int id) {
    ListeInt* current = head;

    while (current != NULL) {
        if (current->id == id) {
            return current->valeur;  // Found the node with the matching ID
        }
        current = current->next;
    }
    printf("no matching id for list int");
    return 0; 
}

int getListIntLength(ListeInt* head) {
    int length = 0;
    ListeInt* current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

ListeFloat* INNIT_listeFloat(float premier_terme) {
    ListeFloat* l = NULL;
    l = malloc(sizeof(ListeFloat));
    l->valeur = premier_terme;
    l->next = NULL;
    return l;
}

void    APPEND_ListeFloat(float valeur, ListeFloat** l) {
    ListeFloat* newFloat = INNIT_listeFloat(valeur);

    if (*l == NULL) {
        *l = newFloat;
        (*l)->id = 0;
    } else {
        ListeFloat* next = *l;
        int id = 0;
        while (next->next != NULL) {
            
           
            next = next->next;
            id = next->id;
        }
        
        next->next = newFloat;
        
        newFloat->id = id + 1;
        
    }
}

void displayListFloat(ListeFloat* head) {
    ListeFloat* temp = head;
    printf("[");
    while (temp != NULL) {
        printf("%f ", temp->valeur);
        temp = temp->next;
    }
    printf("]");
    printf("\n");
}

void freeListFloat(ListeFloat** head) {
    ListeFloat* current = *head;
    ListeFloat* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL; // Set the head to NULL after freeing all nodes
}

float getListFloat(ListeFloat* head, int id) {
    ListeFloat* current = head;

    while (current != NULL) {
        if (current->id == id) {
            return current->valeur;  // Found the node with the matching ID
        }
        current = current->next;
    }

    printf("no matching id");
    return 0.;
}

int getListFloatLength(ListeFloat* head) {
    int length = 0;
    ListeFloat* current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}


ListeVecf * INNIT_listeVecf(Vecf premier_vec) {
    ListeVecf* l = NULL;
    l = malloc(sizeof(ListeVecf));
    l->valeur = premier_vec;
    l->next = NULL;
    return l;
}

void APPEND_listeVecf(Vecf vec, ListeVecf** l) {
    ListeVecf* newVec = INNIT_listeVecf(vec);

    if (*l == NULL) {
        
        *l = newVec;
        
        (*l)->id = 0;
    } else {
        ListeVecf* next = *l;
        int id =0;
        while (next->next != NULL) {
            
            next = next->next;
            id = next->id;
        }
    
        next->next = newVec;
        newVec->id = id + 1;
    }
}

void displayListVecf(ListeVecf* head) {
    ListeVecf* temp = head;
    printf("[");
    while (temp != NULL) {
        printf("[");
        PRINT_Vecf(&(temp->valeur));
        printf("], ");
        temp = temp->next;
    }
    printf("]");
    printf("\n");
}

void freeListVecf(ListeVecf** head) {
    ListeVecf* current = *head;
    ListeVecf* next;

    while (current != NULL) {
        next = current->next;
        FREE_Vecf(&(current->valeur));
        free(current);
        current = next;
    }

    *head = NULL;
}

Vecf * getListVecf(ListeVecf* head, int id) {
    ListeVecf* current = head;

    while (current != NULL) {
        if (current->id==0)
        {
            return &(current->valeur);
        }
        else  if (current->id == id-1) {
            return &(current->next->valeur);
        }
        current = current->next;
    }

    return NULL;
}

int getListVecfLength(ListeVecf* head) {
    int length = 0;
    ListeVecf* current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}


float moyenne_list_float(ListeFloat * liste)
{
    int size = getListFloatLength(liste);
    float somme = 0;
    for (int i = 0; i < size; i++)
    {
        somme += getListFloat(liste,i);
    }
    return somme/size;
    
}

void CARRE_Vecf(Vecf *vec)
{
    for (int i = 0; i < vec->size; i++)
    {
        vec->vec[i] *=vec->vec[i];
    }
    
}

void ADDCONST_Vecf(Vecf *vec,float cst)
{
    for (int i = 0; i < vec->size; i++)
    {
        vec->vec[i] +=cst;
    }
    
}

void RACINE2_Vecf(Vecf *vec)
{
    for (int i = 0; i < vec->size; i++)
    {
        vec->vec[i] = sqrtf(vec->vec[i]);
    }
    
}

void DIV_Vecf(Vecf *vec1, Vecf *vec2)
{
    for (int i = 0; i < vec1->size; i++)
    {
        vec1->vec[i] = vec1->vec[i] / vec2->vec[i];
    }
}