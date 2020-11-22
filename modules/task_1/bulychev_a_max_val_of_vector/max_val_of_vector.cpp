// Copyright 2020 Bulychev Andrey
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/test_tasks/test_mpi/ops_mpi.h"


std::vector<int> GetRndVector(int vec_size) {
    if (vec_size <= 0) { throw "Vector size is incorrect"; }
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(vec_size);
    for (int i = 0; i < vec_size; i++) vec[i] = gen() % 100;
    return vec;
}

int getSequentialMAX(std::vector<int> vec) {
    const int  size = vec.size();
    int tmp_max = vec[0];
    for (int i = 1; i < size; i++)
        tmp_max = std::max(tmp_max, vec[i]);
    return tmp_max;
}

int getParallelMAX(std::vector<int> vec, int count_size_vector) {
    int Size, Rank;
    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    const int count = count_size_vector / Size;
    if (Rank == 0) {
        for (int i = 1; i < Size; i++)
            MPI_Send(&vec[0] + i * count, count, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    std::vector<int> local_vec(count);
    if (Rank == 0) {
        local_vec = std::vector<int>(vec.begin(), vec.begin() + count);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int max_value = 0;
    int local_max_value = getSequentialMAX(local_vec);
    MPI_Op op = MPI_MAX;
    MPI_Reduce(&local_max_value, &max_value, 1, MPI_INT, op, 0, MPI_COMM_WORLD);
    return max_value;
}
