#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "thread.h"

#define NUM 16
#define XAREA_SIZE 2440

struct thread TTABLE[NUM];
size_t curr = 0;

extern void yield();

static void* getXArea();

void init();

/*
 * Called only once by initial thread
 * set up everything, assign initial thread ID and struct
*/
void init()
{
	for (int i = 0; i < NUM; i++) {
		TTABLE[i].state = DEAD;
	}

	struct thread *first = &TTABLE[0];
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
	return (void*)((uintptr_t)(area+64) & ~(uintptr_t)0xFF);
}
