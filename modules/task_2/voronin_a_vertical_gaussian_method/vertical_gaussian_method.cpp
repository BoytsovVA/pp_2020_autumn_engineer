// Copyright 2020 Voronin Aleksey
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_2/voronin_a_vertical_gaussian_method/vertical_gaussian_method.h"

std::vector<double> getRandomMatrixLinear(const int rows) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<double> linear_matrix(rows * (rows + 1));
  for (int i = 0; i < rows * (rows + 1); i++) {
    linear_matrix[i] = (gen() % 5) + 1;
  }
  return linear_matrix;
}

std::vector<double> calculateResults(std::vector<double> matrix, int rows, std::vector<double> values) {
  std::vector<double> results;
  double sum;
  for (int i = 0; i < rows; i++) {
      sum = 0;
      for (int j = 0; j < rows; j++) {
          sum += matrix[((rows+1) * i) + j] * values[j];
      }
      results.push_back(sum);
  }
  return results;
}

std::vector<double> sequentialGaussianMethod(std::vector<double> matrix, int rows) {
  if (matrix.size() != (unsigned int)((rows + 1) * rows) || rows <= 0) {
    std::vector<double> empty(0);
    return empty;
  }

  int i, j, k;
  double tmp;
  std::vector<double> results = std::vector<double>(rows);

  for (i = 0; i < rows; i++) {
    tmp = matrix[i * (rows + 1) + i];
    for (j = rows; j >= i; j--)
      matrix[i * (rows + 1) + j] /= tmp;
    for (j = i + 1; j < rows; j++) {
      tmp = matrix[j * (rows + 1) + i];
      for (k = rows; k >= i; k--)
        matrix[j * (rows + 1) + k] -= tmp * matrix[i * (rows + 1) + k];
    }
  }

  results[rows - 1] = matrix[(rows - 1) * (rows + 1) + rows];
  for (i = rows - 2; i >= 0; i--) {
    results[i] = matrix[i * (rows + 1) + rows];
    for (j = i + 1; j < rows; j++)
      results[i] -= matrix[i * (rows + 1) + j] * results[j];
  }

  return results;
}

std::vector <double> parallelGaussianMethod(std::vector <double> matrix, size_t rows) {
    if (matrix.size() != (unsigned int)((rows + 1) * rows) || rows <= 0) {
        std::vector<double> empty(0);
        return empty;
    }

    int size, rank;
    const size_t cols = rows + 1;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = cols / size;
    const int remaining = cols % size;

    std::vector <double> vec((delta + (rank < remaining ? 1 : 0)) * rows);

    // Creation and distribution of columns
    if (rank == 0) {
        for (int proc = size - 1; proc >= 0; --proc) {
            int index = 0;
            for (size_t j = proc; j < cols; j += size) {
                for (size_t i = 0; i < rows; ++i) {
                    vec[index++] = matrix[i * cols + j];
                }
            }
            if (proc > 0) {
                MPI_Send(vec.data(), index, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Status stat;
        MPI_Recv(vec.data(), vec.size(), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &stat);
    }
    // the process broadcasts its column to each other
    std::vector <double> tmp_col(rows);
    for (size_t row = 0; row < rows; ++row) {
        if (static_cast<int>(row) % size == rank) {
            size_t index = 0;
            for (size_t i = rows * (row / size); i < rows * (row / size + 1); ++i) {
                tmp_col[index++] = vec[i];
            }
        }
        MPI_Bcast(tmp_col.data(), rows, MPI_DOUBLE, row % size, MPI_COMM_WORLD);
        for (int j = row / size; j < (delta + (rank < remaining ? 1 : 0)); ++j) {
            double tmp = vec[j * rows + row];
            for (size_t k = 0; k < rows; ++k) {
                if (k == row) {
                    vec[j * rows + k] /= tmp_col[row];
                } else {
                    vec[j * rows + k] -= tmp * tmp_col[k] / tmp_col[row];
                }
            }
        }
    }

    // after previous actions the process, which was working with b row sends results to 0 process.
    if ((cols - 1) % size == (size_t)rank) {
        MPI_Request request;
        MPI_Isend(vec.data() + ((cols - 1) / size) * rows, rows, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &request);
    }

    if (rank == 0) {
        vec.resize(rows);
        MPI_Status status;
        MPI_Recv(vec.data(), rows, MPI_DOUBLE, (cols - 1) % size, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    return vec;
}
