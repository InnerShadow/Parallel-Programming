
#include "includs.hpp"
#include "Tasks.hpp"

static int SCATTER(int argc, char** argv) {
	int rank, size;
	int n = 10;
	int m = 10;

	int k = 0;

	int root = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* A = new int[n * m];
	for (int i = 0; i < n * m; ++i) {
		A[i] = rank;
	}

	int* B = nullptr;
	if (rank == root)
	{
		B = new int[n * size];
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < size; ++j) {
				B[i * size + j] = j;
			}
		}

	}

	MPI_Scatter(B, size, MPI_INT, A, size, MPI_INT, root, MPI_COMM_WORLD);

	int* AT = new int[n * m]{ 0 };
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			AT[i * n + j] = A[j * n + i];
		}
	}

	std::cout << "Process rank = " << rank << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			std::cout << AT[i * n + j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	delete[] A;
	delete[] AT;

	if (rank == root) {
		delete[] B;
	}

	MPI_Finalize();
	return 0;
}

static int OLD(int argc, char** argv) {
	int rank, size;
	int n = 10;
	int m = 10;

	int k = 0;

	int root = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* A = new int[n * m];
	for (int i = 0; i < n * m; ++i) {
		A[i] = rank;
	}

	int* B = nullptr;
	if (rank == root)
	{
		B = new int[n * size];
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < size; ++j) {
				B[i * size + j] = j;
			}
		}
		
		for (int i = 0; i < size; ++i) {
			if (i != root) {
				MPI_Send(B + i * size, size, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}

		for (int i = 0; i < size; ++i) {
			A[i * n + k] = B[i];
		}

	}

	if (rank != root) {
		int* Recive = new int[size];
		MPI_Recv(Recive, size, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		for (int i = 0; i < size; ++i) {
			A[i * n + k] = Recive[i];
		}
	}

	std::cout << "Process rank = " << rank << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			std::cout << A[i * n + j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	delete[] A;

	if (rank == root) {
		delete[] B;
	}

	MPI_Finalize();
	return 0;
}

int Task_2_8(int argc, char** argv) {
	
	//return SCATTER(argc, argv);

	return OLD(argc, argv);
}