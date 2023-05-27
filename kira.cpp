
#include "includs.hpp"
#include "Tasks.hpp"

static void FromITOJ(int i, int j, int rank, int n, int value) {

    value = (i + 3) * n + (j + 2) * n;

    if (i < n && j < n) {
        if (i < j) {

            if (rank == i) {
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            }

            if (rank == j) {
                MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << "get msg " << value << " process rank = " << rank << " from " << i << "\n";
            }

            if (rank > i && rank < j) {
                MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

            }

        } else {

            if (rank == i) {
                MPI_Send(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

            }

            if (rank == j) {
                MPI_Recv(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << "get msg " << value << " process rank = " << rank << " from " << i << "\n";

            }
            if (rank > j && rank < i) {
                MPI_Recv(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
            }
        }
    }
}

int kira(int argc, char* argv[]) {
    int rank, value = 333, n;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    int i = 1;

    for (int j = 0; j < n; j++) {
        if (j != i) {
            FromITOJ(i, j, rank, n, value);
        }
    }

    MPI_Finalize();

    return 0;
}