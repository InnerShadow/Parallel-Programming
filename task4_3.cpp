
#include "includs.hpp"
#include "Tasks.hpp"

static void ParalelForMAtrixMultiplay(int size, int n, int q, unsigned long long int* A) {
    unsigned long long int* tmpA = new unsigned long long int[n * n]{ 0 };

    #pragma omp parallel shared(A, tmpA)
    {

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                unsigned long long int sum = 0;
                for (int k = 0; k < n; ++k) {
                    sum += A[i * n + k] * A[k * n + j];
                }
                tmpA[i * n + j] = sum;
            }
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = tmpA[i * n + j];
            }
        }

    }

    std::cout << "A^2:\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << A[i * n + j] << "   ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";

    delete[] tmpA;
}

static void Matrixmultiplay(int size, int n, int q, unsigned long long int* A) {

    unsigned long long int* tmpA = new unsigned long long int[n * n]{ 0 };

    #pragma omp parallel shared(A, tmpA)
    {
        int rank = omp_get_thread_num();
        int from = rank * n / size;

        for (size_t i = from; i < from + n / size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                unsigned long long int sum = 0;
                for (size_t k = 0; k < n; ++k) {
                    sum += A[i * n + k] * A[k * n + j];
                }
                tmpA[i * n + j] = sum;
            }
        }

        #pragma omp barrier

        for (size_t i = from; i < from + n / size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                A[i * n + j] = tmpA[i * n + j];
            }
        }

    }

    std::cout << "A^2:\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << A[i * n + j] << "   ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";

    delete[] tmpA;
}

static void ParalellForConstantMultiplay(int size, int n, int q, unsigned long long int* A, unsigned long long int* B, unsigned long long int C) {
    unsigned long long int* tmpA = new unsigned long long int[n * n]{ 0 };

    #pragma omp parallel shared(A, B, tmpA)
    {

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                tmpA[i * n + j] = A[i * n + j] * C;
            }
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                B[i * n + j] = tmpA[i * n + j];
            }
        }

    }

    delete[] tmpA;
}

static void ConstantMultiplay(int size, int n, int q, unsigned long long int* A, unsigned long long int* B, unsigned long long int C) {

    unsigned long long int* tmpA = new unsigned long long int[n * n]{ 0 };

    #pragma omp parallel shared(A, B, tmpA)
    {
        int rank = omp_get_thread_num();
        int from = rank * n / size;

        for (size_t i = from; i < from + n / size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                tmpA[i * n + j] = A[i * n + j] * C;
            }
        }
        
        #pragma omp barrier

        for (size_t i = from; i < from + n / size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                B[i * n + j] = tmpA[i * n + j];
            }
        }

    }

    delete[] tmpA;
}

int Task4_3(int argc, char** argv) {
    int q = 0;
    int n = 8;

    int size = 0;
    std::cout << "Enter num of precesses: ";
    std::cin >> size;

    omp_set_num_threads(size);

    unsigned long long int* A = new unsigned long long int[n * n]{};
    unsigned long long int* B = new unsigned long long int[n * n]{};

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            A[i * n + j] = 1;
        }
    }

    unsigned long long int C = 0;
    for (size_t i = 0; i < n; ++i) {
        unsigned long long int mult = 1;
        for (size_t j = 0; j < n; ++j) {
            mult *= A[i * n + j];
        }
        C += mult;
    }

    double start_time = omp_get_wtime();

    //Matrixmultiplay(size, n, q, A);
    //ConstantMultiplay(size, n, q, A, B, C);

    ParalelForMAtrixMultiplay(size, n, q, A);
    ParalellForConstantMultiplay(size, n, q, A, B, C);

    double end_time = omp_get_wtime();

    if (n == 8) {
        std::cout << "B:\n";
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                printf("%20llu", B[i * n + j]);
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n\n" << "Time, that programm works is: " << end_time - start_time << "\nnumber of prosesses: " << size << "\nmatrix size is: " << n << " * " << n << "\n";


    delete[] A;
    delete[] B;

    return 0;
}