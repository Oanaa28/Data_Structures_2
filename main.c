#include "graf1.h"
#include "graf2.h"
/* DIMA Oana-Mihaela - 311CB */

int main(int argc, char *argv[]) {

    /* declarare fiseere */
    char *fisier_intrare = "tema3.in";
    char *fisier_iesire = "tema3.out";

    /* deschidere fisiere */
    FILE *intrare = fopen(fisier_intrare, "r");
    FILE *iesire = fopen(fisier_iesire, "w");
    if (!intrare || !iesire)
        return 0;

    if (argv[1][0] == '1') {
        /* Cerinta 1 */
        int R; /* numarul de rute */
        int K; /* numarul de ani */
        int L; /* gradul de uzura acceptabil */
        fscanf(intrare, "%d", &R);
        fscanf(intrare, "%d", &K);
        fscanf(intrare, "%d", &L);

        /* vector ce contine prima coloana de orase citite */
        char **orase1 = (char**)calloc(MAXI, sizeof(char*));

        /* vector ce contine a doua coloana de orase citite */
        char **orase2 = (char**)calloc(MAXI, sizeof(char*));

        /* vector in care salvez toate numele de orase */
        char **orase = (char**)calloc(MAXI, sizeof(char*));

        /* initializare si alocare memorie pt graf */
        TGL* g = NULL;
        g = AlocG(2 * R);

        /* numarul de noduri ale grafului */
        int i, n = 0;
        for (i = 0; i < R; i++) {
            /* alocare memorie pentru fiecare pereche de orase */
            orase1[i] = (char*) calloc (MAXI, sizeof(char));
            orase2[i] = (char*) calloc (MAXI, sizeof(char));

            /* citire din fisier a perechilor de orase*/
            fscanf(intrare, "%s", orase1[i]);
            fscanf(intrare, "%s", orase2[i]);

            /* construire vector orase */
            int j, ok1 = 1, ok2 = 1;
            for (j = 0; j < n; j++) {
                if(strcmp(orase[j], orase1[i]) == 0) {
                    ok1 = 0;
                }
                if(strcmp(orase[j], orase2[i]) == 0) {
                    ok2 = 0;
                }
            }
            /* daca nu am gasit orasele citite in vectorul orase*/
            if (ok1) {
                orase[n] = (char*) calloc (MAXI, sizeof(char));
                strcpy(orase[n], orase1[i]);
                n++;
            }
            if (ok2) {
                orase[n] = (char*) calloc (MAXI, sizeof(char));
                strcpy(orase[n], orase2[i]);
                n++;
            }
            /* actualizez numarul de noduri din graf */
            g->n = n;

            /* citire tronsoane pentru perechea curenta de orase */
            int nr_tronsoane;
            float *tronson = (float*)calloc(MAXI, sizeof(float));

            fscanf(intrare, "%d", &(nr_tronsoane));

            for (j = 0; j < nr_tronsoane; j++) {
                fscanf(intrare, "%f", &(tronson[j]));
            }
            /* determinarea index-urilor din graf corespunzatore oraselor citite */
            int index1 = Cauta_Oras(g, orase, orase1[i]) + 1;
            int index2 = Cauta_Oras(g, orase, orase2[i]) + 1;
            /* construire arc intre index1 si index2 */
            Construire_Arc(g, index1, index2, nr_tronsoane, tronson, orase);

            /* eliberare memorie tronson curent */
            free(tronson);
        }
        /* rezolvare cerinta */
        Degradare(g, orase, K, L);
        /* afisare graf */
        Afisare(iesire, g, R, L, orase, orase1, orase2);

        /* eliberare memorie */
        for ( i = 0; i < MAXI; i++) {
            free(orase[i]);
            free(orase2[i]);
            free(orase1[i]);
        }
        free(orase);
        free(orase2);
        free(orase1);
        /* distrugere graf si eliberarea memoriei utilizate de acesta */
        DistrG(&g, R);
    } else {
        if (argv[1][0] == '2') {
            /* Cerinta 2 */
            char *oras_start = (char*) calloc (MAXI, sizeof(char));
            fscanf(intrare, "%s", oras_start);

            int K; /* numarul maxim de cai ferate care pot fi pastrate */
            int M; /* numarul cailor ferate existente */
            fscanf(intrare, "%d", &K);
            fscanf(intrare, "%d", &M);
            int i, n = 0, distanta;
            /* alocare memorie graf */
            TGL2* g = NULL;
            g = AlocG2(2 * M);
            /* alocare memorie pt vectori ce retin nume de orase*/
            char** oras_sursa = (char**) calloc(MAXI, sizeof(char*));
            char** oras_dest = (char**) calloc(MAXI, sizeof(char*));
            char** orase = (char**) calloc(MAXI, sizeof(char*));

            for (i = 0; i < M; i++) {
                /* alocare memorie pentru fiecare pereche de orase */
                oras_sursa[i] = (char*) calloc (MAXI, sizeof(char));
                oras_dest[i] = (char*) calloc (MAXI, sizeof(char));

                /* citire din fisier a perechilor de orase*/
                fscanf(intrare, "%s", oras_sursa[i]);
                fscanf(intrare, "%s", oras_dest[i]);

                /* construire vector orase */
                int j, ok1 = 1, ok2 = 1;
                
                for (j = 0; j < n; j++) {
                    if(strcmp(orase[j], oras_sursa[i]) == 0) {
                        ok1 = 0;
                    }
                    if(strcmp(orase[j], oras_dest[i]) == 0) {
                        ok2 = 0;
                    }
                }
                /* daca nu am gasit orasele citite in vectorul orase*/
                if (ok1) {
                    orase[n] = (char*) calloc (MAXI, sizeof(char));
                    strcpy(orase[n], oras_sursa[i]);
                    n++;
                }
                if (ok2) {
                    orase[n] = (char*) calloc (MAXI, sizeof(char));
                    strcpy(orase[n], oras_dest[i]);
                    n++;
                }
                /* actualizez numarul de noduri din graf */
                g->n = n;

    	        /* citire din fisier a distantei */
                /* corespunzatoare fiecarei perechi */
                fscanf(intrare, "%d", &distanta);

                /* determinarea index-urilor din graf corespunzatore oraselor citite */
                int index_sursa = Cauta_Oras2(g, orase, oras_sursa[i]) + 1;
                int index_dest = Cauta_Oras2(g, orase, oras_dest[i]) + 1;
                
                /* construire arc de la index1 la index2 */
                Construire_Arc2(g, index_sursa, index_dest, distanta, orase);
                /* construire arc de la index2 la index1 */
                Construire_Arc2(g, index_dest, index_sursa, distanta, orase);
            }
            int index_oras_start = Cauta_Oras2(g, orase, oras_start) + 1;
            /* initializare si alocare memorie pt. stuctura de distante */
            Dist2 *d = NULL;
            d = calloc(g->n + 1, sizeof(Dist2));

            /* aplicarea algoritmului Dijkstra pe graful dat */
            Dijkstra(iesire, g, M, K, index_oras_start, orase, d, oras_sursa, oras_dest);
            /* afisare graf */
            Afisare2(iesire, g, M, K, orase, oras_sursa, oras_dest, d);

            /* eliberare memorie */

            free(d);
            free(oras_start);
            for (i = 0; i < MAXI; i++) {
                free(orase[i]);
                free(oras_sursa[i]);
                free(oras_dest[i]);
            }
            free(orase);
            free(oras_sursa);
            free(oras_dest);

            /* distrugere graf si eliberarea memoriei utilizate de acesta */
            DistrG2(&g, M);
        }       
    }
    /* inchidere fisiere */
    fclose(intrare);
    fclose(iesire);
    return 0;
}
