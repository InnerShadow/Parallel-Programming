
#include "includs.hpp"
#include "Tasks.hpp"

int Task_1(int argc, char** argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Tree tree(size - 1);

    //tree.bfs();

    tree.RootToAll(rank);

    if (!rank) {
        tree.print();
    }

    MPI_Finalize();
    
	return 0;
}
