// Copyright 2020 Ptitsyn Pavel
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/ptitsyn_p_min_colums/min_colums.h"

std::vector<int> getRandomVector(int line, int column) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::uniform_int_distribution<> dist(0, 100);
  std::vector<int> vec(line * column);
  for (int i = 0; i < line * column; i++) {
    vec[i] = dist(gen);
  }
  return vec;
}

std::vector<int> getSequentialOperations(std::vector<int> vec, int column, int line) {
  std::vector<int> result(line);
  vec = tVector(vec, column, line);
  for (int i = 0; i < line; i++) {
    int min_element = vec[i * column];
    for (int j = 1; j < column; j++)
      min_element = std::min(min_element, vec[i * column + j]);
    result[i] = min_element;
  }
  return result;
}

std::vector<int> getParallelOperations(std::vector<int> gvec, int column, int line) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = line / size;
  const int ost = line % size;
  if (rank == 0) {
    for (int j = 1; j < size; j++) {
      MPI_Send(gvec.data() + j * delta * column + ost * column, delta * column, MPI_INT, j, 0, MPI_COMM_WORLD);
    }
  }
  std::vector<int> lvec(rank == 0 ? (delta + ost) * column : delta * column);
  if (rank == 0) {
    lvec = {gvec.begin(), gvec.begin() + (delta + ost) * column};
  } else {
    MPI_Status status;
    MPI_Recv(lvec.data(), delta * column, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  std::vector<int> result(line);
  std::vector<int> local_result = getSequentialOperations(lvec, column, rank == 0 ? delta + ost : delta);
  if (rank == 0) {
    for (int i = 0; i < delta + ost; i++) result[i] = local_result[i];
        for (int j = 1; j < size; j++) {
            MPI_Status status;
            MPI_Recv(result.data() + ost + delta * j, delta, MPI_INT, j, 0, MPI_COMM_WORLD, &status);
    }
  } else {
    MPI_Send(local_result.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return result;
}

std::vector<int> tVector(std::vector<int> vec, int line, int column) {
  std::vector<int> tvec(line * column);
  tvec = vec;
  for (int i = 0; i < column; i++)
    for (int j = 0; j < line; j++)
        vec[j + i * line] = tvec[i + j * column];
  return vec;
}
