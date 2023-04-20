
#include <malloc.h>
#include <unistd.h>
#include "Hra.h"

#include <pthread.h>
#include <curses.h>
#include <unistd.h>

typedef struct args {
    bool isRunning;
    bool isTerminated;
    HRA* hra;
} ARGS;

void* keyListener(void* parameter) {
    ARGS* args = (ARGS*)parameter;
    //printw("som v start()\n");
    int a;
    while (!args->isTerminated) {
        a = getch();
        //q - koniec
        if (*(keyname(a)) == 'q') {
            args->isTerminated = true;
        }
        // s - pause/resume
        if (*(keyname(a)) == 's') {
            args->isRunning = !args->isRunning;
        }
    }
    //printw("koncim start()\n");
    return NULL;
}

void* print(void* parameter) {
    //printw("Som v print()\n");
    ARGS* args = (ARGS*)parameter;
    while (!args->isTerminated) {
        while (args->isRunning) {
            hra_krok2(args->hra);
        }
    }
    //printw("koncim print()\n");
}

HRA* hra_constructor() {
    HRA* hra = malloc(sizeof(HRA));

    DISPLEJ* displej = displej_constructor();
    hra->displej = displej;



    return hra;
}
void hra_destructor(HRA* hra) {
    displej_destructor(hra->displej); // delete(...);
    hra->displej = NULL;


    free(hra);
    hra = NULL;
}

void hra_vykresli(HRA* hra) {
    for (int y = 0; y < VYSKA; y++) {
        for (int x = 0; x < SIRKA; x++) {
            if (displej_getBunkaSvieti(hra->displej, x, y)) {
                printf("@ ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void hra_vykresli2(HRA* hra) {
    for (int y = 0; y < VYSKA; y++) {
        for (int x = 0; x < SIRKA; x++) {
            if (displej_getBunkaSvieti(hra->displej, x, y)) {
                printw("@ ");
            } else {
                printw(". ");
            }
        }
        printw("\n");
    }
    //printw("\n");
}

void hra_zmenStavBunky(HRA* hra, int x, int y) {
    if (displej_getBunkaSvieti(hra->displej, x, y)) {
        displej_zmenStav(hra->displej, x, y, false);
    } else {
        displej_zmenStav(hra->displej, x, y, true);
    }
    hra_vykresli(hra);
}

void hra_krok(HRA* hra) {
    displej_krok(hra->displej);
    usleep(1000000/4);
    hra_vykresli(hra);
}

void hra_krok2(HRA* hra) {
    displej_krok(hra->displej);
    usleep(1000000/4);
    hra_vykresli2(hra);
}



//void hra_nakresliVzor(HRA* hra, int suradnicaX, int suradnicaY, int indexVzoru) {
//    citacVzorov_nakresliVzor(hra->citacVzorov, suradnicaX, suradnicaY, indexVzoru);
//    hra_vykresli(hra);
//}

void hra_spusti(HRA* hra) {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    setlinebuf(stdout);

    ARGS args = {true, false, hra};

    //vedlajsie vlakno s logikou + keylistenery
    pthread_t th1;
    pthread_create(&th1, NULL, &keyListener, &args);

    //hlavne vlakno s vypisom
    print(&args);

    pthread_join(th1, NULL);

    endwin();

}




