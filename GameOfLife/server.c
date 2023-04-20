#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "arraylist.h"
#include "CitacVzorov.h"


typedef struct argsObsluhaKlienta {
    int newsockfd;
    pthread_mutex_t* mutex;

} ARGS_OBSLUHA_KLIENTA;

typedef struct argsAcceptKlientov {
    int sockfd;
    socklen_t* cli_len;
    struct sockaddr_in *cli_addr;
    arraylist* listVlakien;
} ARGS_ACCEPT_KLIENTOV;


void* obsluhaKlienta(void* parameters) {
    ARGS_OBSLUHA_KLIENTA* args = (ARGS_OBSLUHA_KLIENTA*)parameters;
    int newsockfd = args->newsockfd;
    int n;

    // volba funkcionality, ktoru poskytuje server
    // volba[0]==4 - nacitanie a poslanie vzoru
    // volba[0]==1 - ukoncenie spojenia 
    int volba[1];
    do {
        //nacitanie volby od klienta
        n = read(newsockfd, volba, sizeof(int));
        //printf("nacital som volbu %d\n", volba[0]);
        if (n < 0) {
            perror("Error reading from socket");
        }

        //volba 4 je nacitanie vzorov pre klienta
        if (volba[0] == 4) {

            //printf("nacitavam ponuku vzorov\n");
            CITACVZOROV* citacVzorov = citacVzorov_constructor(NULL);
            pthread_mutex_lock(args->mutex);
            citacVzorov_nacitajMenaVzorov(citacVzorov);
            pthread_mutex_unlock(args->mutex);

            //posielam pocet vzorov klientovi
            int pocetVzorov[1];
            pocetVzorov[0] = citacVzorov_getPocetVzorov(citacVzorov);
            n = write(newsockfd, pocetVzorov, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }

            //posielam mena vzorov klientovi
            for (int i = 0; i < pocetVzorov[0]; ++i) {
                //prekopirujem meno vzoru do buffra
                char bufferMeno[250];
                char* menoVzoru = citacVzorov_getMenoVzoru(citacVzorov, i);
                for (int j = 0; j < strlen(menoVzoru); ++j) {
                    bufferMeno[j] = menoVzoru[j];
                }
                bufferMeno[strlen(menoVzoru)] = '\0';

                //posielam klientovi buffrom
                n = write(newsockfd, bufferMeno, 250);
                if (n < 0) {
                    perror("Error writing to socket");
                }
            }

            //dostanem volbu vzoru od klienta
            int indexVzoru[1];
            n = read(newsockfd, indexVzoru, sizeof(int));
            printf("nacitam vzor %d\n", indexVzoru[0]);
            if (n < 0) {
                perror("Error reading from socket");
            }


            //nacitam a poslem vzor klientovi
            pthread_mutex_lock(args->mutex);
            VZOR* vzor = citacVzorov_nacitajVzor(citacVzorov, indexVzoru[0]);
            pthread_mutex_unlock(args->mutex);
            char bunkyVzoru[400];
            for (int i = 0; i < vzor_getVyska(vzor) * vzor_getSirka(vzor); ++i) {
                bunkyVzoru[i] = vzor_getBunky(vzor)[i];
            }
            bunkyVzoru[vzor_getVyska(vzor) * vzor_getSirka(vzor)] = '\0';

            int vyska[1] = {vzor_getVyska(vzor)};
            int sirka[1] = {vzor_getSirka(vzor)};

            n = write(newsockfd, vyska, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }
            n = write(newsockfd, sirka, sizeof(int));
            if (n < 0) {
                perror("Error writing to socket");
            }
            n = write(newsockfd, bunkyVzoru, sizeof(char)*400);
            if (n < 0) {
                perror("Error writing to socket");
            }




            citacVzorov_destructor(citacVzorov);

        }
    } while (volba[0] != 1);



    //printf("uzatvaram socket clienta a koncim obsluhu\n");
    close(newsockfd);
}

void* acceptKlientov(void* parameters) {
    ARGS_ACCEPT_KLIENTOV* args = (ARGS_ACCEPT_KLIENTOV*)parameters;
    int newsockfd;
    int sockfd = args->sockfd;
    arraylist* listVlakien = args->listVlakien;
    socklen_t* cli_len = args->cli_len;
    struct sockaddr_in *cli_addr = args->cli_addr;

    while (true) {
        newsockfd = accept(sockfd, (struct sockaddr *) cli_addr, cli_len);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            //return 3;
        }


        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        ARGS_OBSLUHA_KLIENTA args2 = {newsockfd, &mutex};

        pthread_t* th1 = malloc(sizeof(pthread_t));
        pthread_create(th1, NULL, obsluhaKlienta, &args2);

        arraylist_add(listVlakien, th1);
    }
}


int server(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;


    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);




    //jadro servera  ---------------





    arraylist* listVlakien = arraylist_create();


    //tu sa vytvori vlakno acceptKLientov
    ARGS_ACCEPT_KLIENTOV args = {sockfd, &cli_len, &cli_addr, listVlakien};

    pthread_t th1;
    pthread_create(&th1, NULL, acceptKlientov, &args);                      //toto robi memory leak lebo nejoinujem vlakno (neda sa joinut, je tam nekonecny cyklus)
                                                                                                        //pthread_detach(th1); toto by malo osetrit memoryleak ale neosetri

    //server je spusteny dokym uzivatel zada x
    char input;
    do {
        printf("Pre ukoncenie servera zadajte x: \n");
        scanf("%c", &input);
    } while (input != 'x'); //strcmp(input, "x") == 0
    printf("\nCakam dokym ukoncia spojenie klienti...\n");

    //printf("som pred joinamy\n");
    //printf("pocet vlakien v arrayliste=%d\n", arraylist_size(listVlakien));
    //ked server dostane prikaz na ukoncenie -> joine vlakna - pocka dokym sa klienti vypnu - a vypne sa aj on
    for (int i = 0; i < arraylist_size(listVlakien); ++i) {
        pthread_t* th = arraylist_get(listVlakien, i);
        pthread_join(*th, NULL);
    }


    //printf("som pred nicenim arraylistov\n");
    //vyprazdnenie + znicenie arraylistov
    for (int i = 0; i < arraylist_size(listVlakien); ++i) {
        free(arraylist_get(listVlakien, i));
    }
    arraylist_destroy(listVlakien);


    printf("\nVypinam server\n");
    close(sockfd);
    return 0;
}

