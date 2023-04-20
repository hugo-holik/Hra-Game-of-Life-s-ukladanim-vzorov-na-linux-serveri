#ifndef GAMEOFLIFE_BUNKA_H
#define GAMEOFLIFE_BUNKA_H

#include <stdbool.h>

//https://stackoverflow.com/questions/2672015/hiding-members-in-a-c-struct
typedef struct bunka {
    bool bunkaSvieti;
    int pocetSusedov;

    // 0 - prezije (ostane svietit aj dalsi krok)
    // 1 - vznikne (je zhasnuta teraz a dalsi krok sa rozsvieti)
    // 2 - zahynie (teraz svieti a prestane svietit dalsi krok)
    int stavBunkyDalsiKrok;
} BUNKA;

BUNKA* bunka_constructor();
void bunka_destructor(BUNKA* bunka);
void bunka_zmenStav(BUNKA* bunka, bool svieti);
void bunka_setStavBunkyDalsiKrok(BUNKA* bunka, int stav);

int bunka_getStavBunkyDalsiKrok(BUNKA* bunka);
bool bunka_getBunkaSvieti(BUNKA* bunka);
int bunka_getPocetSusedov(BUNKA* bunka);
void bunka_pridajPocetSusedov(BUNKA* bunka);
void bunka_uberPocetSusedov(BUNKA* bunka);

#endif //GAMEOFLIFE_BUNKA_H
