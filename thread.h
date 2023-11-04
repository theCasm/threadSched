/*
 * This file contains the definitions needed to store a thread
*/

#ifndef SCHED_THREAD
#define SCHED_THREAD

extern void yield();
extern void init();

enum state {
	DEAD,
	WAITING,
	READY
};

struct thread {
	size_t ID;
	enum state state;
	void *xarea;
	void *SP;
};

#endif
