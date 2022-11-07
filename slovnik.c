/* all of the function should work (all of them weren't tesetd yet)
   @ATTENTION program isn't finished
*/

/**
 **************************************************************************************************************************
 * 
 * @file slovnik.c
 * @author Matej Baliga
 * @version final a.k.a. ready for MATURITA
 * @date 2022-11-05
 * @brief program je ukazkou pokrocile prace se souborem a retezci na tema slovnik
 * 
 **************************************************************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ERROR       1
#define SUCCES      0

#define MAX_LEN     32
#define LINE_LEN    80

typedef struct data {
    char slovo_en[MAX_LEN];
    char slovo_cs[MAX_LEN];
    char cislo_lekce[MAX_LEN];
} data_t;

int pocet_slov_ve_slovniku() {
    int pocet_slov = 0;

    FILE *soubor = fopen("slovnik.txt", "r");

    char radek[LINE_LEN];

    while (fgets(radek, LINE_LEN, soubor) != NULL) pocet_slov++;

    fclose(soubor); 

    return pocet_slov;
}
int pocet_slov_v_lekci(char cislo_lekce[]) {
    int pocet_slov = 0;

    FILE *soubor = fopen("slovnik.txt", "r");

    char radek[LINE_LEN];

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        if (strstr(radek, cislo_lekce) != NULL) pocet_slov++;
    }

    fclose(soubor);

    return pocet_slov;
}
void velka_na_mala(char slovo[]) {
    for (int i = 0; i < strlen(slovo); i++) {
        if (slovo[i] >= 'A' && slovo[i] <= 'Z')
        slovo[i] += 32;
    }
}
int check_file(FILE *soubor) {
    if (NULL == soubor) {
        printf("slovnki je prazdny");
        return ERROR;
    }
    return SUCCES;
}
int nachazi_se (char slovo[]) {
    char radek[LINE_LEN];

    FILE *soubor = fopen("/slovnik.txt", "r");

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        if (strstr(radek, slovo) != NULL) return SUCCES;
    }

    fclose(soubor);

    return ERROR;
}
void pull_content_from_file(data_t in_out_pole[]) {
    FILE *soubor = fopen("slovnik.txt", "r");

    char radek[LINE_LEN];
    int index = 0;

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce = strtok(NULL, "|");

        strncpy(in_out_pole[index].slovo_en, slovo_en, MAX_LEN-1);
        in_out_pole[index].slovo_en[MAX_LEN-1] = '\0';
        strncpy(in_out_pole[index].slovo_cs, slovo_cs, MAX_LEN-1);
        in_out_pole[index].slovo_cs[MAX_LEN-1] = '\0';        
        strncpy(in_out_pole[index].cislo_lekce, cislo_lekce, MAX_LEN-1);
        in_out_pole[index].cislo_lekce[MAX_LEN-1] = '\0';
        index++;
    }

    fclose(soubor);
}
int strcmp_numbers(char cislo_1_char[], char cislo_2_char[]) {
    // return -1 => 1 < 2
    // return  0 => 1 = 2
    // return  1 => 1 > 2

    int cislo_1 = atoi(cislo_1_char);
    int cislo_2 = atoi(cislo_2_char);

    if (cislo_1 < cislo_2)  return  -1;
    if (cislo_1 == cislo_2) return   0;
    if (cislo_1 > cislo_2)  return   1; 
}
void cpy_data(data_t pole_1[], data_t pole_2[], int index_1, int index_2) {
    strcpy(pole_1[index_1].slovo_en, pole_2[index_2].slovo_en);
    strcpy(pole_1[index_1].slovo_cs, pole_2[index_2].slovo_cs);
    strcpy(pole_1[index_1].cislo_lekce, pole_2[index_2].cislo_lekce);
}
void serazeni_zaznamu(data_t in_out_pole[]) {
    int zamena;

    do {
        zamena = 0;
        for (int i = 0; i < pocet_slov_ve_slovniku() - 1; i++) {
            if ((strcmp_numbers(in_out_pole[i].cislo_lekce, in_out_pole[i+1].cislo_lekce) == 1)) {

                data_t pomocna[1];
            
                cpy_data(pomocna, in_out_pole, 0, i);
                cpy_data(in_out_pole, in_out_pole, i, i+1);
                cpy_data(in_out_pole, pomocna, i+1, 0);
                
                zamena = 1;
            }
        }
    } while (zamena == 1);
}
void vypis_slovniku_cely() {
    data_t zaznamy[pocet_slov_ve_slovniku()];
    
    pull_content_from_file(zaznamy);
    serazeni_zaznamu(zaznamy);

    for (int i = 0; i < pocet_slov_ve_slovniku(); i++) {
        printf("%s %s %s\n", zaznamy[i].slovo_en, zaznamy[i].slovo_cs, zaznamy[i].cislo_lekce);
    }
}
void vypis_slovniku_lekce(char cislo_lekce_in[]) {
    FILE *soubor = fopen("slovnik.txt", "r");

    if (check_file(soubor)) {
        return;
    }
 
    char radek[LINE_LEN];

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce = strtok(NULL ,"|");

        if (!strcmp(cislo_lekce, cislo_lekce_in)) {
            printf("%s  %s,  %s\n", slovo_en, slovo_cs, cislo_lekce);
        }
    }

    fclose(soubor);
}
void preklad_slova(char slovo[]) {
    if (nachazi_se(slovo)) {
        printf("Slovo se nenachazi ve slovniku");
        return;
    }

    char radek[LINE_LEN];

    FILE *soubor = fopen("slovnik.txt", "r");

    if (check_file(soubor)) return;

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");

        if (!strcmp(slovo_en, slovo)) {
            printf("%s", slovo_cs);
        }
        if (!strcmp(slovo_cs, slovo)) {
            printf("%s", slovo_en);
        }
    }

    fclose(soubor);
}
void pridat_do_slovniku(char slovo_en[], char slovo_cs[], char cislo_lekce[]) {
    // if (nachazi_se(slovo_en) || nachazi_se(slovo_cs)) {
    //     printf("slovo se uz nachazi ve slovniku");
    //     return;
    // }

    char slovo_en_f[MAX_LEN];
    char slovo_cs_f[MAX_LEN];

    strcpy(slovo_en_f, slovo_en);
    strcpy(slovo_cs_f, slovo_cs);

    velka_na_mala(slovo_en_f);
    velka_na_mala(slovo_cs_f);

    FILE *soubor = fopen("slovnik.txt", "a");

    fprintf(soubor, "%s|%s|%s|\n", slovo_en_f, slovo_cs_f, cislo_lekce);
    printf("\nslovo bylo pridano do slovniku");

    fclose(soubor);
}
float hodnoceni_v_procentech(int pocet_otazek, int pocet_bodu) {
    return (pocet_bodu * 100.0) / pocet_otazek;
}
void zapis_vysledku(float vysledek) {
    FILE *soubor = fopen("statistiky.txt", "a");

    fprintf(soubor, "%.2f%%\n", vysledek);

    fclose(soubor);
}
void zkouseni_ze_slovicek(char cislo_lekce_CHAR[], int pocet_otazek) {
    int pocet_otazek_f = pocet_otazek;

    if (0 >= pocet_otazek) {
        printf("zkouseni z niceho\n");
        printf("neco takoveho ti neprojde :)\n");
        return;
    }

    if (pocet_slov_v_lekci(cislo_lekce_CHAR) < pocet_otazek) {
        printf("v lekci neni dostatek slov");
        return;
    }

    FILE *soubor = fopen("slovnik.txt", "r");

    int pocet_zodpovezenych_otazek = 0;
    int pocet_bodu = 0;

    char radek[LINE_LEN];

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce_ptr = strtok(NULL, "|");

        if (strcmp(cislo_lekce_CHAR, cislo_lekce_ptr)) {
            continue;
        }

        char odpoved[MAX_LEN];

        printf("%s", slovo_cs);
        printf("\nanglicky preklad: ");
        fflush(stdin);
        gets(odpoved);

        pocet_zodpovezenych_otazek++;

        if (strcmp(slovo_en, odpoved) == 0) {
            printf("spravna odpoved!");
            pocet_bodu++;
        } else { 
            printf("spatna odpoved\n"); 
        }

        if (pocet_otazek_f == pocet_zodpovezenych_otazek) break;
    }

    zapis_vysledku(hodnoceni_v_procentech(pocet_bodu, pocet_otazek));

    fclose(soubor);
}
void odstraneni(char slovo[]) {
    char radek[LINE_LEN];

    FILE *soubor_cteni = fopen("slovnik.txt", "r");
    FILE *soubor_zapis = fopen("slovnik_temp.txt", "w");

    while (fgets(radek, LINE_LEN, soubor_cteni) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce = strtok(NULL, "|");

        if ((strcmp(slovo, slovo_en) != 0) && (strcmp(slovo, slovo_cs) != 0)) {
            fprintf(soubor_zapis, "%s|%s|%s|\n", slovo_en, slovo_cs, cislo_lekce);
        }
    }
    
    fclose(soubor_zapis);
    fclose(soubor_cteni);

    system("del \"slovnik.txt\"");
    system("rename slovnik_temp.txt slovnik.txt");
}
void nahodne_zkouseni(int pocet_otazek) {
    if (pocet_slov_ve_slovniku() < pocet_otazek) {
        printf("ve slovniku neni dostatek slov");
        return;
    }

    FILE *soubor = fopen("slovnik.txt", "r");

    int pocet_bodu = 0;

    char radek[LINE_LEN];
    int index_radku = 0;

    srand((unsigned) time(NULL));

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        int nahodny_index = rand() % pocet_slov_ve_slovniku();

        if (index_radku != nahodny_index) {
            index_radku++;
            continue;
        }

        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce_ptr = strtok(NULL, "|");

        char odpoved[MAX_LEN];

        printf("%s", slovo_cs);
        printf("\nanglicky preklad: ");
        fflush(stdin);
        gets(odpoved);

        if (strcmp(slovo_en, odpoved) == 0) {
            printf("spravna odpoved!");
            pocet_bodu++;
        } else { 
            printf("spatna odpoved\n"); 
        }

        index_radku++;
    }

    zapis_vysledku(hodnoceni_v_procentech(pocet_bodu, pocet_otazek));

    fclose(soubor);
}
void nahrad() {
    system("del \"slovnik.txt\"");
    system("rename slovnik_temp.txt slovnik.txt");
}
void editace(char slovo[]) {
    if (!nachazi_se(slovo)) {
        printf("slovo se nenachazi ve slovniku.\n");
        return;
    }

    FILE *soubor_cteni = fopen("slovnik.txt", "r");
    FILE *soubor_zapis = fopen("slovnik_temp.txt", "w");

    char radek[LINE_LEN];

    while (fgets(radek, LINE_LEN, soubor_cteni) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce = strtok(NULL, "|");

        if ((strcmp(slovo_en, slovo) != 0) && (strcmp(slovo_cs, slovo) != 0) && (strcmp(cislo_lekce, slovo) != 0)) {
            fprintf(soubor_zapis, "%s|%s|%s|\n", slovo_en, slovo_cs, cislo_lekce);
            continue;
        }

        char upravene_slovo[MAX_LEN];
        int volba;

        printf("1 - %s\n", slovo_en);
        printf("2 - %s\n", slovo_cs);
        printf("3 - %s\n", cislo_lekce);
        printf("co chcete zmenit: ");
        scanf("%d", &volba);

        printf("upravena verze: ");
        fflush(stdin);
        gets(upravene_slovo);

        switch (volba) {
            case 1: fprintf(soubor_zapis, "%s|%s|%s|\n", upravene_slovo, slovo_cs, cislo_lekce);
                    break;
            case 2: fprintf(soubor_zapis, "%s|%s|%s|\n", slovo_en, upravene_slovo, cislo_lekce);
                    break;
            case 3: fprintf(soubor_zapis, "%s|%s|%s|\n", slovo_en, slovo_cs, upravene_slovo);
                    break;
        }
    }


    fclose(soubor_zapis);
    fclose(soubor_cteni);

    nahrad();
}
// #endif
int main(void) {
//     pridat_do_slovniku("orange", "pomeranc", "3");
//     pridat_do_slovniku("cucmber", "okurka", "2");
//     pridat_do_slovniku("radish", "turin", "1");
    // pridat_do_slovniku("apple", "jablko", "3");
    // odstraneni("radish");


    // pridat_do_slovniku("peacock", "pav", "5");

    // zkouseni_ze_slovicek("2", 1);

    // preklad_slova("apple");

    // vypis_slovniku_cely();
    vypis_slovniku_lekce("3");


    return 0;
}
