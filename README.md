they will both spend

# HPC Homework 3

**Problem 1**

(a) Since for-loops are executed in two chunks, it depends on the parity of integer n that whether the two chunks are of the same size.

When **n is odd**, then two threads are assigned chunks of an equal number of function calls. Thread 0 would first execute function f() for  1+ ... +(n-1)/2  = (n+1)(n-1)/8 milliseconds. Thread 1 would execute function f() for (n+1)/2 + ... + n-1 = (3n-1)(n-1)/8 milliseconds. Therefore thread 0 will wait for thread 1 for (3n-1)(n-1)/8 - (n+1)(n-1)/8 = (n-1)^2/4 seconds. Then they change the order and do this again. So they will both spend (3n-1)(n-1)/4 milliseconds in executing the parallel region, in which (n-1)^2/2 **milliseconds would be spent in waiting**.

When **n is even**, then the schedule(static) clause would assign a slightly larger chunk for thread 0. Thread 0 would execute function f() for  1+ ... +n/2 + n-1 + ... + n/2 = n^2/2 milliseconds.  Thread 1 would execute function f() for (n/2+1) + ... + n-1+ (n/2-1) + ... + 1 = n(n-2)/2 milliseconds. Since they will abort after the whole thing, **they will both spend n^2-n milliseconds executing the parallel region**, in which **thread 0 will spend n(n-2)/2 milliseconds in waiting** for thread 1, and **thread 1 will spend n^2/2 milliseconds in waiting** for thread 0.

（b) **It would not change.**

When n is odd, thread 0 would execute function f() for  1+3 ... +n-2 + n-1 + ... + 4+ 2 = n(n-1)/2 milliseconds.  Thread 1 would execute function f() for 2 + 4 + ... + n-1+ n-2 + ... +3 + 1 = n(n-1)/2 milliseconds. Since they will abort after the whole thing, **they will both spend n^2-n milliseconds executing the parallel region,** in which **n(n-1)/2 milliseconds would be spent in waiting** for the other thread.

When **n is even**, then the schedule(static) clause would assign a slightly larger chunk for thread 0. Thread 0 would execute function f() for  1+ 3+... +n/2 + n-1 + n-1+ ... +3+ 1 = n^2/2 milliseconds.  Thread 1 would execute function f() for 2+4 + ... + n-2+ n-2 + ... + 2 = n(n-2)/2 milliseconds. Since they will abort after the whole thing, **they will both spend n^2-n milliseconds executing the parallel region**, in which **thread 0 will spend n(n-2)/2 milliseconds in waiting** for thread 1, and **thread 1 will spend n^2/2 milliseconds in waiting** for thread 0.

(c)
