/*
 * Test main program
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"

void myThread(void);
void myThread2(void);
void myThread3(void);

int main()
{
	init();

	int a = 7;
	int b = 2;
	char *test = malloc(100);
	strcpy(test, "YEEEEEEEEEHAW");

	printf("%d, %d, %s\n", a, b, test);

	for (int i = 0; i < 10; ++i) {
		size_t id = createThread(&myThread);
	}

	printf("%d, %d, %s\n", a, b, test);

	for (int i = 0; i < 100; ++i)
		yield(); // let my babies run until they're prob all done
	
	return 0;
}

void myThread()
{
	puts("YOOOOOOOOO");
	size_t jamie = createThread(&myThread2);
	size_t alfred = createThread(&myThread3);
	yield();
	puts("getting sleepy...");
	return;
}

void myThread2()
{
	puts("yo");
	yield();
	puts("not done yet!");
	return;
}

void myThread3()
{
	puts("ARGHGHHHHHHHH");
	return;
}
