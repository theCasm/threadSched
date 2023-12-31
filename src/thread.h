/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * This file contains the definitions needed to make and store a thread.
 */

#ifndef USER_TSCHED_THREAD_H_
#define USER_TSCHED_THREAD_H_

#include <stdint.h>

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
	uint8_t padding[4];
	void *xarea;
	void *SP;
	void *stack;
};


extern struct thread TTABLE[];
extern size_t currID;

extern void yield(void);
extern void threadInit(struct thread*, void*);
extern void init(void);
extern void cleanup(void);
extern size_t createThread(void (*)(void));
extern void die(void);

#endif
