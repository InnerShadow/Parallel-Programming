
#include "includs.hpp"
#include "Tasks.hpp"

static void AxX(unsigned long long int* A, unsigned long long int* X, unsigned long long int* B, int rank, int size, int n, int q) {

    int amout = n / size + (rank >= size - (n % size));

    unsigned long long int* buffer = new unsigned long long int[n * amout]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];

    int div = n / size;

    for (size_t i = 0; i < size; ++i) {
        int diffrence = size - (n % size);
        displs[i] = i * div;
        displs[i] = (i > diffrence) ? displs[i] + (i - diffrence) : displs[i];
        //displs[i] *= n;
        recvcounts[i] = (n / size + (i >= diffrence));
    }

    size_t from = displs[rank] / n;

    for (size_t i = from; i < from + amout; ++i) {
        for (size_t j = 0; j < n; ++j) {
            buffer[(i - from)] += A[i * n + j] * X[j];
        }
    }

    MPI_Gatherv(buffer, recvcounts[rank], MPI_UNSIGNED_LONG_LONG, B, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(B, n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    delete[] recvcounts;
    delete[] displs;
}

static void XxA(unsigned long long int* A, unsigned long long int* X, unsigned long long int* B, int rank, int size, int n, int q) {
    int amout = n / size + (rank >= size - (n % size));

    unsigned long long int* buffer = new unsigned long long int[n * amout]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];

    int div = n / size;

    for (size_t i = 0; i < size; ++i) {
        int diffrence = size - (n % size);
        displs[i] = i * div;
        displs[i] = (i > diffrence) ? displs[i] + (i - diffrence) : displs[i];
        //displs[i] *= n;
        recvcounts[i] = (n / size + (i >= diffrence));
    }

    size_t from = displs[rank] / n;

    for (size_t i = from; i < from + amout; ++i) {
        for (size_t j = 0; j < n; ++j) {
            buffer[(i - from)] += X[i] * A[j * n + i];
        }
    }

    if (rank == q) {
        std::cout << "disps: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << displs[i] << " ";
        }
        std::cout << "\n recive: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << recvcounts[i] << " ";
        }
        std::cout << "\n";
    }

    MPI_Gatherv(buffer, recvcounts[rank], MPI_UNSIGNED_LONG_LONG, X, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q,
        MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(X, n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    delete[] recvcounts;
    delete[] displs;
}

static void sum(unsigned long long int* X, unsigned long long int* B, int rank, int size, int n, int q) {
    int amout = n / size + (rank >= size - (n % size));

    unsigned long long int* buffer = new unsigned long long int[n * amout]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];

    int div = n / size;

    for (size_t i = 0; i < size; ++i) {
        int diffrence = size - (n % size);
        displs[i] = i * div;
        displs[i] = (i > diffrence) ? displs[i] + (i - diffrence) : displs[i];
        //displs[i] *= n;
        recvcounts[i] = (n / size + (i >= diffrence));
    }

    size_t from = displs[rank] / n;

    for (size_t i = from; i < from + amout; ++i) {
        buffer[(i - from)] += X[i] * B[i];
    }

    MPI_Gatherv(buffer, recvcounts[rank], MPI_UNSIGNED_LONG_LONG, B, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q,
        MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(B, n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    delete[] recvcounts;
    delete[] displs;
}

int Task2_3(int argc, char** argv) {
    int q = 0;
    int n = 8;
    int C = 2;
    double t1 = 0;

    MPI_Init(&argc, &argv);

    int p, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    unsigned long long int* A = new unsigned long long int[n * n]{};
    unsigned long long int* X = new unsigned long long int[n] {};
    unsigned long long int* B = new unsigned long long int[n] {};

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            A[i * n + j] = 1;
        }
        X[i] = 1;
    }

    if (rank == q) {
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

        t1 = MPI_Wtime();
    }

    AxX(A, X, B, rank, p, n, q);
    XxA(A, X, B, rank, p, n, q);
    sum(X, B, rank, p, n, q);

    if (rank == q) {
        if (n == 8) {
            for (size_t i = 0; i < n; ++i) {
                std::cout << B[i] << "   ";
            }
            std::cout << "\n";
        }
    }

    if (rank == q) {
        double t2 = MPI_Wtime();
        std::cout << "\n\n" << "time on " << p << " processes, matrix is " << n << " * " << n << " is: " << t2 - t1 << "\n\n";
    }

    delete[] A;
    delete[] X;
    delete[] B;

    MPI_Finalize();

    return 0;
}