
#include "Tasks.hpp"
#include "includs.hpp"

static void ParallelForAxX(unsigned long long int* A, unsigned long long int* X, unsigned long long int* B, int size, int n, int q) {
    unsigned long long int* tmpA = new unsigned long long int[n] { 0 };

    #pragma omp parallel shared(A, tmpA, B, X)
    {

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                tmpA[i] += A[i * n + j] * X[j];
            }
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            B[i] = tmpA[i];
        }
    }

    delete[] tmpA;
}

static void AxX(unsigned long long int* A, unsigned long long int* X, unsigned long long int* B, int size, int n, int q) {

    unsigned long long int* tmpA = new unsigned long long int[n]{ 0 };

    #pragma omp parallel shared(A, tmpA, B, X)
    {
        int rank = omp_get_thread_num();
        int from = rank * n / size;

        for (size_t i = from; i < from + n / size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                tmpA[i] += A[i * n + j] * X[j];
            }
        }

        #pragma omp barrier

        for (size_t i = from; i < from + n / size; ++i) {
            B[i] = tmpA[i];
        }
    }

    delete[] tmpA;
}

static void PAralellForXxA(unsigned long long int* A, unsigned long long int* X, unsigned long long int* B, int size, int n, int q) {
    unsigned long long int* tmpA = new unsigned long long int[n] { 0 };

    #pragma omp parallel shared(A, tmpA, B, X)
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                tmpA[i] += X[i] * A[j * n + i];
            }
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            X[i] = tmpA[i];
        }
    }

    delete[] tmpA;
}

static void XxA(unsigned long long int* A, unsigned long long int* X, unsigned long long int* B, int size, int n, int q) {
    
    unsigned long long int* tmpA = new unsigned long long int[n]{ 0 };

    #pragma omp parallel shared(A, tmpA, B, X)
    {
        int rank = omp_get_thread_num();
        int from = rank * n / size;

        for (size_t i = from; i < from + n / size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                tmpA[i] += X[i] * A[j * n + i];
            }
        }

        #pragma omp barrier

        for (size_t i = from; i < from + n / size; ++i) {
            X[i] = tmpA[i];
        }
    }

    delete[] tmpA;
}

static void ParallellForSum(unsigned long long int* X, unsigned long long int* B, int size, int n, int q) {
    unsigned long long int* tmpA = new unsigned long long int[n] { 0 };

    #pragma omp parallel shared(tmpA, B, X)
    {

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            tmpA[i] += X[i] * B[i];
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            B[i] = tmpA[i];
        }
    }

    delete[] tmpA;
}

static void sum(unsigned long long int* X, unsigned long long int* B, int size, int n, int q) {

    unsigned long long int* tmpA = new unsigned long long int[n] { 0 };

    #pragma omp parallel shared(tmpA, B, X)
    {

        int rank = omp_get_thread_num();
        int from = rank * n / size;

        for (size_t i = from; i < from + n / size; ++i) {
            tmpA[i] += X[i] * B[i];
        }

        #pragma omp barrier

        for (size_t i = from; i < from + n / size; ++i) {
            B[i] = tmpA[i];
        }
    }

    delete[] tmpA;
}

int Task4_8(int argc, char** argv) {
    int q = 0;
    int n = 8;

    int size = 0;
    std::cout << "Enter num of precesses: ";
    std::cin >> size;

    omp_set_num_threads(size);

    unsigned long long int* A = new unsigned long long int[n * n]{};
    unsigned long long int* X = new unsigned long long int[n] {};
    unsigned long long int* B = new unsigned long long int[n] {};

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            A[i * n + j] = 1;
        }
        X[i] = 1;
    }

    std::cout << "A:\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            printf("%20llu", A[i * n + j]);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (size_t i = 0; i < n; ++i) {
        std::cout << X[i] << " ";
    }
    std::cout << "\n";

    double start_time = omp_get_wtime();

    //AxX(A, X, B, size, n, q);
    //XxA(A, X, B, size, n, q);
    //sum(X, B, size, n, q);

    ParallelForAxX(A, X, B, size, n, q);
    PAralellForXxA(A, X, B, size, n, q);
    ParallellForSum(X, B, size, n, q);

    double end_time = omp_get_wtime();

    if (n == 8) {
        for (size_t i = 0; i < n; ++i) {
            std::cout << B[i] << "   ";
        }
        std::cout << "\n";
    }

    std::cout << "\n\n" << "time on " << size << " processes, matrix is " << n << " * " << n << " is: " << end_time - start_time << "\n\n";

    delete[] A;
    delete[] X;
    delete[] B;

	return 0;
}