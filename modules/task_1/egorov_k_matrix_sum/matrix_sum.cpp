// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <iostream>
#include <numeric>
#include <algorithm>
#include "../../../modules/task_1/egorov_k_matrix_sum/matrix_sum.h"

std::vector<int> getRandomVector(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getSequentialSum(std::vector<int> vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

int getParallelSum(std::vector<int> mat,
                          int mat_size) {
    int size, rank, local_sum;

    int *Data;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Data = mat.data();
    MPI_Bcast(Data, mat_size, MPI_INT, 0, MPI_COMM_WORLD);

    const int delta = mat_size / size;
    const int rmPart = mat_size % size;

    int global_sum = 0;
    // std::cout << "Calculating partial sums in rank " << rank << std::endl;
    if (rank == 0) {
        local_sum = std::accumulate(mat.begin(), (mat.begin() + delta + rmPart), 0);
    } else {
        local_sum = std::accumulate(mat.begin() + rank * delta + rmPart,
           (mat.begin() + (rank + 1) * delta + rmPart), 0);
    }
    // std::cout << "In rank " << rank << " local sum is " << local_sum << std::endl;

    // std::cout << "Executing MPI_Reduce: " << std::endl;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // std::cout << "Rank " << rank << " result: " << global_sum << std::endl;
    return global_sum;
}

