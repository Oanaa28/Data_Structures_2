#include "graf1.h"

/* DIMA Oana-Mihaela - 311CB */

/* aloca spatiu pentru descriptor graf si
vector de (n+1) adrese liste arce */
TGL* AlocG(int nr) {
    /* spatiu descriptor */
    TGL *g = (TGL*) malloc(sizeof(TGL));
    if (!g) return NULL;

    /* spatiu vector adr.liste arce */
    g->x = (AArc*) calloc((nr+1), sizeof(AArc));
    if(!g->x) { free(g); return NULL; }
    /* numarul de noduri */
    g->n = nr;
    return g;
}

void Construire_Arc(TGL *g, int index1, int index2, int nr_tronsoane, float *tronsoane, char **orase) {
    AArc *p, aux;
    /* determinare pozitie index2 */
    p = g->x + index1;
    while (*p && (*p)->dest.index_oras < index2) {
        p = &(*p)->urm;
    }
    /* alocare memorie pt. noul arc */
    aux = (TCelArc*)calloc(sizeof(TCelArc),1);    
    aux->urm = *p; *p = aux;

    /* completare informatii pentru oras */
    aux->dest.index_oras = index2; 
    aux->dest.nume_oras = (char*)calloc(MAXI, sizeof(char));
    strcpy(aux->dest.nume_oras, orase[index2 - 1]);

    /* completare info. pentru nr. de tronsoane */
    aux->nr_tronsoane = nr_tronsoane;
    /* alocare memorie pt. vectorii de val. ale tronsoanelor */
    aux->c = (float*)calloc(MAXI, sizeof(float));
    aux->c_degradat = (float*)calloc(MAXI, sizeof(float));
    /* completare info. pentru valorile tronsoanelor */
    int j;
    for (j = 0; j < nr_tronsoane; j++) {
        aux->c[j] = tronsoane[j];
        aux->c_degradat[j] = tronsoane[j];
    }
}

float Maxim_Urm(TGL *g, int index_nod) {
  float maxi = 0;
  /* determinare vecini ce sunt 'destinatie' pentru nodul primit*/
  AArc L;
  for (L = g->x[index_nod]; L != NULL; L = L -> urm) {
    /* determinare maxim dintre primele valori din vect. de troansoane */
    if (L->c[0] > maxi) {
      maxi = L->c[0];
    }
  }
  return maxi;
}

float Maxim_Ant(TGL *g, int nod) {
  /* determinare vecini ce au ca 'destinatie' nodul primit*/
  int i;
  float maxi = 0;
  /* parcurgere noduri graf*/
  for (i = 1; i <= g->n; i++) { 
    if (i != nod) {
      /* parcurgere arce pt. fiecare nod */
      AArc L;
      for (L = g->x[i]; L != NULL; L = L -> urm) {
        /* daca destinatia nodului i este nodul primit */
        if (L->dest.index_oras == nod) {
          int n = L->nr_tronsoane;
          /* determinare maxim dintre ultimele valori din vect. de troansoane */
          if (maxi < L->c[n - 1]) {
            maxi = L->c[n - 1];
          }
        }
      }
    }
  }
  return maxi;
}

float Maxi(float a, float b) {
  /* functie ce returneaza valoarea maxima dintre a si b */
  if (a > b) return a;
  return b;
}

void Degradare(TGL *g, char **orase, int K, int grad_acceptabil) {
  /* cat timp nu au trecut toti anii */
  while (K) {
    int i;
    /* parcurgere noduri graf*/
    for (i = 1; i <= g->n; i++) {
      AArc L;
      /* parcurgere vecini nod curent */
      for (L = g->x[i]; L != NULL; L = L -> urm) {
        int j; 
        /* parcurgere tronsoane pt arcul curent */
        for (j = 0; j < L->nr_tronsoane; j++) {
          /* grad de uzura diferit de 0 */
          if (L->c[j]) {
            L->c_degradat[j] = L->c[j] * 2;
          } else {
            /* grad de uzura 0 */
            if ( j > 0 ) {
              /* j nu se afla pe ultima valoare din vect. de grade de uzura */
              if (j < L->nr_tronsoane - 1) {
                L->c_degradat[j] = Maxi(L->c[j + 1], L->c[j - 1]) / 2.;
              }
              else {
                L->c_degradat[j] = Maxi(Maxim_Urm(g, L->dest.index_oras), Maxi(L->c[j - 1], Maxim_Ant(g, L->dest.index_oras))) / 2.;
              }
            } 
            else {
              /* j == 0 */
              L->c_degradat[j] = Maxi(Maxim_Ant(g, i), Maxim_Urm(g, i));
              if (j < L->nr_tronsoane - 1) {
                /* j nu se afla pe ultima valoare din vect. de grade de uzura */
                L->c_degradat[j] = Maxi(L->c_degradat[j], L->c[j + 1]) / 2.;
              } else {
                  L->c_degradat[j] = Maxi(L->c_degradat[j], Maxim_Urm(g, L->dest.index_oras)) / 2.;
              }
            }
          }
        }
      }
    }
    /* parcurgere noduri graf*/
    for (i = 1; i <= g->n; i++) {
      AArc L;
      /* parcurgere vecini nod curent */
      for (L = g->x[i]; L != NULL; L = L -> urm) {
        int j; 
        for (j = 0; j < L->nr_tronsoane; j++) {
          /* daca gradul ajunge mai mare decat 100 => limitare la 100 */
          if (L->c_degradat[j] > 100) {
            L->c_degradat[j] = 100;
          }
          /* actualizare grade */
          L->c[j] = L->c_degradat[j];
        }
      }
    }
    K--;
  }
}

float Grad_Acceptabil(int nr_tronsoane, float *grade_uzura) {
  /* determinare a mediei gradelor de uzura pentru ruta curenta */
  int i;
  float medie = 0;
  for (i = 0; i < nr_tronsoane; i++) {
    medie += grade_uzura[i];
  }
  medie = medie / nr_tronsoane;
  return medie;
}

AArc Cauta_Index_Tronsoane(TGL *g, int nod_1, int nod_2) {
  /* determinare arc dintre nod_1 si nod_2 */
  int i;
  /* parcurgere noduri graf*/
  for (i = 1; i <= g->n; i++) { 
      AArc L;
      if (i == nod_1) {
        /* parcurgere vecini nod curent */
        for (L = g->x[i]; L != NULL; L = L -> urm) { 
          if(L->dest.index_oras == nod_2)
            return L;
        }
      }
  }
  return NULL;
}


int Cauta_Oras(TGL *g, char **orase, char *oras_cautat) {
  /* determinare a pozitiei orasului cautat in vectorul de orase */
  int i = 0;
  while(strcmp(orase[i], oras_cautat)) {
    i++;
  }
  return i;
}

void Afisare(FILE *iesire, TGL *g, int R, int grad_acceptabil, char **orase, char **orase1, char **orase2) {
  int i, *rute_pastrate, indice_ruta = 0;
  /* alocare memorie pentru indicii rutelor ce merita pastrate */
  rute_pastrate = (int*)calloc(MAXI, sizeof(int));

  for (i = 0; i < R; i++) {
    /* determinare a pozitiei orasului in vectorul de orase */
    int index1 = Cauta_Oras(g, orase, orase1[i]) + 1;
    int index2 = Cauta_Oras(g, orase, orase2[i]) + 1;

    AArc L = Cauta_Index_Tronsoane(g, index1, index2);
    /* afisare rute si numarul de tronsoane coresp. fiecare rute */
    fprintf(iesire, "%s %s %d ", orase1[i], orase2[i], L->nr_tronsoane);

    /* afisare grade de uzura dupa k ani */
    int j;
    for ( j = 0; j < L->nr_tronsoane; j++) {
      fprintf(iesire, "%.2f ", L->c[j]);
    }
  /* verificare daca media gradelor de uzura este mai mica decat gradul acceptat*/
    if (Grad_Acceptabil(L->nr_tronsoane, L->c) < grad_acceptabil) {
      rute_pastrate[indice_ruta++] = i + 1;
    }
    fprintf(iesire, "\n");
  }
  /* afisare indici rute pastrate */
  for ( i = 0; i < indice_ruta; i++) {
    fprintf(iesire, "%d ", rute_pastrate[i]);
  }
  /* eliberare memorie pt. vectorul de indici */
  free(rute_pastrate);
}

void DistrG(TGL** ag, int R) { 
    int i;
    AArc p, aux;
    /* eliberare memorie graf */
    for(i = 0; i < 2 * R; i++) { 
        p = (*ag)->x[i];
        while(p) {
            aux = p; p = p->urm;
            free(aux->dest.nume_oras);
            free(aux->c_degradat);
            free(aux->c);
            free(aux);
        }
    }
    free((*ag)->x);
    free(*ag);
    *ag = NULL;
}
