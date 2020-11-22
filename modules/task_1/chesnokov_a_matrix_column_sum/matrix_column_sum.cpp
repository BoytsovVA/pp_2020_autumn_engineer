// Copyright 2020 Chesnokov Artyom
#include "../../../modules/task_1/chesnokov_a_matrix_column_sum/matrix_column_sum.h"
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>

#include <iostream>

#define MPI_CALL(x)\
if (x != MPI_SUCCESS) { \
    std::cout << #x << __FILE__ << __LINE__ << std::endl; \
} \

std::vector<std::vector<int>> getRandomMatrix(int columns, int rows) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<std::vector<int>> matrix(columns);
    for (int i = 0; i < columns; i++) {
        matrix[i].resize(rows);
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = gen() % 128;
        }
    }
    return matrix;
}

std::vector<int> getSequentialColumnSum(const Matrix & matrix) {
    std::vector<int> v(matrix.size());
    for (size_t i = 0; i < v.size(); i++) {
        v[i] = 0;
    }
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[0].size(); j++) {
            v[i] += matrix[i][j];
        }
    }
    return v;
}

std::vector<int> getParallelColumnSum(const Matrix & matrix) {
    int size, rank;
    size_t columns = matrix.size();
    size_t rows = matrix[0].size();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (static_cast<int>(columns) < size) {
        return getSequentialColumnSum(matrix);
    }

    // allocating memory for each process
    // last one gets more then others if columns % size != 0
    int delta = columns / size;
    Matrix local_submatrix;
    if (rank == size - 1) {
      local_submatrix.resize(delta + columns % size);
    } else {
      local_submatrix.resize(delta);
    }

    // distribution of columns by process
    if (rank == 0) {
      std::copy(matrix.begin(), matrix.begin() + delta,
                local_submatrix.begin());

      int current_column = delta;
      for (int proc = 1; proc < size - 1; proc++) {
        for (int i = 0; i < delta; current_column++, i++) {
          MPI_CALL(MPI_Send(&matrix[current_column][0], rows, MPI_INT, proc, 0,
                   MPI_COMM_WORLD));
        }
      }
      if (size > 1) {
        for (int i = 0; i <  static_cast<int>(delta + columns % size); current_column++, i++) {
          MPI_CALL(MPI_Send(&matrix[current_column][0], rows, MPI_INT, size - 1, 0,
                   MPI_COMM_WORLD));
        }
      }
    } else {
      if (rank == size - 1) delta = delta + columns % size;
      MPI_Status status;
      for (int i = 0; i < delta; i++) {
        local_submatrix[i] = std::vector<int>(rows);
        MPI_CALL(MPI_Recv(&local_submatrix[i][0], rows, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status));
      }
    }

    // sequently counting column sum
    std::vector<int> sub_vec = getSequentialColumnSum(local_submatrix);

    std::vector<int> res(columns);
    // gathering sub-vectors
    if (rank == 0) {
      std::copy(sub_vec.begin(), sub_vec.end(), res.begin());
      if (size <= 1) return res;
      MPI_Status status;
      for (int i = 1; i < size - 1; i++) {
        MPI_CALL(MPI_Recv(&res[0] + i * delta, delta, MPI_INT, i, 0,
                          MPI_COMM_WORLD, &status));
      }
      MPI_CALL(MPI_Recv(&res[0] + (size - 1) * delta, delta + columns % size,
                        MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status));
    } else {
      MPI_CALL(
          MPI_Send(&sub_vec[0], sub_vec.size(), MPI_INT, 0, 0, MPI_COMM_WORLD));
    }

    return res;
}
