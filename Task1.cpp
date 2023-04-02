
#include "includs.hpp"
#include "Tasks.hpp"

int getSum(int argc) {
    int sum = 0;
    for (size_t i = 0; i < argc; i++) {
        sum += std::pow(2, i);
    }
    return sum;
}

int Task_1(int argc, char** argv) {

    argc = 4;

    Tree tree(getSum(argc) - 1);

    //tree.bfs();

    tree.RootToAll(argc, argv);
    
	return 0;
}
