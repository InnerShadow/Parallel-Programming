
#include "includs.hpp"
#include "Tasks.hpp"

static int getSum(int argc) {
    int sum = 0;
    for (size_t i = 0; i < argc; ++i) {
        sum += std::pow(2, i);
    }
    return sum;
}

int Task_1(int argc, char** argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Tree tree(size - 1);

    //tree.bfs();

    tree.RootToAll(rank);

    if (rank == 0) {
        tree.print();
    }

    MPI_Finalize();
    
	return 0;
}
