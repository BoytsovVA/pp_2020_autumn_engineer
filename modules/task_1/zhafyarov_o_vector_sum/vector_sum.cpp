  // Copyright 2020 Zhafyarov Oleg
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_1/zhafyarov_o_vector_sum/vector_sum.h"

std::vector <int> GetRandomVector(int size) {
    std::vector<int> vec_tmp(size);

    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < size; i++) {
        vec_tmp[i] = gen() % 50;
    }

    return vec_tmp;
}

int GetParallelSum(std::vector <int> vec, int size) {
    int process_number, process_rank;
    int buffer = 0;
    int sum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &process_number);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Status status;
    int size_for_process = size / process_number;
    int div = size % process_number;

    if (process_rank == 0) {
        if (size < process_number) {
        buffer += GetSequentialSum(vec);
        } else {
            for (int i = 1; i < process_number; i++) {
                MPI_Send(&vec[0] + i * size_for_process + div, size_for_process, MPI_INT, i, 0, MPI_COMM_WORLD);
            }

            for (int i = 0; i < div + size_for_process; i++) {
                buffer += vec[i];
            }
        }
    } else {
        if (size < process_number) {
            buffer = 0;
        } else {
            std::vector <int> vec(size_for_process);

            MPI_Recv(&vec[0], size_for_process, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            for (int i = 0; i < size_for_process; i++) {
                buffer += vec[i];
            }
        }
    }

    MPI_Reduce(&buffer, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    vec.clear();

    return sum;
}

int GetSequentialSum(std::vector<int> vec) {
    int sum = 0;
    const int size = vec.size();
    for (int i = 0; i < size; i++) {
        sum += vec[i];
    }

    return sum;
}
