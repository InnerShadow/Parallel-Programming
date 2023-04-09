
#include "Tasks.hpp"
#include "includs.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

static void Get_next_2n_power(int size, int n, int rank, int* A, int q, int* B) {
    int* buffer = new int[n * n / size]{};
    int* Cbuffer = new int[n * n / size]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];
    for (size_t i = 0; i < size; ++i) {
        displs[i] = i * n / size * n;
        recvcounts[i] = n * (n / size);
    }

    size_t start_line = displs[rank] / n;

    for (size_t i = start_line; i < start_line + n / size; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * A[k * n + j];
            }
            buffer[(i - start_line) * n + j] = sum;
        }
    }

    MPI_Gatherv(buffer, recvcounts[rank], MPI_INT, A, recvcounts, displs, MPI_INT, q,
        MPI_COMM_WORLD);
    MPI_Bcast(A, n * n, MPI_INT, q, MPI_COMM_WORLD);

    for (size_t i = start_line; i < start_line + n / size; ++i) {
        for (size_t j = 0; j < n; ++j) {
            Cbuffer[(i - start_line) * n + j] += A[(i - start_line) * n + j] + B[(i - start_line) * n + j];
        }
    }

    MPI_Gatherv(Cbuffer, recvcounts[rank], MPI_INT, B, recvcounts, displs, MPI_INT, q,
        MPI_COMM_WORLD);
    MPI_Bcast(B, n * n, MPI_INT, q, MPI_COMM_WORLD);
    

    delete[] recvcounts;
    delete[] displs;
}

int Task_3(int argc, char** argv) {

    int q = 0;
    int n = 8 * 8 * 4 * 2 * 2;

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* A = new int[n * n]{};
    int* B = new int[n * n]{};

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            A[i * n + j] = 1;
            B[i * n + j] = A[i * n + j];
        }
    }

    if (rank == q) {
        if (false) {
            std::cout << "A:\n";
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    printf("%10d", A[i * n + j]);
                }
                std::cout << "\n";
            }
        }
        start_time = std::chrono::high_resolution_clock::now();
    }

    for (size_t i = 0; i < 3; ++i) {
        Get_next_2n_power(size, n, rank, A, q, B);
    }

    if (rank == q) {
        if (false) {
            std::cout << "B is: \n";
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    printf("%10d", B[i * n + j]);
                }
                std::cout << "\n";
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << duration.count() << "\n";
        std::ofstream file("M" + std::to_string(n) + "P" + std::to_string(size) + ".txt", std::ios::app);
        if (file.is_open()) {
            file << duration.count() << "\n";
            file.close();
        } else {
            throw std::runtime_error("Cannot Open file!");
            return 1;
        }
    }

    delete[] A;
    delete[] B;

    MPI_Finalize();

    return 0; 
}