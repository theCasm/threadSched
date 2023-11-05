CC := clang
CFLAGS := -Wall
ASM := nasm
ASMFLAGS := -felf64

main: build/thread.c.o build/yield.a.o build/main.c.o
	$(CC) $^ -o build/main

build/%.c.o: %.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.a.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@ 

clean:
	rm build/*.o
	rm build/main
