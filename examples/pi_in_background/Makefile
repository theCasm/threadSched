# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

CC := clang
CFLAGS := -Wall

LIBS := -lthreads
LIBDIRS := -L./../../build

main: main.o
	$(CC) $(LIBDIRS) $(LIBS) $(CFLAGS) -o main main.o

.PHONY: clean
clean:
	rm main.o
	rm main
