
#include "includs.hpp"
#include "Tasks.hpp"

static void FromItoJ(int rank, int i, int j, std::string msg, int argv, char** argc) {
    if (i == 0) {
        if (i == rank) {
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_CHAR, j, 0, MPI_COMM_WORLD);
        } else if (j == rank) {
            char buffer[100];
            MPI_Recv(buffer, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Received message: " << buffer << ", procese's rank = " << rank << ", get message from " << i << std::endl;
        }
    }
    else if (j == 0) {
        if (i == rank) {
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        } if (rank == 0) {
            char buffer[100];
            MPI_Recv(buffer, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Received message: " << buffer << ", procese's rank = " << rank << ", get message from " << i << std::endl;
        }
    } else if (i != 0) {
        if (i == rank) {
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        } if (rank == 0) {
            char buffer[100];
            MPI_Recv(buffer, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, 100, MPI_CHAR, j, 0, MPI_COMM_WORLD);
        } if (rank == j) {
            char buffer[100];
            MPI_Recv(buffer, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Received message: " << buffer << ", procese's rank = " << rank << ", get message from " << i << std::endl;
        }
    }
}

int Task_1_4(int argc, char** argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string message("Good morning world!");

    //from i to j
    int i = 1;
    int j = 3;
    //FromItoJ(rank, i, j, message, argc, argv);

    //From i to all
    for (int j = 0; j < size; j++) {
        if (i != j) {
            FromItoJ(rank, i, j, message, argc, argv);
        }
    }


    MPI_Finalize();

    return 0;
}