#ifndef GAMEOFLIFE_CITACVZOROV_H
#define GAMEOFLIFE_CITACVZOROV_H

#include "Displej.h"
#include "arraylist.h"
#include "Vzor.h"

typedef struct citacVzorov {
    DISPLEJ* displej;
    arraylist* menaVzorov;
} CITACVZOROV;

CITACVZOROV* citacVzorov_constructor(DISPLEJ* displej);
void citacVzorov_destructor(CITACVZOROV* citacVzorov);
void citacVzorov_nacitajMenaVzorov(CITACVZOROV* citacVzorov);
VZOR* citacVzorov_nacitajVzor(CITACVZOROV* citacVzorov, int indexVzoru);

void citacVzorov_nakresliVzor(CITACVZOROV* citacVzorov, int suradnicaX, int suradnicaY, VZOR* vzor);

int citacVzorov_getPocetVzorov(CITACVZOROV* citacVzorov);
char* citacVzorov_getMenoVzoru(CITACVZOROV* citacVzorov, int indexVzoru);

void citacVzorov_pridajMenoVzoru(CITACVZOROV* citacVzorov, char* menoVzoru);

#endif //GAMEOFLIFE_CITACVZOROV_H
