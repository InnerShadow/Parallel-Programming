
#include "Tasks.hpp"
#include "includs.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
static unsigned long long int t1;

static void Get_next_2n_power(int size, int n, int rank, unsigned long long int* A, int q, unsigned long long int* B) {
    unsigned long long int* buffer = new unsigned long long int[n * n / size]{};
    unsigned long long int* Bbuffer = new unsigned long long int[n * n / size]{};

    int* displs = new int[size];
    int* recvcounts = new int[size];
    for (size_t i = 0; i < size; ++i) {
        displs[i] = i * n / size * n;
        recvcounts[i] = n * (n / size);
    }

    size_t from = displs[rank] / n;

    for (size_t i = from; i < from + n / size; ++i) {
        for (size_t j = 0; j < n; ++j) {
            unsigned long long int sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * A[k * n + j];
            }
            buffer[(i - from) * n + j] = sum;
        }
    }

    MPI_Gatherv(buffer, recvcounts[rank], MPI_UNSIGNED_LONG_LONG, A, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q,
        MPI_COMM_WORLD);
    MPI_Bcast(A, n * n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);

    for (size_t i = from; i < from + n / size; ++i) {
        for (size_t j = 0; j < n; ++j) {
            Bbuffer[(i - from) * n + j] += A[(from) * n + j] + B[(from) * n + j];
        }
    }

    MPI_Gatherv(Bbuffer, recvcounts[rank], MPI_UNSIGNED_LONG_LONG, B, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, q,
        MPI_COMM_WORLD);
    MPI_Bcast(B, n * n, MPI_UNSIGNED_LONG_LONG, q, MPI_COMM_WORLD);
    
    delete[] recvcounts;
    delete[] displs;
}

int Task_3(int argc, char** argv) {

    int q = 0;
    int n = 1024 ;

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned long long int* A = new unsigned long long int[n * n]{};
    unsigned long long int* B = new unsigned long long int[n * n]{};

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            A[i * n + j] = 1;
            B[i * n + j] = A[i * n + j];
        }
    }

    if (rank == q) {
        if (n == 8 ) {
            std::cout << "A:\n";
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    printf("%20llu", A[i * n + j]);
                }
                std::cout << "\n";
            }
        }
        t1 = MPI_Wtime();
    }

    for (size_t i = 0; i < 3; ++i) {
        Get_next_2n_power(size, n, rank, A, q, B);
    }

    if (rank == q) {
        if (n == 8 ) {
            std::cout << "B is: \n";
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    printf("%20llu", B[i * n + j]);
                }
                std::cout << "\n";
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        //std::cout << duration.count() << "\n";
        std::ofstream file("M" + std::to_string(n) + "P" + std::to_string(size) + ".txt", std::ios::app);

        unsigned long long int t2 = MPI_Wtime();
        std::cout << "time " << t2 - t1 << std::endl;

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
