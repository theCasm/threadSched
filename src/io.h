/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef USER_TSCHED_IO_H_
#define USER_TSCHED_IO_H_

#include <stdio.h>

ssize_t nonblock_read(int, void*, size_t);
int nonblock_fgetc(FILE*);
char *nonblock_fgets(char*, int, FILE*);

#endif
