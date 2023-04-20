#include <malloc.h>
#include "Vzor.h"

VZOR* vzor_constructor(int vyska, int sirka) {
    VZOR* vzor = malloc(sizeof(VZOR));
    vzor->vyska = vyska;
    vzor->sirka = sirka;
    char* bunky = malloc(sizeof(char) * vyska * sirka);
    vzor->bunky = bunky;
    return vzor;
}
void vzor_destructor(VZOR* vzor) {
    free(vzor->bunky);
    vzor->bunky = NULL;

    free(vzor);
    vzor = NULL;
}
int vzor_getVyska(VZOR* vzor) {
    return vzor->vyska;
}
int vzor_getSirka(VZOR* vzor) {
    return vzor->sirka;
}
char* vzor_getBunky(VZOR* vzor) {
    return vzor->bunky;
}