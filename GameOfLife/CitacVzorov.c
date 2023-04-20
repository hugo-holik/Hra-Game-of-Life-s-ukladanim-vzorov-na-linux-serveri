
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "CitacVzorov.h"
#include "arraylist.h"

CITACVZOROV* citacVzorov_constructor(DISPLEJ* displej) {
    CITACVZOROV* citacVzorov = malloc(sizeof(CITACVZOROV));
    citacVzorov->displej = displej;

    arraylist* menaVzorov = arraylist_create();
    citacVzorov->menaVzorov = menaVzorov;

    //citacVzorov_nacitajMenaVzorov(citacVzorov);

    return citacVzorov;
}

void citacVzorov_destructor(CITACVZOROV* citacVzorov) {
    //lebo stringy boli alokovane dynamicky
    for (int i = 0; i < arraylist_size(citacVzorov->menaVzorov); ++i) {
        free(arraylist_get(citacVzorov->menaVzorov, i));
    }
    arraylist_destroy(citacVzorov->menaVzorov);

    free(citacVzorov);
}

void citacVzorov_nacitajMenaVzorov(CITACVZOROV* citacVzorov) {

    FILE* fp;
    fp = fopen("objekty_life.txt", "r");
    if (fp == NULL) {
        printf("Nepodarilo sa otvorit subor");
    }

    int MAX_LEN = 250;
    char buffer[MAX_LEN];

    while (fgets(buffer, MAX_LEN, fp)) { //fgets preskoci prazdny riadok
        fgets(buffer, MAX_LEN, fp); //fgets zobere meno
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;


        char* menoVzoru = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(menoVzoru, buffer);
        //printf("%s\n", menoVzoru);

        arraylist_add(citacVzorov->menaVzorov, menoVzoru);
        //printf("%s\n", arraylist_get(citacVzorov->menaVzorov, 0));

        int vyska = 0;
        fscanf(fp, "%*d %d", &vyska);
        fgets(buffer, MAX_LEN, fp); //fgets preskoci riadok (lebo fscanf po nacitani neskoci na novy riadok)
        //printf("%d", vyska);
        for (int i = 0; i < vyska; ++i) {
            fgets(buffer, MAX_LEN, fp); //fgets preskoci riadok
        }
    }


    fclose(fp);

//    for (int i = 0; i < arraylist_size(listStringov); ++i) {
//        printf("%s\n", (char*)arraylist_get(listStringov, i));
//    }
}

VZOR* citacVzorov_nacitajVzor(CITACVZOROV* citacVzorov, int indexVzoru) {
    FILE* fp;
    fp = fopen("objekty_life.txt", "r");
    if (fp == NULL) {
        printf("Nepodarilo sa otvorit subor");
    }

    int MAX_LEN = 250;
    char buffer[MAX_LEN];
    char* menoVzoru = arraylist_get(citacVzorov->menaVzorov,indexVzoru);

    while (fgets(buffer, MAX_LEN, fp)) { //fgets preskoci prazdny riadok
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, menoVzoru) == 0) {
            int vyska = 0;
            int sirka = 0;
            fscanf(fp, "%d %d", &sirka, &vyska);

            fgets(buffer, MAX_LEN, fp); //fgets preskoci riadok (lebo fscanf po nacitani neskoci na novy riadok)

            VZOR* vzor = vzor_constructor(vyska, sirka);

            for (int i = 0; i < vyska; i++) {
                fgets(buffer, MAX_LEN, fp);
                buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

                for (int j = 0; j < sirka; j++) {
                    vzor->bunky[(i * sirka) + j] = buffer[j];
                }
            }

            fclose(fp);
            return vzor;
        }
    }

    fclose(fp);
    return NULL;
}

void citacVzorov_nakresliVzor(CITACVZOROV* citacVzorov, int suradnicaX, int suradnicaY, VZOR* vzor) {

    int vyska = vzor_getVyska(vzor);
    int sirka = vzor_getSirka(vzor);

    for (int i = 0; i < vyska; i++) {
        for (int j = 0; j < sirka; j++) {
            bool bunkaSvieti = vzor->bunky[i*sirka + j] == '*';
            displej_zmenStav(citacVzorov->displej, suradnicaX + j, suradnicaY + i, bunkaSvieti);
        }
    }

    //vzor_destructor(vzor);
}

int citacVzorov_getPocetVzorov(CITACVZOROV* citacVzorov) {
    return arraylist_size(citacVzorov->menaVzorov);
}

char* citacVzorov_getMenoVzoru(CITACVZOROV* citacVzorov, int indexVzoru) {
    return arraylist_get(citacVzorov->menaVzorov, indexVzoru);
}

void citacVzorov_pridajMenoVzoru(CITACVZOROV* citacVzorov, char* menoVzoru) {
    arraylist_add(citacVzorov->menaVzorov, menoVzoru);
}