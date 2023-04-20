#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Hra.h"

int userInteraction(HRA* hra, int sockfd) {
    printf("Co by ste chceli vykonat?\n");
    printf("1. ukoncit aplikaciu\n");
    printf("2. rozsvietit/zhasnut bunku\n");
    printf("3. vykonat n krokov\n");
    printf("4. vybrat a nakreslit z ponuknutych vzorov\n");
    printf("5. spustit simulaciu ('s' - pause/resume, 'q' - koniec simulacie)\n");
    printf("Zadajte cislo vyberu: ");
    int volba = 0;
    scanf("%d", &volba);
    printf("\n");

    switch (volba) {

        case 1: ;
            //PRVY WRITE -> dava informaciu serveru o volbe funkcionality
            int volba[1] = {1};
            int n = write(sockfd, volba, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }
            return 1; // return 1 -> ukoncit hru

            break;

        case 2:
            printf("Zadajte suradnicu x: ");
            int x = 0;
            scanf("%d", &x);
            printf("\n");

            printf("Zadajte suradnicu y: ");
            int y = 0;
            scanf("%d", &y);
            printf("\n");

            hra_zmenStavBunky(hra, x, y);
            return 0;
            break;

        case 3:
            printf("Zadajte pocet krokov: ");
            int pocet = 0;
            scanf("%d", &pocet);
            printf("\n");
            for (int i = 0; i < pocet; ++i) {
                hra_krok(hra);
            }
            return 0;
            break;

        case 4:
            printf("\n");
            printf("Ponuka vzorov: \n");

            //PRVY WRITE -> dava informaciu serveru o volbe funkcionality
            volba[0] = 4;
            n = write(sockfd, volba, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }

            //dostanem od servera pocet vzorov
            int pocetVzorov[1];
            n = read(sockfd, pocetVzorov, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }
            printf("pocet vzorov: %d\n", pocetVzorov[0]);

            //postupne nacitam od servera mena vzorov a nahadzem do citacVzorov
            CITACVZOROV *citacVzorov = citacVzorov_constructor(hra->displej);
            for (int i = 0; i < pocetVzorov[0]; ++i) {
                char bufferMeno[250];
                n = read(sockfd, bufferMeno, 250);
                if (n < 0) {
                    perror("Error writing to socket");
                }
                //printf("%s strlen=%lu\n", bufferMeno, strlen(bufferMeno));

                char* menoVzoru = malloc(strlen(bufferMeno) * sizeof(char));
                strcpy(menoVzoru, bufferMeno);
                citacVzorov_pridajMenoVzoru(citacVzorov, menoVzoru);
            }
            for (int i = 0; i < citacVzorov_getPocetVzorov(citacVzorov); ++i) {
                printf("%d. %s\n", i, citacVzorov_getMenoVzoru(citacVzorov, i));
            }

            printf("Zadajte cislo vzoru: ");
            int cisloVzoru = 0;
            scanf("%d", &cisloVzoru);
            printf("\n");

            //poslem serveru zvoleny index vzoru
            int indexVzoru[1];
            indexVzoru[0] = cisloVzoru;
            n = write(sockfd, indexVzoru, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }

            //dostanem od servera vzor (sirku, vysku, bunky)
            int vyska[1];
            n = read(sockfd, vyska, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }
            int sirka[1];
            n = read(sockfd, sirka, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }
            char bunkyVzoru[400];
            n = read(sockfd, bunkyVzoru, 400);
            if (n < 0) {
                perror("Error writing to socket");
            }
            //printf("vyska: %d\n", vyska[0]);
            //printf("sirka: %d\n", sirka[0]);
            //printf("telo: %s\n", bunkyVzoru);

            //vytvorim vzor z nacitanych dat
            VZOR *vzor = vzor_constructor(vyska[0], sirka[0]);
            for (int i = 0; i < vzor_getVyska(vzor) * vzor_getSirka(vzor); ++i) {
                vzor_getBunky(vzor)[i] = bunkyVzoru[i];
            }
            vzor_getBunky(vzor)[vzor_getVyska(vzor) * vzor_getSirka(vzor)] = '\0';
            //printf("nacitane bunky vzoru od servera:\n%s\n", vzor_getBunky(vzor));


            printf("Zadajte suradnicu x: ");
            x = 0;
            scanf("%d", &x);
            printf("\n");

            printf("Zadajte suradnicu y: ");
            y = 0;
            scanf("%d", &y);
            printf("\n");

            citacVzorov_nakresliVzor(citacVzorov, x, y, vzor);
            hra_vykresli(hra);

            vzor_destructor(vzor);
            citacVzorov_destructor(citacVzorov);
            return 0;
            break;

        case 5:
            hra_spusti(hra);
            return 0;
            break;

        default:
            return 1;
    }
}

int client(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }

    //jadro klienta ----------------


    HRA* hra = hra_constructor();
    int returnHodnota = 0;
    while (returnHodnota != 1) {
        returnHodnota = userInteraction(hra, sockfd);
    }
    hra_destructor(hra);


    //jadro klienta end --------------

    close(sockfd);
    return 0;
}

