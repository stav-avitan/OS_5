#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

PQueue queueCreate() {
	PQueue queue = (PQueue)malloc(sizeof(Queue));

	if (queue == NULL)
		return NULL;

	queue->head = NULL;
	queue->tail = NULL;

	pthread_mutex_init(&queue->lock, NULL);
	pthread_cond_init(&queue->cond, NULL);

	return queue;
}

void queueDestroy(PQueue queue) {
	if (queue == NULL)
		return;

	pthread_mutex_lock(&queue->lock);

	PNode node = queue->head;

	while (node != NULL)
	{
		PNode next = node->next;
		free(node->data);
		free(node);
		node = next;
	}

	pthread_mutex_unlock(&queue->lock);
	pthread_mutex_destroy(&queue->lock);
	pthread_cond_destroy(&queue->cond);

	free(queue);
}

void queueEnqueue(PQueue queue, void *data) {
	if (queue == NULL)
		return;

	PNode node = (PNode)malloc(sizeof(Node));

	if (node == NULL)
		return;

	node->data = data;
	node->next = NULL;

	pthread_mutex_lock(&queue->lock);

	if (queue->head == NULL)
	{
		queue->head = node;
		queue->tail = node;

		pthread_cond_signal(&queue->cond);
	}

	else
	{
		queue->tail->next = node;
		queue->tail = node;
	}

	pthread_mutex_unlock(&queue->lock);
}

void *queueDequeue(PQueue queue) {
	if (queue == NULL)
		return NULL;

	pthread_mutex_lock(&queue->lock);

	while (queue->head == NULL)
		pthread_cond_wait(&queue->cond, &queue->lock);

	PNode node = queue->head;
	void *data = node->data;

	queue->head = node->next;

	if (queue->head == NULL)
		queue->tail = NULL;

	free(node);

	pthread_mutex_unlock(&queue->lock);

	return data;
}

int queueIsEmpty(PQueue queue) {
	if (queue == NULL)
		return -1;

	pthread_mutex_lock(&queue->lock);
	int isEmpty = (queue->head == NULL ? 1:0);
	pthread_mutex_unlock(&queue->lock);

	return isEmpty;
}