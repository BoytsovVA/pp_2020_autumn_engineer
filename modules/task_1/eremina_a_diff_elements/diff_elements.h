// Copyright 2020 Eremina Alena
#ifndef MODULES_TASK_1_EREMINA_A_DIFF_ELEMENTS_DIFF_ELEMENTS_H_
#define MODULES_TASK_1_EREMINA_A_DIFF_ELEMENTS_DIFF_ELEMENTS_H_

#include <mpi.h>
#include <vector>

std::vector<int> getRandomVector(int size);
void maxDiffElems(int* in, int* out, int* len, MPI_Datatype *dtype);
void getSequentialOperations(std::vector<int> vec, int* elems);
void getParallelOperations(std::vector<int> global_vec, int count_size_vector, int* max_diff_elems);

#endif  // MODULES_TASK_1_EREMINA_A_DIFF_ELEMENTS_DIFF_ELEMENTS_H_
