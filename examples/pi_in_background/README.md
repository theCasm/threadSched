# Pi in background
This is an example program using this userspace threads library which 
calculates pi while waiting for user input.  

The point of this example is that many programs which benefit from 
multithreading are those which wait for user input and then perform a compute intensive task.

##To run:
###step 0: make sure you've actually compiled the library, see the other README for that.  

###step 1: add the directory with libthreads.so to your runtime linkerlibrary path.  
####what?:  
just run something like  
`export LD_LIBRARY_PATH=/path/to/build:$LD_LIBRARY_PATH`  
to prepend the default build directory to your runtime linker library path.  
Everything will compile without it, but when you try to run the program you will have issues.  

###step 2: run `make` to compile  

###step 3: run it!  
