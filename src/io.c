#include <stdio.h>
#include <sys/select.h>
#include "io.h"
#include "thread.h"

int nonblock_fgetc(FILE *stream)
{
	int res;
	int fd = fileno(stream);
	fd_set rfds;
	struct timeval now;

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	now.tv_sec = 0;
	now.tv_usec = 0;
	while ((res = select(1, &rfds, NULL, NULL, &now)) == 0) {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		yield();
	}

	return fgetc(stream);
}

char *nonblock_fgets(char *s, int size, FILE *stream)
{
	int res;
	int fd = fileno(stream);
	fd_set rfds;
	struct timeval now;

	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	now.tv_sec = 0;
	now.tv_usec = 0;
	while ((res = select(1, &rfds, NULL, NULL, &now)) == 0) {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		yield();
	}

	return fgets(s, size, stream);
}
