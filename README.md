they will both spend

# HPC Homework 3

**Problem 1**

(a) Since for-loops are executed in two chunks, it depends on the parity of integer n that whether the two chunks are of the same size.

When **n is odd**, then two threads are assigned chunks of an equal number of function calls. Thread 0 would first execute function f() for  1+ ... +(n-1)/2  = (n+1)(n-1)/8 milliseconds. Thread 1 would execute function f() for (n+1)/2 + ... + n-1 = (3n-1)(n-1)/8 milliseconds. Therefore thread 0 will wait for thread 1 for (3n-1)(n-1)/8 - (n+1)(n-1)/8 = (n-1)^2/4 seconds. Then they change the order and do this again. So they will **both spend (3n-1)(n-1)/4 milliseconds in executing the parallel region, in which (n-1)^2/2** **milliseconds would be spent in waiting**.

When **n is even**, then the schedule(static) clause would assign a chunk with one more function call for thread 0. Thread 0 would first execute function f() for  1+ ... +n/2  = n^2/8+n/4 milliseconds. Thread 1 will execute function f() for  (n/2 + 1) + ... + n-1  = 3n^2/8-3n/4 milliseconds. So thread 0 will wait for thread 1 for 3n^2/8-3n/4 - n^2/8 - n/4 = n^2/4 - n milliseconds. Then thread 0 would execute function f() for n-1 + ... + n/2 = 3n^2/8 - n/4 milliseconds. Thread 1 would execute f() for 1+ ... +n/2 - 1 = n^2/8-n/4 milliseconds. So wait time for thread 1 would be 3n^2/8 - n/4 - n^2/8 + n/4 = n^2/4 milliseconds. So they will **both spend** 3n^2/8-3n/4 + 3n^2/8 - n/4 = **3n^2/4 - n milliseconds in executing the parallel region, in which thread 0 will wait for n^2/4 - n milliseconds and thread 1 will wait for n^2/4** **milliseconds**.

(b)

(c)

**Problem 2**

I parallelized the serial code and ran it on AMD 64-bit CPU, Intel® Core™ i5-10210U CPU, with 8 compute cores. The OS is Windows Subsystem for Linux diThe runtime with different thread number are following:

thread number = 2, run time = 0.167012s
thread number = 3, run time = 0.170121s
thread number = 4, run time = 0.236609s
thread number = 5, run time = 0.201763s
thread number = 6, run time = 0.225366s
thread number = 7, run time = 0.223382s
thread number = 8, run time = 0.222939s
thread number = 9, run time = 0.270187s
thread number = 10, run time = 0.242522s
thread number = 11, run time = 0.232214s
thread number = 12, run time = 0.246561s
thread number = 13, run time = 0.259333s
thread number = 14, run time = 0.225655s
thread number = 15, run time = 0.232036s
thread number = 16, run time = 0.228077s
