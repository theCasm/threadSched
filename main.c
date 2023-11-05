/*
 * Test main program
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include "thread.h"

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
	readLine(buf, 256);
	if (buf[0] == 'y') {
		done();
	}

	fputs("are you sure? ", stdout);
	fflush(stdout);
	readLine(buf, 256);
	if (buf[0] == 'n') {
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
	puts("(hope that helped)");
	printf("btw the value of pi is ~%.24Lf\n", 4*pi);
	exit(0);
}

/*
 * In a nicer implementation, init would mess with the dynamically linked
 * library and replace standard functions with ones made to only block the,
 * calling thread, like this (but like the actual function and in bulk).
 * 
 * Perhaps it could only have one select-manager thing and have threads
 * mark themselves as waiting on an fd, so on a signal for an fd becomming
 * available it could simply unmark the thread
 *
 * that'd be cool I think
*/
char *readLine(char* buf, size_t n)
{
	int res;
	fd_set rfds;
	struct timeval now;
	do {
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		now.tv_sec = 0;
		now.tv_usec = 0;

		yield();
	} while ((res = select(1, &rfds, NULL, NULL, &now)) == 0);

	return fgets(buf, n, stdin);
}
