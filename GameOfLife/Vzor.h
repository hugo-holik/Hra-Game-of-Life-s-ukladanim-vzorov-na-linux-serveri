#ifndef GAMEOFLIFE_VZOR_H
#define GAMEOFLIFE_VZOR_H

#include <stdbool.h>

typedef struct vzor {
    int vyska;
    int sirka;
    char* bunky;
} VZOR;

VZOR* vzor_constructor(int vyska, int sirka);
void vzor_destructor(VZOR* vzor);
int vzor_getVyska(VZOR* vzor);
int vzor_getSirka(VZOR* vzor);
char* vzor_getBunky(VZOR* vzor);


#endif //GAMEOFLIFE_VZOR_H
