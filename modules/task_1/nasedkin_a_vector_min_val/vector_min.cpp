// Copyright 2020 Nasedkin Nikita
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>


std::vector<int> rndVector(int vector_size) {
  if (vector_size <= 0) {
    throw("Incorrect Vector Size");
  }
  std::mt19937 random;
  random.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> vec(vector_size);
  for (int i = 0; i < vector_size; i++) { vec[i] = random() % 100; }
  return vec;
}

int SequentialMininum(std::vector<int> vec) {
  int min_element = vec[0];
  int vector_size = vec.size();
  for (int i = 1; i < vector_size; i++)
    if (min_element > vec[i]) { min_element = vec[i]; }
  return min_element;
}

int ParallelMinimum(std::vector<int> source_vector, int vector_size) {
  int procRank, procSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procSize);
  int step = vector_size / procSize;

    if (procRank == 0) {
      for (int i = 1; i < procSize; i++) {
        MPI_Send(&source_vector[0] + i * step, step, MPI_INT, i, 0, MPI_COMM_WORLD);
      }
    }

    std::vector<int> local_vector(step);

    if (procRank == 0) {
      local_vector = std::vector<int>(source_vector.begin(), source_vector.begin() + step);
    } else {
      MPI_Status stat;
      MPI_Recv(&local_vector[0], step, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    }
  int vector_min_val = 0;

  int local_vector_min_val = SequentialMininum(local_vector);

  MPI_Op op_code = MPI_MIN;

  MPI_Reduce(&local_vector_min_val, &vector_min_val, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);

  return vector_min_val;
}


