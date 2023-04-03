
#include "includs.hpp"
#include "Tasks.hpp"

static int getIndex(size_t i, size_t j, int n) {
	return i * n + j;
}

static int USSROption(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 60;
    std::vector<int> B(n - rank);
    std::vector<int> recv_buf(n * size);

    for (size_t i = 0; i <= rank; ++i) {
        B.at(i) = rank;
    }

    int* recvcounts = new int[size];
    int* displs = new int[size];
    for (size_t i = 0; i < size; ++i) {
        recvcounts[i] = n;
        displs[i] = i * n;
    }

    MPI_Allgatherv(B.data(), n, MPI_INT, recv_buf.data(), recvcounts, displs, MPI_INT, MPI_COMM_WORLD);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            recv_buf.at(getIndex(i, j, n)) = 0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "A: \n";
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                //std::cout << recv_buf.at(getIndex(i, j, n)) << " ";
                printf("%3d", recv_buf.at(getIndex(i, j, n)));
            }
            std::cout << "\n";
        }

        // std::cout << "\n";

        // for (size_t i = 0; i < size; ++i) {
        //     std::cout << recvcounts[i] << "  ";
        // }
        // std::cout << "\n";
        // for (size_t i = 0; i < size; ++i) {
        //     std::cout << displs[i] << "  ";
        // }
        // std::cout << "\n";

    }

    delete[] recvcounts;
    delete[] displs;

    MPI_Finalize();

    return 0;
}

int Task_2(int argc, char** argv) {

    return USSROption(argc, argv);

}