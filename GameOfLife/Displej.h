#include <stdbool.h>
#include "Bunka.h"

#ifndef GAMEOFLIFE_DISPLEJ_H
#define GAMEOFLIFE_DISPLEJ_H

#define VYSKA 20
#define SIRKA 20

typedef struct displej {
    BUNKA* bunky[VYSKA][SIRKA];
} DISPLEJ;

DISPLEJ* displej_constructor();
void displej_destructor(DISPLEJ* displej);
void displej_zmenStav(DISPLEJ* displej, int x, int y, bool svieti);
bool displej_getBunkaSvieti(DISPLEJ* displej, int x, int y);
int displej_getPocetSusedovBunky(DISPLEJ* displej, int x, int y);
void displej_krok(DISPLEJ* displej);



#endif //GAMEOFLIFE_DISPLEJ_H
