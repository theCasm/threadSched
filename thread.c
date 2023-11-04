#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
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
		TTABLE[i].xarea = NULL;
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

/*
 * API call - actually make a thread.
 * Will *not* run the thread. Just make one capable of running.
 * Caller can yield to make their thread work.
 *
 * RETURNS: new thread ID.
*/
size_t createThread(void (*func)(void))
{
	void *stack;

	size_t i = currID + 1;
	while (TTABLE[i].state != DEAD && i != currID) {
		i = (i+1) % NUM;
	}

	// check if we actually had a thread available
	if (i == currID) {
		// uh oh
		fputs("Ran out of threads :(\n", stderr);
		exit(1);
	}

	// TODO: not leak this memory
	stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, 
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (stack == -1) {
		fprintf(stderr, "Mmap failed: %s\n", strerror(errno));
		exit(1);
	}

	TTABLE[i].ID = i;
	TTABLE[i].state = READY;
	if (TTABLE[i].xarea == NULL)
		TTABLE[i].xarea = getXArea();
	TTABLE[i].SP = stack + STACK_SIZE - 16 - 120;

	
	// rearrange stack so it has fake savestate, dies upon finishing
	*(void**)(stack + STACK_SIZE - 8) = &die;
	*(void**)(stack + STACK_SIZE - 16) = func;

	return i;
}

void die()
{
	// because we don't do any interrupt shenanagins currID is the
	// recently deceased thread

	TTABLE[currID].state = DEAD;


	// will never return because its dead, so we're good
	// it will be recycled eventually
	yield();
}
