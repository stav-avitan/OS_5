#ifndef _QUEUE_H
#define _QUEUE_H

#include <pthread.h>

typedef struct _Node {
    void *data;
    struct _Node *next;
} Node, *PNode;

typedef struct _Queue
{
    PNode head;
    PNode tail;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Queue, *PQueue;

PQueue queueCreate();

void queueDestroy(PQueue queue);

void queueEnqueue(PQueue queue, void *data);

void *queueDequeue(PQueue queue);

int queueIsEmpty(PQueue queue);


#endif