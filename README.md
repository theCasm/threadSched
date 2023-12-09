# threadSched
Simple userspace nonpreemptive thread schedular. So simple I feel bad calling it a schedular, all the scheduling takes maybe 5 lines of code.  

I was supposed to be writing an essay instead of this but yk life happens. I personally think my approach is very cool.  

## To build:
enter the src directory  
run `make` to run the Makefile using default settings  
the generated libthreads.so file will be in the build directory  

## To use:
For testing purposes, check out how the examples found in the examples directory do it. For a proper installation, see below. Note that this will require root privileges.  
### Installing the library
1. Copy libthreads.so from the build dir to /usr/lib  
2. Set permissions with `chmod 0755 /usr/lib/libthreads.so`.
3. Update loader cache by running `ldconfig`.
4. If you have already tried the library the temporary way, clean everything up; `unset LD_LIBRARY_PATH` and re-link programs, this time without using the -L option.
5. Giv'er a go! You will need to specify `-llibthreads.so` when linking your program, and you will need to get the header files somehow. To install these too:  
### Installing header files
1. create a directory in /usr/include with a unique memorable name, such as `user_threads`  
2. move the header files from the src directory into this directory  
3. reference the header files in source code as <user_threads/header.h>
