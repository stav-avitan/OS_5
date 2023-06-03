#ifndef _ACTIVE_OBJ_H
#define _ACTIVE_OBJ_H

#include "Queue.h"

typedef void (*PFunc)(void *);

typedef struct _ActiveObject {
	pthread_t thread;
	PQueue queue;
	PFunc func;
} ActiveObject, *PActiveObject;

PActiveObject CreateActiveObject(PFunc func);

PQueue getQueue(PActiveObject activeObject);

void stopActiveObject(PActiveObject activeObject);

#endif