// Copyright 2020 Alexey Nekravtsev
#include "../../modules/task_1/nekravtsev_a_matrix_sum/matrix_sum.h"
#include <mpi.h>
#include <time.h>
#include <random>
#include <vector>

std::vector<int> setRandomMatrix(int rows, int cols) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> result(rows * cols);

  for (int i = 0; i < rows * cols; i++) {
    result[i] = gen() % 100;
  }
  return result;
}

int SequentialCalc(std::vector<int> matrix, int size) {
  int result = 0;

  for (int i = 0; i < size; i++) {
    result += matrix[i];
  }

  return result;
}

int ParallelCalc(std::vector<int> matrix, int size) {
  int procNum;
  int procRank;

  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  int delta = size / procNum;
  int remainder = size % procNum;

  if (procRank == 0) {
    for (int current_proc = 1; current_proc < procNum; current_proc++)
      MPI_Send(&matrix[current_proc * delta + remainder], delta, MPI_INT, current_proc, 0, MPI_COMM_WORLD);
  }

  std::vector<int> local_vector(delta);

  if (procRank == 0) {
    local_vector.resize(delta + remainder);
    local_vector = std::vector<int>(matrix.begin(), matrix.begin() + delta + remainder);
  } else {
    MPI_Status status;
    MPI_Recv(&local_vector[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  int result = 0;
  int local_sum = SequentialCalc(local_vector, local_vector.size());
  MPI_Reduce(&local_sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return result;
}
