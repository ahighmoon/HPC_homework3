#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <cassert>  


// Scan A array and write result into prefix_sum array;
// use long data type to avoid overflow
void scan_seq(long* prefix_sum, const long* A, long n) {
    if (n == 0) return;
    prefix_sum[0] = A[0];
    //#pragma omp parallel for
    for (long i = 1; i < n; i++) {
        prefix_sum[i] = prefix_sum[i-1] + A[i];
    }
}

void scan_omp(long* prefix_sum, const long* A, long n) {
    //int p = omp_get_num_threads();
    int p = 8;
    //int t = omp_get_thread_num();
    // Fill out parallel scan: One way to do this is array into p chunks
    // Do a scan in parallel on each chunk, then share/compute the offset
    // through a shared vector and update each chunk by adding the offset
    // in parallel
    int chunksize = ceil(n / double(p));

    #pragma omp parallel for num_threads(p)
    for (int i = 0; i < p; i++){
        long head = i * chunksize;
        prefix_sum[head] = A[head];
        for (int j = head + 1; j < std::min(head + chunksize, n); j++){
            prefix_sum[j] = prefix_sum[j - 1] + A[j];
        }
    }

    for (int i = 1; i < p; i++){
        long head = i * chunksize;
        long correction = prefix_sum[head - 1];
        #pragma omp parallel for num_threads(p)
        for (int j = head; j < std::min(head + chunksize, n); j++){
            prefix_sum[j] += correction;
        }
    }
}

int main() {
  long N = 100000000;
  long* A = (long*) malloc(N * sizeof(long));
  long* B0 = (long*) malloc(N * sizeof(long));
  long* B1 = (long*) malloc(N * sizeof(long));
  for (long i = 0; i < N; i++) A[i] = rand();
  for (long i = 0; i < N; i++) B0[i] = 0;
  for (long i = 0; i < N; i++) B1[i] = 0;
  
  double tt = omp_get_wtime();
  scan_seq(B0, A, N);
  printf("sequential-scan = %fs\n", omp_get_wtime() - tt);

  // for (int p = 2; p < 32; p ++){
  //   double tt = omp_get_wtime();
  //   scan_seq(B0, A, N, p);
  //   printf("p = %d, sequential-scan = %fs\n", p, omp_get_wtime() - tt);
  // }

  tt = omp_get_wtime();
  scan_omp(B1, A, N);
  printf("parallel-scan   = %fs\n", omp_get_wtime() - tt);

  long err = 0;
  for (long i = 0; i < N; i++) {
      err = std::max(err, std::abs(B0[i] - B1[i]));
      // if (err != 0) {
      //   std::cout << "B0[" << i << "] = " << B0[i] << std::endl;
      //   std::cout << "B1[" << i << "] = " << B1[i] << std::endl;
      //   std::cout << "B0[" << i <<"] - B1[" << i << "] = " << B0[i] - B1[i] << std::endl;
      //   std::cout << "i = " << i << ", err = " << err << "\n";
      //   break;
      // }
      // assert (err == 0);
  }
  printf("error = %ld\n", err);

  free(A);
  free(B0);
  free(B1);
  return 0;
}
