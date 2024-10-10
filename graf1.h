#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXI 100
#define INT_MAX 2147483647

/* DIMA Oana-Mihaela - 311CB */

/* nod(oras) */
typedef struct destinatie {
    int index_oras;     /* indexul orasului */
    char *nume_oras;    /* numele orasului */
} Ddest;

/* lista de arce */
typedef struct celarc { 
    Ddest dest;          /* destinatie arc */

    float* c;            /* cost arc */
    float* c_degradat;   /* cost arc degradat */

    int nr_tronsoane;    /* numarul de tronsoare*/

    struct celarc *urm;  /* adresa urmatorul adiacent */
} TCelArc, *AArc;

typedef struct { 
    int n;      /* numar noduri */
    AArc* x;    /* vector adrese liste arce */
} TGL;

TGL* AlocG(int nr);
void Construire_Arc(TGL *g, int index1, int index2, int nr_tronsoane, float *tronsoane, char **orase);
float Maxim_Urm(TGL *g, int index_nod);
float Maxim_Ant(TGL *g, int nod);
float Maxi(float a, float b);
void Degradare(TGL *g, char **orase, int K, int grad_acceptabil);
float Grad_Acceptabil(int nr_tronsoane, float *grade_uzura);
AArc Cauta_Index_Tronsoane(TGL *g, int nod_1, int nod_2);
int Cauta_Oras(TGL *g, char **orase, char *oras_cautat);
void Afisare(FILE *iesire, TGL *g, int R, int grad_acceptabil, char **orase, char **orase1, char **orase2);
void DistrG(TGL** ag, int R);