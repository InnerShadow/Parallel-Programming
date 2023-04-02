
#include "includs.hpp"
#include "Tasks.hpp"

int Test_task(int argc, char** argv) {
    int rank, size;
    int message = 42;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "Received message: " << message << std::endl;
    }

    MPI_Finalize();
    return 0;
}
