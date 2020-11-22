// Copyright 2020 Sharov Alexander
#ifndef MODULES_TASK_1_SHAROV_A_VECTOR_DIFFERENT_VECTOR_DIFFERENT_H_
#define MODULES_TASK_1_SHAROV_A_VECTOR_DIFFERENT_VECTOR_DIFFERENT_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int sz);
void Different(int* first, int* second, int* lenght, MPI_Datatype* dtype);
void getSequentialOperations(std::vector<int> vector, int* elems);
void getParallelOperations(std::vector<int> global_vec, int vector_size, int* diff_elem);

#endif  // MODULES_TASK_1_SHAROV_A_VECTOR_DIFFERENT_VECTOR_DIFFERENT_H_

