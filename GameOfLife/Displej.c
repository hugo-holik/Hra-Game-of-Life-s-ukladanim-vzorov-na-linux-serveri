#include <malloc.h>
#include "Displej.h"

//https://stackoverflow.com/questions/14768230/malloc-for-struct-and-pointer-in-c
DISPLEJ* displej_constructor() {
    DISPLEJ* displej = malloc(sizeof(DISPLEJ));

    for (int y = 0; y < VYSKA; y++) {
        for (int x = 0; x < SIRKA; x++) {
            BUNKA* bunka = bunka_constructor(); // new Bunka();
            displej->bunky[y][x] = bunka;
        }
    }

    return displej;
}
void displej_destructor(DISPLEJ* displej) {
    for (int y = 0; y < VYSKA; y++) {
        for (int x = 0; x < SIRKA; x++) {
            bunka_destructor(displej->bunky[y][x]); // delete(...);
            displej->bunky[y][x] = NULL;
        }
    }
    free(displej);
    displej = NULL;
}

void displej_zmenStav(DISPLEJ* displej, int x, int y, bool svieti) {
    if (bunka_getBunkaSvieti(displej->bunky[y][x]) == svieti) {
        return;
    }

    bunka_zmenStav(displej->bunky[y][x], svieti);

    int susedia[8][8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    for (int i = 0; i < 8; i++) {
        int smerX = susedia[i][0];
        int smerY = susedia[i][1];

        bool podmienkaX = !(x + smerX < 0 || x + smerX > SIRKA - 1);
        bool podmienkaY = !(y + smerY < 0 || y + smerY > VYSKA - 1);

        if (podmienkaX && podmienkaY) {
            if (svieti) {
                bunka_pridajPocetSusedov(displej->bunky[y + smerY][x + smerX]);
            } else {
                bunka_uberPocetSusedov(displej->bunky[y + smerY][x + smerX]);
            }
        }
    }
}

bool displej_getBunkaSvieti(DISPLEJ* displej, int x, int y) {
    return bunka_getBunkaSvieti(displej->bunky[y][x]);
}
int displej_getPocetSusedovBunky(DISPLEJ* displej, int x, int y) {
    return bunka_getPocetSusedov(displej->bunky[y][x]);
}

void displej_krok(DISPLEJ* displej) {
    for (int y = 0; y < VYSKA; y++) {
        for (int x = 0; x < SIRKA; x++) {
            int pocetSusedov = displej_getPocetSusedovBunky(displej, x, y);

            if (pocetSusedov == 2) {
                //this.displej[y][x].stavBunkyDalsiKrok(StavBunky.PREZIJE);
                bunka_setStavBunkyDalsiKrok(displej->bunky[y][x], 0);
            } else if (pocetSusedov == 3) {
                //this.displej[y][x].stavBunkyDalsiKrok(StavBunky.VZNIKNE);
                bunka_setStavBunkyDalsiKrok(displej->bunky[y][x], 1);
            } else {
                //this.displej[y][x].stavBunkyDalsiKrok(StavBunky.ZAHYNIE);
                bunka_setStavBunkyDalsiKrok(displej->bunky[y][x], 2);
            }
        }
    }

    for (int y = 0; y < VYSKA; y++) {
        for (int x = 0; x < SIRKA; x++) {
            int pocetSusedov = displej_getPocetSusedovBunky(displej, x, y);

//            if (this.displej[y][x].getStavBunkyDalsiKrok() == StavBunky.ZAHYNIE) {
//                this.zmenStav(x, y, false);
//            } else if (this.displej[y][x].getStavBunkyDalsiKrok() == StavBunky.PREZIJE) {
//                this.zmenStav(x, y, this.displej[y][x].bunkaSvieti());
//            } else if (this.displej[y][x].getStavBunkyDalsiKrok() == StavBunky.VZNIKNE) {
//                this.zmenStav(x, y, true);
//            }

            if (bunka_getStavBunkyDalsiKrok(displej->bunky[y][x]) == 2) {
                displej_zmenStav(displej, x, y, false);
            } else if (bunka_getStavBunkyDalsiKrok(displej->bunky[y][x]) == 0) {
                displej_zmenStav(displej, x, y, bunka_getBunkaSvieti(displej->bunky[y][x]));
            } else if (bunka_getStavBunkyDalsiKrok(displej->bunky[y][x]) == 1) {
                displej_zmenStav(displej, x, y, true);
            }
        }
    }
}
