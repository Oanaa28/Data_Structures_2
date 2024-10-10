#include "graf1.h"
#include "graf2.h"

/* DIMA Oana-Mihaela - 311CB */

/* aloca spatiu pentru descriptor graf si
vector de (n+1) adrese liste arce */

TGL2* AlocG2(int nr) {

    /* spatiu descriptor */
    TGL2 *g = (TGL2*) malloc(sizeof(TGL2));
    if (!g) return NULL;

    /* spatiu vector adr.liste arce */
    g->x = (AArc2*) calloc((nr+1), sizeof(AArc2));
    if(!g->x) { free(g); return NULL; }

    /* numarul de noduri */
    g->n = nr;
    return g;
}

int Cauta_Oras2(TGL2 *g, char **orase, char *oras_cautat) {
  /* determinare a pozitiei orasului cautat in vectorul de orase */
  int i = 0;
  while(strcmp(orase[i], oras_cautat)) {
    i++;
  }
  return i;
}

void Construire_Arc2(TGL2 *g, int index1, int index2, int distanta, char **orase) {
    AArc2 *p, aux;
    /* determinare pozitie index2 */
    p = g->x + index1;
    while (*p && (*p)->dest.index_oras < index2) {
        p = &(*p)->urm;
    }
    /* alocare memorie pt. noul arc */
    aux = (TCelArc2*)calloc(sizeof(TCelArc2),1);    
    aux->urm = *p; *p = aux;
    /* completare informatii pentru oras */
    aux->dest.index_oras = index2; 
    aux->dest.nume_oras = (char*)calloc(MAXI, sizeof(char));
    strcpy(aux->dest.nume_oras, orase[index2 - 1]);

    /* completare info. pentru distante */
    aux->dist = distanta;
}

int MinDist(int nr_noduri, int *vizitat, Dist2 *distanta) {
  /* functie ce calculeaza minimul din vect de distante 
  pt. nodurile nevizitate*/
  int minDist = INT_MAX, min_index;
  int i;
  for (i = 1; i <= nr_noduri; i++) {
    /* daca nodul curent e mai mic decat ultimul minDist gasit
    si nu e fost vizitat deja */
    if (distanta[i].distanta < minDist && vizitat[i] == 0) {
      minDist = distanta[i].distanta;
      min_index = i;
    }
  }
  /* returneaza indexul minimului gasit */
  return min_index;
}

void Dijkstra(FILE *iesire, TGL2 *g, int M, int K, int nod_sursa, char **orase, Dist2 *distanta, char** orase_sursa, char** orase_dest) {
  
  /* initializare vectori */
  int *vizitat = calloc((g->n + 1), sizeof(int));
  int i;
  for (i = 1; i <= g->n; i++) {
    distanta[i].distanta = INT_MAX;
  }
  /* distanta de la nodul sursa pana la el insusi = 0 */
  distanta[nod_sursa].distanta = 0;

  for (i = 1; i <= g->n; i++) {
    /* selectare nod vizitat */
    int index = MinDist(g->n, vizitat, distanta);
    vizitat[index] = 1;
    /* parcurgem vecinii nodului proaspat vizitat */
   AArc2 L;
    for (L = g->x[index]; L != NULL; L = L -> urm) {
      /* daca nodul nu a fost vizitat */
      if (vizitat[L->dest.index_oras] == 0) {
        /* daca distanta pana in nodul curent + distanta arcului gasit 
        e mai mica decat ultima distanta minima salvata */
        if (L->dist + distanta[index].distanta < distanta[L->dest.index_oras].distanta) {
          /* salvare in structura a valorii distantei curente */
          distanta[L->dest.index_oras].distanta = L->dist + distanta[index].distanta;
          /* salvare in structura a indexului sursa curent */
          distanta[L->dest.index_oras].index_sursa = index;
          /* salvare in structura a indexului destinatie curent */
          distanta[L->dest.index_oras].index_dest = L->dest.index_oras;
        }
      }
    }
  }
  free(vizitat);
}

void Interschimbare(int *a, int *b) {
  /* functie ce interschimba doua valori de tip float*/
  int aux;
  aux = *a;
  *a = *b;
  *b = aux;
}

void Sortare_Distante(TGL2 *g, Dist2 *d) {
  /* functie ce sorteaza crescator structurile 
  distantelor dupa valorile distantelor */
  int i;
  for (i = 1; i < g->n; i++) {
    int j;
    for (j = i + 1; j <= g->n; j++) {
      if (d[i].distanta > d[j].distanta) {
        Interschimbare(&(d[i].distanta), &(d[j].distanta));
        Interschimbare(&(d[i].index_sursa), &(d[j].index_sursa));
        Interschimbare(&(d[i].index_dest), &(d[j].index_dest));
      }
    }
  }
}

void Afisare2(FILE *iesire, TGL2 *g, int M, int K, char **orase, char **orase_sursa, char **orase_dest, Dist2 *distanta) {
  Sortare_Distante(g, distanta);
  /* numarul cailor ce vor fi pastrate */
  int i, T = 0;
  /* determinarea numarului de cai ce vor fi pastrate */
  for (i = 1; i <= g->n; i++)
    if (distanta[i].index_sursa && distanta[i].index_dest) {
      if (T < K)
        T++;
  }
  /* afisarea numarului de cai ce vor fi pastrate */
  fprintf(iesire, "%d\n", T);
  /* afisarea cailor pastrate */
  for (i = 0; i < M; i++) {
    int j;
    for (j = 1; j <= T + 1; j++) {
      /* daca exista cale intre cele doua orase */
      if (distanta[j].index_sursa && distanta[j].index_dest) {
        /* caut orasele ale caror cai vor fi pastrate in
        vectorii orase_sursa si orase_dest */
        /* index_sursa - index_dest */
        int i1 = distanta[j].index_sursa - 1;
        int i2 = distanta[j].index_dest - 1;
        
        if (strcmp (orase_sursa[i], orase[i1]) == 0) {
          if (strcmp (orase_dest[i], orase[i2]) == 0) {
            /* afisare in ordinea citirii */
            fprintf(iesire, "%s %s\n", orase[i1], orase[i2]);
          }
        }
        /* caut orasele ale caror cai vor fi pastrate in
        vectorii orase_sursa si orase_dest */
        /* index_dest - index_sursa */
        if (strcmp (orase_dest[i], orase[i1]) == 0) {
          if (strcmp (orase_sursa[i], orase[i2]) == 0) {
            fprintf(iesire, "%s %s\n", orase[i2], orase[i1]);
          }
        }
      }
    }
  }
}

void DistrG2(TGL2 **ag, int M) { 
    int i;
    AArc2 p, aux;
    /* eliberare memorie graf */
    for(i = 0; i < 2 * M; i++){ 
        p = (*ag)->x[i];
        while(p) {
            aux = p; p = p->urm;
            free(aux->dest.nume_oras);
            free(aux);
        }
    }
    free((*ag)->x);
    free(*ag);
    *ag = NULL;
}