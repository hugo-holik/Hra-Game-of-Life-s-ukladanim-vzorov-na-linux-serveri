#include <malloc.h>
#include "Bunka.h"

BUNKA* bunka_constructor() {
    BUNKA* bunka = malloc(sizeof(BUNKA));
    if (bunka == NULL) {
        return NULL;
    }
    bunka->bunkaSvieti = false;
    bunka->pocetSusedov = 0;
    return bunka;
}
void bunka_destructor(BUNKA* bunka) {
    free(bunka);
    bunka = NULL;
}

void bunka_zmenStav(BUNKA* bunka, bool svieti) {
    bunka->bunkaSvieti = svieti;
}

void bunka_setStavBunkyDalsiKrok(BUNKA* bunka, int stav) {
    bunka->stavBunkyDalsiKrok = stav;
}

int bunka_getStavBunkyDalsiKrok(BUNKA* bunka) {
    return bunka->stavBunkyDalsiKrok;
}
bool bunka_getBunkaSvieti(BUNKA* bunka) {
    return bunka->bunkaSvieti;
}
int bunka_getPocetSusedov(BUNKA* bunka) {
    return bunka->pocetSusedov;
}
void bunka_pridajPocetSusedov(BUNKA* bunka) {
    bunka->pocetSusedov++;
}
void bunka_uberPocetSusedov(BUNKA* bunka) {
    bunka->pocetSusedov--;
}