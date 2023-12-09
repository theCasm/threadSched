/*
 * Test main program
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/thread.h"
#include "../../src/io.h"

char *readLine(char*, size_t);
void somethingElse(void);
void done(void);

static long double pi;

int main()
{
	init();
	pi = 0.0;

	size_t selse = createThread(&somethingElse);

	char buf[256] = "n";
	fputs("can I help you? ", stdout);
	fflush(stdout);
	nonblock_fgets(buf, 256, stdin);
	if ((buf[0] | 0b100000) == 'y') {
		done();
	}

	fputs("are you sure? ", stdout);
	fflush(stdout);
	nonblock_fgets(buf, 256, stdin);
	if ((buf[0] | 0b100000) == 'n') {
		done();
	}

	puts(":(");
	
	cleanup();
	return 0;
}

void somethingElse()
{
	// some generic compute-bound process
	int i;
	float term;

	i = 0;
	while (1) {
		for (int j = 0; j < 500; ++j) {
			term = 1.0/(2.0*i+1.0);
			if (i & 1) term = -term;
			pi += term;
			++i;
		}
		yield();
	}
}

void done()
{
	puts("Yay!");
	printf("btw the value of pi (calculated in the background)" 
		"is ~%.24Lf\n", 4*pi);
	exit(0);
}
