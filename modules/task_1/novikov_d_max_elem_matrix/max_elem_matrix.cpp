// Copyright 2020 Novikov Danil
#include <mpi.h>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include <algorithm>
#include "../../../modules/test_tasks/test_mpi/ops_mpi.h"
#include "../../../modules/task_1/novikov_d_max_elem_matrix/max_elem_matrix.h"

int* RandomMatrixValues(int* mat, int rows, int columns) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            mat[i * columns + j] = gen() % 10000;
        }
    }

    return mat;
}

int ConsistentOperations(int* mat, int rows, int columns) {
    int maxElem = INT_MIN;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (mat[i * columns + j] > maxElem) {
                maxElem = mat[i * columns + j];
            }
        }
    }

    return maxElem;
}

int ParallelOperations(int* mat, int rows, int columns) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = rows / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&mat[proc * delta * columns], delta * columns,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    int* local_mat = new int[delta * columns];

    if (rank == 0) {
        local_mat = mat;
    } else {
        MPI_Status status;
        MPI_Recv(local_mat, delta * columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int global_max = 0;
    int local_max = ConsistentOperations(local_mat, delta, columns);

    if (rank == 0 && rows > size * delta) {
        int local_max_ost;
        local_mat = &mat[size * delta * columns];
        local_max_ost = ConsistentOperations(local_mat, rows - (size * delta), columns);
        local_max = (local_max > local_max_ost) ? local_max : local_max_ost;
    }

    MPI_Op op_code = MPI_MAX;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return global_max;
}

