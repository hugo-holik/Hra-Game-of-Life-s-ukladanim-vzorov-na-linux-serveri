#ifndef GAMEOFLIFE_HRA_H
#define GAMEOFLIFE_HRA_H

#include "Displej.h"
#include "CitacVzorov.h"

typedef struct hra {
    DISPLEJ* displej;
} HRA;

HRA* hra_constructor();
void hra_destructor(HRA* hra);
void hra_vykresli(HRA* hra);
void hra_vykresli2(HRA* hra);
void hra_krok(HRA* hra);
void hra_krok2(HRA* hra);
void hra_zmenStavBunky(HRA* hra, int x, int y);
//void hra_nakresliVzor(HRA* hra, int suradnicaX, int suradnicaY, VZOR* vzor);

void hra_spusti(HRA* hra);

#endif //GAMEOFLIFE_HRA_H
