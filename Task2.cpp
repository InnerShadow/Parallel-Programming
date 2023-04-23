
#include "includs.hpp"
#include "Tasks.hpp"
   
static int getIndex(size_t i, size_t j, int n) {
	return i * n + j;
}

static int OldTypeTusk(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 60;
    std::vector<int> B(n - rank);
    std::vector<int> A(n * size, 0);

    for (size_t i = 0; i < n - rank; ++i) {
        B.at(i) = rank;
    }

    for (size_t i = 0; i < size; ++i) {
        if (i != rank) {
            MPI_Send(B.data(), B.size(), MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
    }

    for (size_t i = 0; i < size; ++i) {
        if (i != rank) {
            std::vector<int> B_recive(n - i, 0);
            MPI_Recv(B_recive.data(), B_recive.size(), MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (size_t j = 0; j < n - i; ++j) {
                A.at(getIndex(i, j, n)) = B_recive.at(j);
            }
        }
    }

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = n - 1; j > n - rank - 1; --j) {
            A.at(getIndex(i, j, n)) = 0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "A: \n";
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                printf("%3d", A.at(getIndex(i, j, n)));
            }
            std::cout << "\n";
        }
    }

    MPI_Finalize();

    return 0;
}

static int USSROption(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 60;
    std::vector<int> B(n - rank);
    std::vector<int> A(n * size);

    for (size_t i = 0; i < n - rank; ++i) {
        B.at(i) = rank;
    }

    int* recvcounts = new int[size];
    int* displs = new int[size];
    for (size_t i = 0; i < size; ++i) {
        recvcounts[i] = n;  // recvcounts[i] = n - i;
        displs[i] = i * n;  // displs[i] = i * n + i;
    }

    MPI_Allgatherv(B.data(), n - rank, MPI_INT, A.data(), recvcounts, displs, MPI_INT, MPI_COMM_WORLD);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = n - 1; j > n - rank - 1; --j) {
            A.at(getIndex(i, j, n)) = 0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "A: \n";
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < n; ++j) {
                printf("%3d", A.at(getIndex(i, j, n)));
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

    //return USSROption(argc, argv);

    return OldTypeTusk(argc, argv);

}
