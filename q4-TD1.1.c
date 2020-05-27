#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

/* Constantes */
#define CONS_COUNT 0
#define PROD_COUNT 1
#define CONS_NAME "/cons"
#define PROD_NAME "/prod"
/* Global variables */
sem_t* cons;
sem_t* prod;
char depot[50];
int k = 0;

/* fonctions */
void produire(char* message)
{
    k++;
    sprintf(message, "Message : %d", k);
}

void deposer(char message[])
{
    strcpy(depot, message);
}

void retirer(char* message)
{
    strcpy(message, depot);
    strcpy(depot, "");
}

void consommer(char* message)
{
    char *s = message;
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }

    printf("%s\n", message);
}


/* threads */
void *production(void *arg)
{
    char message[50];
    int i;

    for(i = 0; i < 30; i++) {
        produire(message);
        sem_wait(prod);
        deposer(message);
        sem_post(cons);
    }
}

void *consommation(void *arg)
{
    char message[50];
    int i;

    for(i = 0; i < 30; i++) {
        sem_wait(cons);
        retirer(message);
        sem_post(prod);
        consommer(message);
    }
}


void main ()
{
    pthread_t production_thread, consommation_thread;
    /* semaphores */
    if ((cons = sem_open(CONS_NAME, O_CREAT, 0644, CONS_COUNT)) == SEM_FAILED) {
        perror("Could not initialize cons semaphore");
        exit(2);
    }
    if ((prod = sem_open(PROD_NAME, O_CREAT, 0644, PROD_COUNT)) == SEM_FAILED) {
        perror("Could not initialize prod semaphore");
        exit(2);
    }

    /* threads */
    if (pthread_create(&production_thread, NULL, consommation, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }
    if (pthread_create(&consommation_thread, NULL, production, NULL) < 0) {
        perror("Error: thread cannot be created");
        exit(1);
    }

    /* wait for created thread to terminate */
    pthread_join(production_thread, NULL);
    pthread_join(consommation_thread, NULL);
    /* destroy semaphore sync */
    sem_destroy(cons);
    sem_destroy(prod);

    exit(0);
}
