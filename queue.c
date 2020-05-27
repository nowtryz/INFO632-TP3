
// C program for array implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// A structure to represent a queue
typedef struct Queue
{
    int front, rear, size;
    unsigned capacity;
    char** array;
} Queue;

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (char**) malloc(queue->capacity * sizeof(char*));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, char* item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = malloc((strlen(item) + 1) * sizeof(char));
    queue->size = queue->size + 1;
    strcpy(queue->array[queue->rear], item);
    printf("'%s' enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
char* dequeue(struct Queue* queue, char* dest)
{
    if (isEmpty(queue))
        return dest = "empty";
    strcpy(dest, queue->array[queue->front]);
    free(queue->array[queue->front]);
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return dest;
}

// Function to get front of queue
char* front(struct Queue* queue, char* dest)
{
    if (isEmpty(queue))
        return dest = "empty";
    strcpy(dest, queue->array[queue->front]);
    return dest;
}

// Function to get rear of queue
char* rear(struct Queue* queue, char* dest)
{
    if (isEmpty(queue))
        return dest = "empty";
    strcpy(dest, queue->array[queue->rear]);
    return dest;
}

 //Driver program to test above functions.
//int main()
//{
//    struct Queue* queue = createQueue(1000);
//    char str[2];
//
//    printf("'%s' dequeued from empty queue\n", dequeue(queue, str));
//
//    enqueue(queue, "aa");
//    enqueue(queue, "bb");
//    enqueue(queue, "cc");
//    enqueue(queue, "dd");
//
//    printf("%s dequeued from queue\n\n", dequeue(queue, str));
//
//    printf("Front item is '%s'\n", front(queue, str));
//    printf("Rear item is '%s'\n", rear(queue, str));
//
//    return 0;
//}
