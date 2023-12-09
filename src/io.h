/*
 * This file contains the definitions needed to make and store a thread
*/

#ifndef USER_TSCHED_IO_H_
#define USER_TSCHED_IO_H_

#include <stdio.h>

int nonblock_fgetc(FILE*);
char *nonblock_fgets(char*, int, FILE*);

#endif
