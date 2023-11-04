#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "thread.h"

extern void yield(void);

void init(void);
void scheduleNext(void);

static void *getXArea(void);

struct thread TTABLE[NUM];
size_t currID = 0;

/*
 * Called only once by initial thread
 * set up everything, assign initial thread ID and struct
*/
void init()
{
	struct thread *first;

	for (int i = 0; i < NUM; i++) {
		TTABLE[i].state = DEAD;
	}

	first = &TTABLE[0];
	first->ID = 0;
	first->state = READY;
	first->xarea = getXArea();
	first->SP = NULL; // will be set only on yield
}

static void* getXArea()
{
	// must be 64 bit aligned
	// we are about to do a very bad, irresponsible, memory leak thing
	void *area = malloc(XAREA_SIZE + 64);
	return (void*)(((uintptr_t)area+64) & ~(uintptr_t)0xFF);
}

/*
 * This is only called from yield, after important saving has been done.
 * We only need to change currID and return to switch threads.
*/
void scheduleNext()
{
	struct thread *currThread;

	int i = (currID + 1) % NUM;
	while (TTABLE[i].state != READY) {
		i = (i+1) % NUM;
	}

	currThread = &TTABLE[i];
	if (currThread->ID == currID) {
		// no other threads available
		return;
	}

	// schedule new thread. Assume past thread is still ready.
	currID = currThread->ID;

	return;
}
