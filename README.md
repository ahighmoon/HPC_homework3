they will both spend

# HPC Homework 3

**Problem 1**

(a) Since for-loops are executed in two chunks, it depends on the parity of integer n that whether the two chunks are of the same size.

When **n is odd**, then two threads are assigned chunks of an equal number of function calls. Thread 0 would first execute function f() for  1+ ... +(n-1)/2  = (n+1)(n-1)/8 milliseconds. Thread 1 would execute function f() for (n+1)/2 + ... + n-1 = (3n-1)(n-1)/8 milliseconds. Therefore thread 0 will wait for thread 1 for (3n-1)(n-1)/8 - (n+1)(n-1)/8 = (n-1)^2/4 seconds. Then they change the order and do this again. So they will **both spend (3n-1)(n-1)/4 milliseconds in executing the parallel region, in which (n-1)^2/2** **milliseconds would be spent in waiting**.

When **n is even**, then the schedule(static) clause would assign a chunk with one more function call for thread 0. Thread 0 would first execute function f() for  1+ ... +n/2  = n^2/8+n/4 milliseconds. Thread 1 will execute function f() for  (n/2 + 1) + ... + n-1  = 3n^2/8-3n/4 milliseconds. So thread 0 will wait for thread 1 for 3n^2/8-3n/4 - n^2/8 - n/4 = n^2/4 - n milliseconds. Then thread 0 would execute function f() for n-1 + ... + n/2 = 3n^2/8 - n/4 milliseconds. Thread 1 would execute f() for 1+ ... +n/2 - 1 = n^2/8-n/4 milliseconds. So wait time for thread 1 would be 3n^2/8 - n/4 - n^2/8 + n/4 = n^2/4 milliseconds. So they will **both spend** 3n^2/8-3n/4 + 3n^2/8 - n/4 = **3n^2/4 - n milliseconds in executing the parallel region, in which thread 0 will wait for n^2/4 - n milliseconds and thread 1 will wait for n^2/4** **milliseconds**.

(b) 

(c)
