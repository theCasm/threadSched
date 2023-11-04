/*
 * This file contains the definitions needed to store a thread
*/

#ifndef SCHED_THREAD
#define SCHED_THREAD

#define NUM 16
#define XAREA_SIZE 2440

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

extern struct thread TTABLE[];
extern size_t currID;
extern void yield(void);
extern void init(void);

#endif
