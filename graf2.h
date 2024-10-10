#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXI 100
#define INT_MAX 2147483647

/* DIMA Oana-Mihaela - 311CB */

/* stuctura pentru distante*/
typedef struct distante2 {
    int index_sursa;
    int index_dest;
    int distanta;
}Dist2;

/* orasul(nodul)*/
typedef struct destinatie2 {
    int index_oras;     /* indexul orasului */
    char *nume_oras;    /* numele orasului */
} Ddest2;

/* lista de arce */
typedef struct celarc2 { 
    Ddest2 dest;           /* destinatie arc */
    int dist;              /* distanta */
    struct celarc2 *urm; /* adresa urmatorul adiacent */
} TCelArc2, *AArc2;

/* graful */
typedef struct { 
    int n;     /* numar noduri */
    AArc2* x;    /* vector adrese liste arce */
} TGL2;

TGL2* AlocG2(int nr);
int Cauta_Oras2(TGL2 *g, char **orase, char *oras_cautat);
void Construire_Arc2(TGL2 *g, int index1, int index2, int distanta, char **orase);
int MinDist(int nr_noduri, int *vizitat, Dist2 *distanta);
void Dijkstra(FILE *iesire, TGL2 *g, int M, int K, int nod_sursa, char **orase, Dist2 *distanta, char **orase_sursa, char **orase_dest);
void Interschimbare(int *a, int *b);
void Sortare_Distante(TGL2 *g, Dist2 *d);
void Afisare2(FILE *iesire, TGL2 *g, int M, int K, char **orase, char **orase_sursa, char **orase_dest, Dist2 *d);
void DistrG2(TGL2 **ag, int M);