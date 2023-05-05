
#include "includs.hpp"
#include "Tasks.hpp"

static int Collectors(int argc, char** argv) {

    int q = 0;
    int k = 0;

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int m = 5;
    int n = 5;

    int* A = new int[n * m];

    for (int i = 0; i < n * m; ++i) {
        A[i] = rank;
    }

    int* B = nullptr;

    if (rank == q) {
        B = new int[m * size];
    }

    MPI_Gather(A + k * n, m, MPI_INT, B + k * m * size, m, MPI_INT, q, MPI_COMM_WORLD);

    if (rank == q) {

        for (int i = 0; i < m * size; ++i) {
            std::cout << B[i] << " ";
        }
        std::cout << "\n";

        delete[] B;
    }

    delete[] A;

    MPI_Finalize();

    return 0;
}

static int OldType(int argc, char** argv) {

    int q = 0;
    int k = 0;

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int m = 5;
    int n = 5;

    int* A = new int[n * m];

    for (int i = 0; i < n * m; ++i) {
        A[i] = rank;
    }

    int* B = nullptr;

    if (rank == q) {
        B = new int[m * size];
    }

    if (rank != q) {
        MPI_Send(A, m, MPI_INT, q, 0, MPI_COMM_WORLD);
    }

    if (rank == q) {

        for (int i = 0; i < m; ++i) {
            B[q * m + i] = q;
        }

        for (int i = 0; i < size; ++i) {
            if (i != q) {
                MPI_Recv(B + i * m, m, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    if (rank == q) {

        for (int i = 0; i < m * size; ++i) {
            std::cout << B[i] << " ";
        }
        std::cout << "\n";

        delete[] B;
    }

    delete[] A;

    MPI_Finalize();

    return 0;
}


int Task_2_1(int argc, char** argv) {

    //Collectors(argc, argv);

    OldType(argc, argv);

	return 0;
}
