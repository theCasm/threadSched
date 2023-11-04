/*
 * Test main program
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"

int main()
{
	init();

	int a = 7;
	int b = 2;
	char *test = malloc(100);
	strcpy(test, "YEEEEEEEEEHAW");

	printf("%d, %d, %s\n", a, b, test);

	// hopefully these variables maybe won't be smashed
	yield();

	printf("%d, %d, %s\n", a, b, test);

	// maybe it'll even exit gracefully too
	return 0;
}
