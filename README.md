they will both spend

# HPC Homework 3

**Problem 1**

(a) Since for-loops are executed in two chunks, it depends on the parity of integer n that whether the two chunks are of the same size.

When ***n is odd***, then two threads are assigned chunks of an equal number of function calls. Thread 0 would first execute function f() for  1+ ... +(n-1)/2  = (n+1)(n-1)/8 milliseconds. Thread 1 would execute function f() for (n+1)/2 + ... + n-1 = (3n-1)(n-1)/8 milliseconds. Therefore thread 0 will wait for thread 1 for (3n-1)(n-1)/8 - (n+1)(n-1)/8 = (n-1)^2/4 seconds. Then they change the order and do this again. So they will ***both spend (3n-1)(n-1)/4 milliseconds in executing the parallel region, in which (n-1)^2/2** **milliseconds would be spent in waiting***.

When ***n is even***, then the schedule(static) clause would assign a chunk with one more function call for thread 0. Thread 0 would first execute function f() for  1+ ... +n/2  = n^2/8+n/4 milliseconds. Thread 1 will execute function f() for  (n/2 + 1) + ... + n-1  = 3n^2/8-3n/4 milliseconds. So thread 0 will wait for thread 1 for 3n^2/8-3n/4 - n^2/8 - n/4 = n^2/4 - n milliseconds. Then thread 0 would execute function f() for n-1 + ... + n/2 = 3n^2/8 - n/4 milliseconds. Thread 1 would execute f() for 1+ ... +n/2 - 1 = n^2/8-n/4 milliseconds. So wait time for thread 1 would be 3n^2/8 - n/4 - n^2/8 + n/4 = n^2/4 milliseconds. So they will ***both spend 3n^2/8-3n/4 + 3n^2/8 - n/4 = 3n^2/4 - n milliseconds in executing the parallel region, in which thread 0 will wait for n^2/4 - n milliseconds and thread 1 will wait for n^2/4 milliseconds.***

(b) When we use the (static, 1) schedule, the first for-loop of calling f() always exhibit a similar pattern. For example, when thread 0 executes f(1), f(3), ..., f(11) and thread 1 executes f(2), f(4), ..., f(12), they always gets tasks and runs interchangebly. There is never the case when one thread gets two continous (next to each other) tasks and runs them. So the run time for first loop is also somehow fixed in this sense. When n is odd, n-1 is even, and runtime would be 2 + 4 + ... + n-1 = (n-1)(n+1)/4 millisesconds. The wait time is (n-1)(n+1)/4  - [1 + 3 + ... + n-2] = (n-1)/2 milliseconds. Note that this is symmetric in threads, i.e., it doesn't matter to start with thread 0 or thread 1 to execute f(1).

When n is odd the second loop run time would be exactly like the first loop, because it will assign what is assigned to thread 1 in the first loop to thread 0, in reverse order. So the run time would still be (n-1)(n+1)/4 millisesconds and wait time is (n-1)/2 milliseconds. So put together, ***when n is odd (static, 1) will give runtime (n-1)(n+1)/2 milliseconds with n-1 milliseconds wait time***.

When n is even, n-1 is odd, and runtime for first loop would be 1 + 3 + ... + n-1 = n^2/4 millisesconds. The wait time is n^2/4  - (2 + 4 + ... + n-2) = n/2 milliseconds. For the second loop, run time would be the same by symmetry. So, ***when n is even,(static, 1) will give runtime n^2/2 milliseconds with n milliseconds wait time***.

(c) The (dynamic, 1) case is more complicated than previous ones. First consider when n is odd, in the first for-loop, two threads would be assigned tasks just as in (static, 1). So first for-loop would execute for 2 + 4 + ... + n-1 = (n-1)(n+1)/4 millisesconds, in which one thread will spend (n-1)/2 milliseconds waiting. 

Then in the seconds for-loop, due to the reverse order of index, there will be a fix pattern. Let's say thread 0 gets f(n-1) and thread 1 get f(n-2) in the second loop, then thread 1 would end first and get f(n-3), then thread 0 ends and gets f(n-4). Then after 2n-5 milliseconds they will both end at the same time. This pattern continues until there are less than 4 tasks. Since n is odd, n-1 is even, there would be 2 or 0 left. If there're 2, one gets f(2) and the other gets f(1). If there's none then program ends. So we can analyze the second for-loop in terms of the remainder of n-1 divided by 4. If the remainder is 2 (n=4t+3), then second loop would execute for (4t+2+4t-1)+(4t-2+4t-5)+ ... + 2 = (8t+1)+(8t-7) + ... +9+2 = 4t^2+5*t+2 = (n+2)(n-3)/4 +2 milliseconds, in which 1 millisecond is wait time.

If the remainder is 0 (n=4t+1), then second loop would execute for (4t+4t-3)+(4t-4+4t-7)+ ... + 2 = (8t-3)+(8t-11) + ... +5+0 = 4t^2+t = n(n-1)/4 milliseconds, in which wait time = 0.

So, together with the first for-loop, the ***total execution time for n when n = 3 mod 4 is (n-1)(n+1)/4 + (n+2)(n-3)/4 +2 = (2n^2-n+1)/4 milliseconds (with (n+1)/2 millisecond waiting), when n = 1 mod 4 is (n-1)(n+1)/4 + n(n-1)/4 = (2n^2-n-1)/4 milliseconds (with (n-1)/2 millisecond waiting)***.

If n is even, then first for-loop would execute for 1 + 3 + ... + n-3 + n-1 = n^2/4 millisesconds, in which one thread will spend n/2 milliseconds waiting. In the second loop, since n is even, n-1 is odd, there would be 3 or 1 left. If the remainder is 3 (n=4t), then second loop would execute for (4t-1+4t-4)+(4t-5+4t-8)+ ... + 11+ 3 = (8t-5)+(8t-13) + ... +11+3 = 4t^2-t= (n-1)n/4 milliseconds, wait time =0. If the remainder is 1 (n=4t+2), then second loop would execute for (4t+1+4t-2)+(4t-3+4t-6)+ ... + 7+ 1 = (8t-1)+(8t-9) + ... +7+1 = 4t^2+3*t+1= (n+1)(n-2)/4+1 milliseconds, wait time =1. So, together with the first for-loop, the ***total execution time for n when n = 0 mod 4 is n^2/4+ (n-1) n/4= (2n^2-n)/4 milliseconds (with n/2 millisecond waiting), when n = 2 mod 4 is n^2/4+ (n+1)(n-2)/4+1 =  (2n^2-n+2)/4 milliseconds(with n/2+1 millisecond waiting)***.

(d) ***Yes, the directive is nowait***.

When ***n is odd***, then two threads are assigned chunks of an equal number of function calls and together they should run without wait for a exactly same amount of time. ***The time would be 1 + ... + n-1 = n(n-1)/2 milliseconds, wait time = 0***.

When ***n is even***, then the schedule(static) clause would assign a chunk with one more function call for thread 0. So thread 1 would wait for thread 0 for a little time after it finishes all tasks. ***Total execution time would be 1+ ... +n/2+n/2 + ... +n-1 = n^2/2 milliseconds. Thread 1 will wait for n seconds***within those n^2/2 milliseconds.

**Problem 2**

I parallelized the serial code and ran it on AMD 64-bit CPU, Intel® Core™ i5-10210U CPU, with 4 compute cores. The OS is Windows Subsystem for Linux distro Ubuntu 22.04. The runtime with different thread number are following:

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


**Problem 3**

Jacobi: Run time for different N and number of threads used (in seconds):

| N\num of threads | 2        | 4        | 8        | 12       | 16       | 32       |
| ---------------- | -------- | -------- | -------- | -------- | -------- | -------- |
| 10               | 0.000911 | 0.001169 | 0.001628 | 0.259923 | 0.345807 | 0.704130 |
| 20               | 0.001295 | 0.001224 | 0.001860 | 0.262076 | 0.348330 | 0.688536 |
| 40               | 0.001781 | 0.001846 | 0.002243 | 0.262964 | 0.368311 | 0.694200 |
| 80               | 0.003787 | 0.003333 | 0.003277 | 0.262515 | 0.347103 | 0.689056 |
| 100              | 0.005197 | 0.004778 | 0.004211 | 0.262674 | 0.349931 | 0.688023 |
| 200              | 0.015535 | 0.014766 | 0.009952 | 0.267112 | 0.354343 | 0.696541 |
| 400              | 0.082922 | 0.069810 | 0.065902 | 0.314272 | 0.418587 | 0.722531 |
| 800              | 0.834939 | 0.847266 | 1.138915 | 1.262281 | 1.276995 | 1.401142 |
| 1600             | 3.803361 | 3.710784 | 4.250760 | 4.463524 | 4.562097 | 4.915007 |

Gauss-Seidel: Run time for different N and number of threads used (in seconds):


| N\num of threads | 2         | 4        | 8         | 12        | 16        | 32        |
| ---------------- | --------- | -------- | --------- | --------- | --------- | --------- |
| 10               | 0.001277  | 0.001780 | 0.002494  | 0.518931  | 0.692328  | 1.411206  |
| 20               | 0.001634  | 0.001956 | 0.002929  | 0.520263  | 0.693911  | 1.389902  |
| 40               | 0.002722  | 0.002719 | 0.003464  | 0.518570  | 0.692076  | 1.412739  |
| 80               | 0.008562  | 0.005182 | 0.004672  | 0.520848  | 0.701445  | 1.388122  |
| 100              | 0.010318  | 0.007580 | 0.005765  | 0.523681  | 0.699641  | 1.391739  |
| 200              | 0.037402  | 0.029513 | 0.034397  | 0.552567  | 0.709858  | 1.401142  |
| 400              | 0.161485  | 0.121739 | 0.112694  | 0.662344  | 0.818592  | 1.498015  |
| 800              | 1.863605  | 2.112722 | 2.377001  | 2.763952  | 2.773475  | 3.123177  |
| 1600             | 11.017991 | 9.613312 | 11.348801 | 11.582299 | 13.246994 | 11.666601 |
