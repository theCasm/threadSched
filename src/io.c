/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * This file contains replacements for blocking IO funcitons.
 * In practice, this will likely just be equivilant to writing a much
 * worse version of glibc. If there was a way to see the number of chars
 * available for reading, we could just call glibc with that num and
 * all would be well. Unfortunately, I can't find a way to do that - if
 * you can, please let me know!
*/


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include "io.h"
#include "thread.h"

static void initFD(int);
static char *extract_delim(char*, char*, char);

static void initFD(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static char *extract_delim(char *start, char *end, char delim)
{
	while (start < end) {
		if (*start++ == delim) {
			return start;
		}
	}

	return (char *)-1;
}

ssize_t nonblock_read(int fd, void *buf, size_t count)
{
	ssize_t res;

	initFD(fd);
	while ((res = read(fd, buf, count)) == -1) {
		if (errno != EAGAIN && errno !=  EWOULDBLOCK) {
			return -1;
		}
		yield();
	}

	return res;
}

int nonblock_fgetc(FILE *stream)
{
	ssize_t res; 
	int fd = fileno(stream);
	char c[1];

	res = nonblock_read(fd, &c, 1);
	return (res != -1) ? c[0] : -1;
}

char *nonblock_fgets(char *s, int size, FILE *stream)
{
	ssize_t res; 
	int c = 0;
	char *delim;
	int fd = fileno(stream);

	while (size > 1) {
		res = nonblock_read(fd, s + c, (size_t)size);

		if ((delim = extract_delim(s + c, s + c + res, '\n')) != (char *)-1) {
			*(delim + 1) = '\0';
			return s;
		}
		
		c += res;
		size -= res;
	}
	s[size - 1] = '\0';

	return s;
}
