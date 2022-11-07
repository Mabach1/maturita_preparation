/* program dosen't work, under constraction */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_LEN 32

typedef struct udaje {
    char    jmeno[CHAR_LEN];
    char    prijmeni[CHAR_LEN];
    char    trida[CHAR_LEN];
    int     vek;
} udaje_t;

typedef struct zak {
    udaje_t      data;
    struct zak  *nasl;
    struct zak  *pred;
} zak_t;

zak_t *prvni = NULL;

void pridaniNaKonec(char jmeno[], char prijmeni[], char trida[], int vek) {
    zak_t *novy;

    novy = (zak_t*) malloc(sizeof(zak_t));

    if (NULL == novy) {
        printf("Nedostatek mista\n");
        return;
    }

    strcpy(novy->data.jmeno, jmeno);
    strcpy(novy->data.prijmeni, prijmeni);
    strcpy(novy->data.trida, trida);
    novy->data.vek = vek;

    novy->nasl = NULL;
    novy->pred = NULL;

    if (NULL == prvni) {
        prvni = novy;
    } else {
        zak_t *posledni = prvni;
        while (posledni->nasl != NULL) { posledni = posledni->nasl; }
        
        posledni->nasl = novy;
        posledni->pred = prvni;
    }
}

void vypis() {
    for (zak_t *pom = prvni; pom != NULL; pom = pom->nasl) {
        printf("%s %s %s %d\n", pom->data.jmeno, pom->data.prijmeni, pom->data.trida, pom->data.vek);
    }
}

int checkPrvniForNULL() {
    if (prvni == NULL) {
        printf("seznam je preazdny");
        return 1;
    }
    return 0;
}

void odstraneniZaznamu(char hledany[]) {
    if (checkPrvniForNULL()) return;

    for (zak_t *pomocny = prvni; pomocny != NULL; pomocny = pomocny->nasl) {
        if (strcmp(pomocny->data.prijmeni, hledany) != 0) {
            continue;
        }

        if (pomocny->pred == NULL && pomocny->nasl == NULL) {
            prvni = NULL;
            free(pomocny);
            break;
        } else if (pomocny->pred == NULL) {
            pomocny->nasl->pred = NULL;
            prvni = pomocny->nasl;
            free(pomocny);
            break;
        } else if (pomocny->nasl == NULL) {
            pomocny->pred->nasl = NULL;
            prvni = pomocny->pred;
            free(pomocny);
            break;
        } else {
            pomocny->nasl->pred = pomocny->pred;
            pomocny->pred->nasl = pomocny->nasl;
            free(pomocny);
            break;
        }
    }
    
}

int main() {
    pridaniNaKonec("matej", "baliga", "4.b", 18);
    pridaniNaKonec("matej", "hanke", "4.b", 18);
    pridaniNaKonec("matej", "gruzskowski", "4.b", 18);
    pridaniNaKonec("matej", "brezina", "4.b", 18);



    // odstraneniZaznamu("brezina");

    vypis();
}