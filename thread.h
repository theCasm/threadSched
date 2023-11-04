/*
 * This file contains the definitions needed to make and store a thread
*/

#ifndef SCHED_THREAD
#define SCHED_THREAD

#define NUM 32
#define XAREA_SIZE 2440
#define STACK_SIZE ((size_t)(2 << 19)) // 2^20 = 1 MB

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
extern size_t createThread(void (*)(void));
extern void die(void);

#endif
