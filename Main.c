#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
	#if __STDC_VERSION__ >= 199901L
		#define _XOPEN_SOURCE 600   /* SUS v3, POSIX 1003.1 2004 (POSIX 2001 + Corrigenda) */
	#else
		#define _XOPEN_SOURCE 500   /* SUS v2, POSIX 1003.1 1997 */
	#endif /* __STDC_VERSION__ */
#endif /* !_XOPEN_SOURCE && !_POSIX_C_SOURCE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "ActiveObject.h"

typedef struct _Task {
	unsigned int num_of_tasks;
	unsigned int _data;
} Task, *PTask;

PActiveObject ActiveObjects[4] = { NULL };

int check_prime(unsigned int num) {
	if (num <= 2 || num % 2 == 0)
		return 0;

	unsigned int root = (unsigned int)sqrt(num);

	for (unsigned int i = 3; i <= root; i += 2)
	{
		if (num % i == 0)
			return 0;
	}

	return 1;
}

void Task1(void* task) {
	PTask task_init = (PTask)task;

	unsigned int n = task_init->num_of_tasks, seed = task_init->_data;

	srand(seed != 0 ? seed : time(NULL));

	for (unsigned int i = 0; i < n; i++)
	{
		unsigned int num = (rand() % 900000) + 100000;

		PTask task_data = (PTask)malloc(sizeof(Task));

		if (task_data == NULL)
			exit(1);

		task_data->num_of_tasks = n;
		task_data->_data = num;

		queueEnqueue(getQueue(ActiveObjects[1]), task_data);

		usleep(1000);
	}

	stopActiveObject(ActiveObjects[0]);
}

void Task2(void* task) {
	static unsigned int count = 0;
	PTask task_data = (PTask)task;

	unsigned int iterations = task_data->num_of_tasks, num = task_data->_data;

	fprintf(stdout, "%u\n%s\n", num, check_prime(num) ? "true" : "false");

	task_data->_data += 11;

	queueEnqueue(getQueue(ActiveObjects[2]), task_data);

	if (iterations <= ++count)
		stopActiveObject(ActiveObjects[1]);
}

void Task3(void* task) {
	static unsigned int count = 0;
	PTask task_data = (PTask)task;

	unsigned int iterations = task_data->num_of_tasks, num = task_data->_data;

	fprintf(stdout, "%u\n%s\n", num, check_prime(num) ? "true" : "false");

	task_data->_data -= 13;

	queueEnqueue(getQueue(ActiveObjects[3]), task_data);

	if (iterations <= ++count)
		stopActiveObject(ActiveObjects[2]);
}

void Task4(void* task) {
	static unsigned int count = 0;
	PTask task_data = (PTask)task;

	unsigned int iterations = task_data->num_of_tasks, num = task_data->_data;

	printf("%u\n%s\n%u\n", num, check_prime(num) ? "true" : "false", (num + 2));
	free(task_data);

	if (iterations <= ++count)
		stopActiveObject(ActiveObjects[3]);
}

int main(int argc, char **args) {
	if (argc > 3 || argc < 2) {
		printf("Usage: %s <n> [<seed>]\n", *args);
		return 1;
	}

	unsigned int n = atoi(args[1]);
	unsigned int seed = 0;

	if (argc == 3) {
		seed = atoi(args[2]);

		if (seed == 0) {
			printf("Seed must be positive!");
			return 1;
		}
	}

	if (n == 0) {
		printf("Number must be positive!");
		return 1;
	}

	ActiveObjects[0] = CreateActiveObject(Task1);
	ActiveObjects[1] = CreateActiveObject(Task2);
	ActiveObjects[2] = CreateActiveObject(Task3);
	ActiveObjects[3] = CreateActiveObject(Task4);

	PTask task_data = (PTask)malloc(sizeof(Task));

	if (task_data == NULL)
		exit(1);

	task_data->num_of_tasks = n;
	task_data->_data = seed;

	queueEnqueue(getQueue(ActiveObjects[0]), task_data);

	pthread_join(ActiveObjects[0]->thread, NULL);
	pthread_join(ActiveObjects[1]->thread, NULL);
	pthread_join(ActiveObjects[2]->thread, NULL);
	pthread_join(ActiveObjects[3]->thread, NULL);
	

	return 0;
}