// g++ -std=c++11 -fopenmp gs2D-omp.cpp -o gs2D-omp
// OMP_NUM_THREADS=1 ./gs2D-omp -n 10

#if defined(_OPENMP)
#include <omp.h>
#else
typedef int omp_int_t;
inline omp_int_t omp_get_thread_num() { return 0; }
inline omp_int_t omp_get_num_threads() { return 1; }
inline omp_int_t omp_get_max_threads() { return 1; }
#endif

#include <stdio.h>
#include <cmath>
#include <stdexcept>
#include "utils.h"
#include <iostream>
#include <fstream>
using namespace std;

// throughout this program, we represent a N+2 by N+2 matrix as a vector such that the (i,j)-th entry of the matrix is (i* (N+2) + j)-th entry of the vector

double GS_iterate(double *pre, long N, long i, long j, double *f)
{
    long index = i*(N+2)+j;
    return 1/4 * (f[index]/pow(N+1,2) + pre[index-N-2] + pre[index-1] + pre[index+N+2] + pre[index+1]);
}

int main(int argc, char **argv){
    Timer t;
    t.tic();
    long N = read_option<long>("-n", argc, argv);
    double *u = (double *)malloc(pow(N + 2, 2) * sizeof(double));
    double *f = (double *)malloc(pow(N + 2, 2) * sizeof(double));
    for (long i = 0; i < pow(N + 2, 2); i++){
        u[i] = 0;
        f[i] = 1;
    }

    int num_iter = 10;
    for (long c = 0; c < num_iter; c++){
        double *new_u = (double *)malloc(pow(N + 2, 2) * sizeof(double));
        for (long i = 0; i < N + 2; i++){
            new_u[i] = 0; // [0][~]
            new_u[i * (N + 2)] = 0; // [~][0]
            new_u[(N + 1) * (N + 2) + i] = 0; // [n+1][~]
            new_u[i * (N + 2) + (N + 1)] = 0; // [~][n+1]
        }

        // update red nodes
        #pragma omp parallel for
        for (long i = 1; i < N + 1; i++){
            for (long j = 2 - i % 2; j < N + 1; j += 2){
                new_u[i * (N + 2) + j] = GS_iterate(u, N, i, j, f);
            }
        }

        // update black nodes
        #pragma omp parallel for
        for (long i = 1; i < N + 1; i++){
            for (long j = 1 + i % 2; j < N + 1; j += 2){
                new_u[i * (N + 2) + j] = GS_iterate(new_u, N, i, j, f);
            }
        }
        free(u);
        u = new_u;
    }

    printf("The total run time is %f. \n", t.toc());
    ofstream myfile;
    string filename = "u_computed_with_" + to_string(omp_get_max_threads()) + "_threads.txt";
    myfile.open(filename);
    for (long i = 0; i < N + 2; i++){
        for (long j = 0; j < N + 2; j++){
            myfile << to_string(u[(i) * (N + 2) + j]) << " ";
        }
        myfile << "\n";
    }
    myfile.close();
    free(u);
    free(f);
    return 0;
}