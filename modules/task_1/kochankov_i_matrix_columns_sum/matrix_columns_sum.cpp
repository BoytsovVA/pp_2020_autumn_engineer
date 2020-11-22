// Copyright 2020 Kochankov Ilya
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include "../../../modules/task_1/kochankov_i_matrix_columns_sum/matrix_columns_sum.h"


vector<int> sequential_operations(const Matrix& matrix) {
    vector<int> result(matrix.get_columns());
    std::fill(result.begin(), result.end(), 0);

    for (int i = 0; i < matrix.get_rows(); i++) {
        for (int j = 0; j < matrix.get_columns(); j++) {
            result[j] += matrix[i][j];
        }
    }

    return result;
}

vector<int> parallel_sum(const Matrix& matrix) {
    int size, rank, len = matrix.get_rows();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<int> columns_sums(len);

    if (len < size) {
        if (rank == 0) {
            return sequential_operations(matrix);
        } else {
            return vector<int>();
        }
    }

    int delta = len / size;
    int start_index;
    Matrix local_matrix;

    if (rank == 0) {
        local_matrix = Matrix(delta, matrix.get_columns());
        std::copy(matrix[0], matrix[0]+delta * matrix.get_columns(),  local_matrix[0]);

        int count = delta;
        for (int process_num = 1; process_num < size; process_num++) {
            start_index = delta * process_num;
            if (process_num == size - 1) {
                count = len - start_index;
            }
            MPI_Send(matrix[start_index], matrix.get_columns()*count, MPI_INT, process_num, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        if (rank == size - 1) {
            delta = len - delta * (size - 1);
        }

        local_matrix = Matrix(delta, matrix.get_columns());
        MPI_Recv(local_matrix[0], matrix.get_columns() * delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    auto local_result = sequential_operations(local_matrix);

    vector<int> global_result(matrix.get_columns());
    MPI_Reduce(&local_result[0], &global_result[0],
            global_result.size(), MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_result;
}

Matrix get_rand_matrix(int x, int y) {
    std::mt19937 gen;

    gen.seed(static_cast<unsigned int>(time(0)));
    Matrix vec(x, y);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            vec[i][j] = (static_cast<double >(gen()) / 100000);
        }
    }
    return vec;
}
