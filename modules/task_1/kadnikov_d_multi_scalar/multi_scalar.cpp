// Copyright 2020 Denis Kadnikov
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_1/kadnikov_d_multi_scalar/multi_scalar.h"

std::vector<int> createRandomVector(int v_size) {
    std::vector<int> vector(v_size);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(NULL)));
    for (int i = 0; i < v_size; i++) {
        vector[i] = gen() % 100;
    }
    return vector;
}

int getSequentialMultiScalar(std::vector<int> v1, std::vector<int> v2) {
    if (v1.size() != v2.size()) {
        throw(1);
    }
    const int v_size = v1.size();
    int result = 0;
    for (int i = 0; i < v_size; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

int getParallelMultiScalar(std::vector<int> v1, std::vector<int> v2) {
    int rank, size;

    if (v1.size() != v2.size()) {
        throw(1);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int v_size = v1.size() / size;
    int resize = v1.size() % size;
    int global_result = 0;
    int local_result = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            if (proc == size - 1) {
                MPI_Send(&v1[0] + proc * v_size, v_size + resize, MPI_INT, proc, 2, MPI_COMM_WORLD);
                MPI_Send(&v2[0] + proc * v_size, v_size + resize, MPI_INT, proc, 3, MPI_COMM_WORLD);
            } else {
                MPI_Send(&v1[0] + proc * v_size, v_size, MPI_INT, proc, 0, MPI_COMM_WORLD);
                MPI_Send(&v2[0] + proc * v_size, v_size, MPI_INT, proc, 1, MPI_COMM_WORLD);
            }
        }
    }

    std::vector<int> local_v1(v_size);
    std::vector<int> local_v2(v_size);


    if (rank == 0) {
        local_v1 = std::vector<int>(v1.begin(), v1.begin() + v_size);
        local_v2 = std::vector<int>(v2.begin(), v2.begin() + v_size);
    } else if (rank == size - 1) {
        local_v1.resize(v_size + resize);
        local_v2.resize(v_size + resize);

        MPI_Status status;
        MPI_Recv(&local_v1[0], v_size + resize, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_v2[0], v_size + resize, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
    } else {
        MPI_Status status;
        MPI_Recv(&local_v1[0], v_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_v2[0], v_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    if (rank == size - 1) {
        for (int i = 0; i < v_size + resize; ++i) {
            local_result += local_v1[i] * local_v2[i];
        }
    } else {
        for (int i = 0; i < v_size; ++i) {
            local_result += local_v1[i] * local_v2[i];
        }
    }

    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_result;
}
