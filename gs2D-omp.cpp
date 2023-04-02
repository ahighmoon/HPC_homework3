#if defined(_OPENMP)
#include <omp.h>
#else
typedef int omp_int_t;
inline omp_int_t omp_get_thread_num() { return 0;}
inline omp_int_t omp_get_num_threads() { return 1;}
#endif

#include <stdio.h>
#include <cmath>
#include "utils.h"

double GS_iterate(double *pre, long N, long i, long j, double *f)
{
    long index = i*(N+2)+j;
    return 0.25 * (f[index]/pow(N+1,2) + pre[index-N-2] + pre[index-1] + pre[index+N+2] + pre[index+1]);
}

int main(int argc, char **argv){
    Timer t;
    t.tic();
    long N = read_option<long>("-n", argc, argv);
    double *u1 = (double *)malloc(pow(N + 2, 2) * sizeof(double));
    double *u2 = (double *)malloc(pow(N + 2, 2) * sizeof(double));
    double *f = (double *)malloc(pow(N + 2, 2) * sizeof(double));
    for (long i = 0; i < pow(N + 2, 2); i++){
    u1[i] = 0; f[i] = 1;
    }

    for (long i = 0; i < N + 2; i++){
        u2[i] = 0; // [0][~]
        u2[i * (N + 2)] = 0; // [~][0]
        u2[(N + 1) * (N + 2) + i] = 0; // [n+1][~]
        u2[i * (N + 2) + (N + 1)] = 0; // [~][n+1]
    }

    int num_iter = 1000;
    for (long c = 0; c < num_iter; c++){
        #pragma omp parallel
        {
            #pragma omp for
            // for red nodes, i and j have the same parity
            for (long i = 1; i < N + 1; i++){
                for (long j = 2 - i % 2; j < N + 1; j += 2){
                    u2[i * (N + 2) + j] = GS_iterate(u1, N, i, j, f);
                }
            }

            #pragma omp for
            //for black nodes, i and j have diff parity
            for (long i = 1; i < N + 1; i++){
                for (long j = 1 + i % 2; j < N + 1; j += 2){
                    u2[i * (N + 2) + j] = GS_iterate(u2, N, i, j, f);
                }
            }

            #pragma omp for
            for (long i = 1; i < N+1; i++){
                for (long j = 1; j < N + 1; j++){
                    u1[i * (N + 2) + j] = u2[i * (N + 2) + j];
                }
            }
        }
    }
    printf("The total run time is %f. \n", t.toc());
    // for (long i = 0; i < N + 2; i++){
    //     for (long j = 0; j < N + 2; j++){
    //         printf("%f ", u1[(i) * (N + 2) + j]);
    //     }
    //     printf("\n");
    // }
    free(u1);
    free(u2);
    free(f);
    return 0;
}