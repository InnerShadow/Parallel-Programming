
#include "includs.hpp"
#include "Tasks.hpp"

int Task2_3(int argc, char** argv) {

    int rank = 0;
    int size = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 2;
    int m = 4;
    int k = 1;

    int root = 0;

    std::unique_ptr<int[]> A = std::make_unique<int[]>(n * m);
    for (size_t i = 0; i < n; ++i){
        for (size_t j = 0; j < m; ++j) {
            A[i * m + j] = rank;
        }
    }

    if (rank == root) {
        std::unique_ptr<int[]> B = std::make_unique<int[]>(m * size);
        for (size_t i = 0; i < m * size; ++i) {
            B[i] = i;
        }

        for (size_t i = 0; i < size; ++i) {
            int* C = B.get() + m * i;
            if(i != root){
                MPI_Send(C, m, MPI_INT, i, 0, MPI_COMM_WORLD);
            } else {
                for (size_t j = 0; j < m; ++j) {
                    A[k * m + j] = C[j];
                }
            }
        }
    } else {
        std::unique_ptr<int[]> Recive = std::make_unique<int[]>(m);
        MPI_Recv(Recive.get(), m, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (size_t i = 0; i < m; ++i) {
            A[k * m + i] = Recive[i];
        }
    }

    std::cout << "Rank == " << rank << "\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            printf("%-5d", A[i * m + j]);
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    return MPI_Finalize();
}

