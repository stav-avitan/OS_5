#include <stdio.h>
#include <stdlib.h>
#include "ActiveObject.h"

void *activeObjectRunFunction(void *activeObject) {
	if (activeObject == NULL)
		return NULL;

	PActiveObject ao = (PActiveObject)activeObject;
	PQueue queue = ao->queue;
	void *task = NULL;

	if (queue == NULL)
		return NULL;

	while ((task = queueDequeue(queue)) != NULL)
		ao->func(task);

	return activeObject;
}

PActiveObject CreateActiveObject(PFunc func) {
	PActiveObject activeObject = (PActiveObject)malloc(sizeof(ActiveObject));

	if (activeObject == NULL)
		return NULL;

	activeObject->queue = queueCreate();

	if (activeObject->queue == NULL)
	{
		free(activeObject);
		return NULL;
	}

	if (func == NULL)
	{
		queueDestroy(activeObject->queue);
		free(activeObject);
		return NULL;
	}

	activeObject->func = func;

	if (pthread_create(&activeObject->thread, NULL, activeObjectRunFunction, activeObject) != 0)
	{
		queueDestroy(activeObject->queue);
		free(activeObject);
		return NULL;
	}

	return activeObject;
}

PQueue getQueue(PActiveObject activeObject) {
	if (activeObject == NULL)
		return NULL;

	return activeObject->queue;
}

void stopActiveObject(PActiveObject activeObject) {
	if (activeObject == NULL)
		return;

	pthread_cancel(activeObject->thread);
	pthread_join(activeObject->thread, NULL);

	queueDestroy(activeObject->queue);
	free(activeObject);
}