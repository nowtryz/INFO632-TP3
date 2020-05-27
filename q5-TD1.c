#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "queue.c" // c'est pas beau, mais inutile de créer un header ici

/* Constantes */
#define CONS_COUNT 0
#define PROD_COUNT 3
#define LOOP 30 / PROD_COUNT
#define CONS_NAME "/cons"
#define PROD_NAME "/prod"
#define QUEUE_NAME "/queue"
#define ID_NAME "/k"
#define QUEUE_CAPACITY PROD_COUNT
#define THREAD_COUNT PROD_COUNT
/* Global variables */
sem_t* cons, * prod, * queue_access, * k_mutex;
Queue* queue;
char depot[50];
int k = 0;

/* fonctions */
void produire(char* message)
{
    int i;

    sem_wait(k_mutex);
    i = k++;
    sem_post(k_mutex);

    sprintf(message, "Message : %d", k);
}

void deposer(char message[])
{
    sem_wait(queue_access);
    enqueue(queue, message);
    sem_post(queue_access);
}

void retirer(char* message)
{
    sem_wait(queue_access);
    dequeue(queue, message);
    sem_post(queue_access);
}

void consommer(char* message)
{
    char *s = message;
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }

    const struct timespec delay = {rand() % 3, rand() % 1000000000};
    nanosleep(&delay, NULL);

    printf("%s\n", message);
}


/* threads */
void *production(int i)
{
    char message[50];
    int j;

    printf("Thread production#%d   initialized\n", i);

    for(j = 0; j < LOOP; j++) {
        produire(message);
        sem_wait(prod);
        deposer(message);
        sem_post(cons);
    }
}

void *consommation(int i)
{
    char message[50];
    int j;

    printf("Thread consommation#%d initialized\n", i);

    for(j = 0; j < LOOP; j++) {
        sem_wait(cons);
        retirer(message);
        sem_post(prod);
        consommer(message);
    }
}


void main ()
{
    pthread_t production_threads[THREAD_COUNT], consommation_threads[THREAD_COUNT];
    int i;

    queue = createQueue(QUEUE_CAPACITY);

    /* semaphores */
    if ((cons = sem_open(CONS_NAME, O_CREAT, 0644, CONS_COUNT)) == SEM_FAILED) {
        perror("Could not initialize cons semaphore");
        exit(2);
    }
    if ((prod = sem_open(PROD_NAME, O_CREAT, 0644, PROD_COUNT)) == SEM_FAILED) {
        perror("Could not initialize prod semaphore");
        exit(2);
    }
    if ((queue_access = sem_open(QUEUE_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Could not initialize queue access semaphore");
        exit(2);
    }
    if ((k_mutex = sem_open(ID_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Could not initialize id semaphore");
        exit(2);
    }

    /* threads */
    for (i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&production_threads[i], NULL, consommation, i + 1) < 0) {
            perror("Error: thread cannot be created");
            exit(1);
        }
        if (pthread_create(&consommation_threads[i], NULL, production, i + 1) < 0) {
            perror("Error: thread cannot be created");
            exit(1);
        }
    }

    /* wait for created thread to terminate */
    for (i = 0; i < THREAD_COUNT; i++) pthread_join(production_threads[i], NULL);
    for (i = 0; i < THREAD_COUNT; i++) pthread_join(consommation_threads[i], NULL);
    /* destroy semaphore sync */
    sem_destroy(cons);
    sem_destroy(prod);

    free(queue);

    exit(0);
}
