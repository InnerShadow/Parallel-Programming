
#include "includs.hpp"
#include "Tasks.hpp"
#include "omp.h"

static void Get_next_2n_power(unsigned long long int* A, unsigned long long int* B, int size, int n) {

	unsigned long long int* tmpA = new unsigned long long int[n * n]{ 0 };

	#pragma omp parallel shared(A, B, tmpA) 	
	{
		int rank = omp_get_thread_num();
		int from = rank * n / size;

		for (size_t i = from; i < from + n / size; ++i) {
			for (size_t j = 0; j < n; ++j) {
				unsigned long long int sum = 0;
				for (size_t k = 0; k < n; ++k) {
					sum += A[i * n + k] * A[k * n + j];
				}
				tmpA[i * n + j] = sum;
			}
		}

		#pragma omp barrier

		for (size_t i = from; i < from + n / size; ++i) {
			for (size_t j = 0; j < n; ++j) {
				A[i * n + j] = tmpA[i * n + j];
				B[i * n + j] += A[i * n + j];
			}
		}
	
	}
}

int Task_4(int argc, char** argv, int size) {

	int myid = 0;

	int n = 8 * 8 * 4 * 2 * 2;

	unsigned long long int* A = new unsigned long long int[n * n];
	unsigned long long int* B = new unsigned long long int[n * n];
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			A[i * n + j] = 1;
			B[i * n + j] = A[i * n + j];
		}
	}

	omp_set_num_threads(size);

	double start_time = omp_get_wtime();

	for (size_t i = 0; i < 3; ++i) {
		Get_next_2n_power(A, B, size, n);
	}

	double end_time = omp_get_wtime();

	std::ofstream file("__OMP__M" + std::to_string(n) + "P" + std::to_string(size) + ".txt", std::ios::app);
	if (file.is_open()) {
		file << end_time - start_time << "\n";
		file.close();
	}
	else {
		throw std::runtime_error("Cannot Open file!");
		return 1;
	}

	if (false) {
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				std::cout << B[i * n + j] << " ";
			}
			std::cout << "\n";
		}
	}

	return 0;
}