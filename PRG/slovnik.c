/**
 ********************************************************************************************************************************************************
 * @file slovnik.c
 * @author Matej Baliga
 * @version final a.k.a. ready for MATURITA
 * @date 2022-11-05
 * @brief program je ukazkou pokrocile prace se souborem a retezci na tema slovnik
 * 
 * @attention Program wasn't refactored yet and needs a lot more work. Program dosen't have user menu, all functions work
 * 
 *********************************************************************************************************************************************************
 */

#include <stdio.h>          // basic in/out (printf(), scanf(), ...)
#include <stdlib.h>         // funkce atoi()
#include <string.h>         // funkce pro prace s retezcem (strcpy(), strcmp(), ...)
#include <time.h>           // pro nahondne generovani

#define ERROR        1
#define SUCCESS      0

#define MAX_LEN     32      // maximalni delka vstupniho retezce
#define LINE_LEN    80      // maximalni delka nacitaneho radku souboru

/**
 * @brief datovy typ pro importovani dat ze souboru
 * 
 * Pro vypis vsech dat ze souboru je nejdriv bylo nutne seradit dle cisla
 * lekce, aby vypis daval smysl. Alternativou je bylo 3D char pole.
 */
typedef struct data {
    char slovo_en[MAX_LEN];
    char slovo_cs[MAX_LEN];
    char cislo_lekce[MAX_LEN];
} slovo_t;
/**
 * @brief spocita pocet slov ve slovniku
 * 
 * Funkci muzeme pouzit prave tehdy, kdyz chceme pole o
 * velikosti poctu slov ve slovniku nebo, kdyz chceme porovnavat
 * data.
 * 
 * @return pocet slov 
 */
int pocet_slov_ve_slovniku() {
    int pocet_slov = 0;

    FILE *soubor = fopen("slovnik.txt", "r");

    char radek[LINE_LEN];

    while (fgets(radek, LINE_LEN, soubor) != NULL) pocet_slov++;

    fclose(soubor); 

    return pocet_slov;
}
/**
 * @brief spocita pocet slov v dane lekci ve slovniku
 * 
 * Pouziti funkce je stejne jako u pocet_slov_ve_slovniku()
 * 
 * @param cislo_lekce 
 * @return pocet slov v lekci 
 */
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
/**
 * @brief funkce najde velka pismena a prepise je na male
 * 
 * @param slovo in/out parametr
 */
void velka_na_mala(char slovo[]) {
    for (int i = 0; i < strlen(slovo); i++) {
        if (slovo[i] >= 'A' && slovo[i] <= 'Z')
        slovo[i] += 32;
    }
}
/**
 * @brief zkontroluje, zda uzivatelsky input(slovo) neobsahuje neco jineho krom pismen
 * 
 * @param slovo uzivatelsky input
 * @return ERROR = v inputu se nachazi i neco jineho krom pismen
 * @return SUCCESS = inputem jsou pouze pismena
 */
int check_znaky(char slovo[]) {
    char slovo_f[MAX_LEN];
    
    strcpy(slovo_f, slovo);

    velka_na_mala(slovo_f);

    for (int i = 0; i < strlen(slovo_f); i++) {
        if ((slovo_f[i] < 'a' || slovo_f[i] > 'z') && (slovo_f[i] < 'A' || slovo_f[i] > 'Z')) {
            return ERROR;
        }
    }

    return SUCCESS;
}
/**
 * @brief zahlasi error v pripade, ze soubor se nevitvoril nebo neexistueje
 * 
 * @param soubor se kterym chceme pracovat
 * @return ERROR/SUCCESS
 */
int check_file_for_null(FILE *soubor) {
    if (NULL == soubor) {
        printf("slovnki je prazdny");
        return ERROR;
    }
    return SUCCESS;
}
/**
 * @brief rekne zda se slovo v souboru nachzi nebo ne
 * 
 * Funkci pouzivame, abychom se vyvarovali duplikatum ve slovniku
 * 
 * @param slovo ktere v souboru hledame
 * @return ERROR/SUCCESS
 */
int nachazi_se (char slovo[]) {
    char radek[LINE_LEN];

    FILE *soubor = fopen("slovnik.txt", "r");

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        if (strstr(radek, slovo) != NULL) return ERROR;
    }

    fclose(soubor);

    return SUCCESS;
}
void vypis_lekci(int lekce[]) {
    FILE *soubor = fopen("slovnik.txt", "r");
    
    char radek[LINE_LEN];
    int i = 0;

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        char *slovo = strtok(radek, "|");
        slovo = strtok(NULL, "|");
        char *cislo_lekce = strtok(NULL, "|");

        int cislo_lekce_int = atoi(cislo_lekce);

        if (0 == i) {
            printf("%s\n", cislo_lekce);
            lekce[i] = cislo_lekce_int;
            i++;
            continue;
        }

        int jiz_vypsano = 0;

        for (int j = 0; j < i+1; j++) {
            if (cislo_lekce_int == lekce[i]) {
                jiz_vypsano = 1;
                break;
            }
        }

        if (jiz_vypsano == 1) {
            continue;
        }

        printf("%s\n", cislo_lekce);

        lekce[i] = cislo_lekce_int;
        i++;
        jiz_vypsano = 0 ;
    }

    fclose(soubor);
}
/**
 * @brief importuje data ze souboru do pole
 * 
 * @param in_out_pole do tohoto pole budou ulozeny data ze souboru
 */
void pull_content_from_file(slovo_t in_out_pole[]) {
    FILE *soubor = fopen("slovnik.txt", "r");

    char radek[LINE_LEN];
    int index = 0;

    while (fgets(radek, LINE_LEN, soubor) != NULL) {
        char *slovo_en = strtok(radek, "|");
        char *slovo_cs = strtok(NULL, "|");
        char *cislo_lekce = strtok(NULL, "|");

        /* funkce strcpy(), ale takto => strcpy(char, *char) */
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
/**
 * @brief porovnava dve cisla char jako int
 * 
 * V jazyce C neexistuje takovato funkce, proto jsem pro ni napsal vlastni rutinu
 * 
 * @param cislo_1_char 
 * @param cislo_2_char 
 * @return -1 => 1 < 2
 * @return  0 => 1 = 2
 * @return  1 => 1 > 2
 */
int strcmp_numbers(char cislo_1_char[], char cislo_2_char[]) {
    int cislo_1 = atoi(cislo_1_char);
    int cislo_2 = atoi(cislo_2_char);

    if (cislo_1 < cislo_2)  return  -1;
    if (cislo_1 == cislo_2) return   0;
    if (cislo_1 > cislo_2)  return   1; 
}
/**
 * @brief zkopiruje data z pole typu slovo_t do jineho pole typu slovo_t
 * 
 * @param pole_1    pole, do ktereho jsou data kopirovana
 * @param pole_2    pole, z ktereho jsou data kopirovana
 * @param index_1   index prvniho pole, do ktereho kopirujeme data
 * @param index_2   index druheho pole, ze ktereho data kopirujeme
 */
void cpy_data(slovo_t pole_1[], slovo_t pole_2[], int index_1, int index_2) {
    strcpy(pole_1[index_1].slovo_en, pole_2[index_2].slovo_en);
    strcpy(pole_1[index_1].slovo_cs, pole_2[index_2].slovo_cs);
    strcpy(pole_1[index_1].cislo_lekce, pole_2[index_2].cislo_lekce);
}
/**
 * @brief prijmene pole typu slovo_t a seradi je podle cislo_lekce
 * 
 * @param in_out_pole data z tohoto pole budou serazena
 */
void serazeni_zaznamu(slovo_t in_out_pole[]) {
    int zamena;

    do {
        zamena = 0;
        for (int i = 0; i < pocet_slov_ve_slovniku() - 1; i++) {
            if ((strcmp_numbers(in_out_pole[i].cislo_lekce, in_out_pole[i+1].cislo_lekce) == 1)) {

                /*  pole pomocna nemusi byt polem, ale kvuli charakteristice funkce cpy_data nam to znatelne ulehci praci */
                slovo_t pomocna[1];
            
                cpy_data(pomocna, in_out_pole, 0, i);
                cpy_data(in_out_pole, in_out_pole, i, i+1);
                cpy_data(in_out_pole, pomocna, i+1, 0);
                
                zamena = 1;
            }
        }
    } while (zamena == 1);
}
/**
 * @brief vypise, podle cisla lekce serazeny, obsah slovniku 
 * 
 * @note doporucije prohlednout si definice pomocnych funkcich, ktere byly pouzity
 */
void vypis_slovniku_cely() {
    slovo_t zaznamy[pocet_slov_ve_slovniku()];
    
    pull_content_from_file(zaznamy);
    serazeni_zaznamu(zaznamy);

    for (int i = 0; i < pocet_slov_ve_slovniku(); i++) {
        printf("%s %s %s\n", zaznamy[i].slovo_en, zaznamy[i].slovo_cs, zaznamy[i].cislo_lekce);
    }
}
/**
 * @brief vypise ze slovniku pouze slovicka z dane lekce
 * 
 * @param cislo_lekce_in cislo lekce, jejichz slovicka, chceme vypsat
 */
void vypis_slovniku_lekce(char cislo_lekce_in[]) {
    FILE *soubor = fopen("slovnik.txt", "r");

    if (check_file_for_null(soubor)) {
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
/**
 * @brief funcke prelozi nami vlozene slovo
 * 
 * Pri vstupu nemusi byt zpecifikovano, zda se jedna o ceske nebo anglicke slovo
 * 
 * @param slovo toto chce uzivatel prelozit
 */
void preklad_slova(char slovo[]) {
    if (nachazi_se(slovo)) {
        printf("Slovo se nenachazi ve slovniku");
        return;
    }

    char radek[LINE_LEN];

    FILE *soubor = fopen("slovnik.txt", "r");

    if (check_file_for_null(soubor)) return;

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
/**
 * @brief funkce prida slovo, ktere uzivatel zadal do slovniku
 * 
 * @param slovo_en 
 * @param slovo_cs 
 * @param cislo_lekce 
 */
void pridat_do_slovniku(char slovo_en[], char slovo_cs[], char cislo_lekce[]) {
    char slovo_en_f[MAX_LEN];
    char slovo_cs_f[MAX_LEN];

    /* kdybychom do funkce velka_na_mala() vlozili uzivatelem zadana slova, zmenili by se coz je neco ceho se chceme vyvarovat */
    strcpy(slovo_en_f, slovo_en);
    strcpy(slovo_cs_f, slovo_cs);

    velka_na_mala(slovo_en_f);
    velka_na_mala(slovo_cs_f);

    FILE *soubor = fopen("slovnik.txt", "a");

    if (nachazi_se(slovo_en_f) && nachazi_se(slovo_cs_f)) {
        printf("slovo se uz nachazi ve slovniku");
        return;
    }
    
    fprintf(soubor, "%s|%s|%s|\n", slovo_en_f, slovo_cs_f, cislo_lekce);
    printf("\nslovo bylo pridano do slovniku");

    fclose(soubor);
}
/**
 * @brief vypocita prumer  
 * 
 * @param pocet_otazek 
 * @param pocet_bodu 
 * @return prumer 
 */
float hodnoceni_v_procentech(int pocet_otazek, int pocet_bodu) {
    return (pocet_bodu * 100.0) / pocet_otazek;
}
/**
 * @brief otevre soubor "statistiky.txt" a zapise do nej vysledky kazdeho testovani 
 * 
 * @param vysledek  
 */
void zapis_vysledku(float vysledek) {
    FILE *soubor = fopen("statistiky.txt", "a");

    fprintf(soubor, "%.2f%%\n", vysledek);

    fclose(soubor);
}
/**
 * @brief funkce vyzkousi uzivatele ze slovicek
 * 
 * @param cislo_lekce_CHAR slovnik je rozdelen na lekce a uzivatel si muze vybrat, ze ktere chce byt vyzkouseny
 * @param pocet_otazek 
 */
void zkouseni_ze_slovicek(char cislo_lekce_CHAR[], int pocet_otazek) {
    int pocet_otazek_f = pocet_otazek;

    /* osetreni */
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
        
        /* jestlize slovo ve slovniku neni z lekce, kterou jsme zadali, preskoci zkouseni */
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
/**
 * @brief funkce odstrani slovo ze slovniku 
 * 
 * funkce pracuje na prinicipu, kdy otevre dva souboru; z prvniho cte a do druheho zapisuje veskery kontent bez toho nami urceneho slova 
 *
 *  @param slovo 
 */
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
/**
 * @brief funkce vyzkosi uzivatele nahodne
 * 
 * @param pocet_otazek z kolika chce byt uzivatel zkouseny
 */
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
        /* generace nahodneho indexu v intervalu <0; pocet slov ve slovniku) */
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
/**
 * @brief funcke slouzi pro editaci slova 
 * 
 * funcke funguje na stejnem principu jako funkce odstraneni(), jen zde slovo nepreskocime, ale nahradime slovem upravenym
 * pro hlubsi vysvetleni viz. komentar funkce odstraneni()
 * 
 * @param slovo ktere chceme editovat
 */
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
int nabidka(void) {
    int vyber;

    printf("0 - ukoncit\n");
    printf("1 - vypis slovniku\n");
    printf("0 - ukoncit\n");
    printf("0 - ukoncit\n");
    printf("0 - ukoncit\n");
    printf("0 - ukoncit\n");
    printf("0 - ukoncit\n");

    scanf("%d", &vyber);

    return vyber;
}
int main(void) {
    switch (nabidka()) {
        case 0: break;
        case 1 : 
            vypis_slovniku_cely();
            break;
        case 2:

    }

    return 0;
}
