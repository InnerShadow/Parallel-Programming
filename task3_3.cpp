
#include "includs.hpp"
#include "Tasks.hpp"

static double t1;

static void Matrixmultiplay(int rank, int size, int n, int q, unsigned long long int* A, unsigned long long int* B) {

    int amout = n / size + (rank >= size - (n % size));

    unsigned long long int* buffer = new unsigned long long int[n * amout]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];

    int div = n / size;

    for (size_t i = 0; i < size; ++i) {
        int diffrence = size - (n % size);
        displs[i] = i * div;
        displs[i] = (i > diffrence) ? displs[i] + (i - diffrence) : displs[i];
        displs[i] *= n;
        recvcounts[i] = n * (n / size + (i >= diffrence));
    }

    if (rank == q) {
        std::cout << "disp: ";
        for(size_t i = 0; i < size; ++i){
            std::cout << displs[i] << "  ";
        }
        std::cout << "\nrecvcounts: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << recvcounts[i] << "  ";
        }
        std::cout << "\n";
    }

    size_t from = displs[rank] / n;

    for (size_t i = from; i < from + amout; ++i) {
        for (size_t j = 0; j < n; ++j) {
            unsigned long long int sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            buffer[(i - from) * n + j] = sum;
        }
    }

    MPI_Gatherv(buffer, amout * n, MPI_UNSIGNED_LONG_LONG, A, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q,
        MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(A, n * n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    delete[] recvcounts;
    delete[] displs;
    delete[] buffer;
}

static void ConstantMultiplay(int rank, int size, int n, int q, unsigned long long int* A, unsigned long long int* B, unsigned long long int C) {
    int amout = n / size + (rank >= size - (n % size));

    unsigned long long int* buffer = new unsigned long long int[n * amout]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];

    int div = n / size;

    for (size_t i = 0; i < size; ++i) {
        int diffrence = size - (n % size);
        displs[i] = i * div;
        displs[i] = (i > diffrence) ? displs[i] + (i - diffrence) : displs[i];
        displs[i] *= n;
        recvcounts[i] = n * (n / size + (i >= diffrence));
    }

    size_t from = displs[rank] / n;

    for (size_t i = from; i < from + amout; ++i) {
        for (size_t j = 0; j < n; ++j) {
            buffer[(i - from) * n + j] = A[i * n + j] * C;
        }
    }

    MPI_Gatherv(buffer, amout * n, MPI_UNSIGNED_LONG_LONG, B, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);
    MPI_Bcast(B, n * n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    delete[] recvcounts;
    delete[] displs;
    delete[] buffer;
}

int task3_3(int argc, char** argv) {

    int q = 0;
    int n = 500;

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned long long int* A = new unsigned long long int[n * n]{};
    unsigned long long int* B = new unsigned long long int[n * n]{};

    if (rank == q) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                A[i * n + j] = 1;
            }
        }
        t1 = MPI_Wtime();
    }
    MPI_Bcast(A, n * n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    unsigned long long int C = 0;
    if (rank == q) {
        for (size_t i = 0; i < n; ++i) {
            unsigned long long int mult = 1;
            for (size_t j = 0; j < n; ++j) {
                mult *= A[i * n + j];
            }
            C += mult;
        }
    }

    MPI_Bcast(&C, 1, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    Matrixmultiplay(rank, size, n, q, A, A);
    ConstantMultiplay(rank, size, n, q, A, B, C);

    if (rank == q) {
        if (n == 8) {
            std::cout << "B:\n";
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    printf("%20llu", B[i * n + j]);
                }
                std::cout << "\n";
            }
        }
        double t2 = MPI_Wtime();

        std::cout << "\n\n" << "Time, that programm works is: " <<  t2 - t1 << "\nnumber of prosesses: " << size << "\nmatrix size is: " << n << " * " << n << "\n";

    }


    delete[] A;
    delete[] B;

    MPI_Finalize();
    return 0;
}