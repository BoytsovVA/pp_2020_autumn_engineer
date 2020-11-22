// Copyright 2020 Nasedkin Nikita
#ifndef MODULES_TASK_1_NASEDKIN_A_VECTOR_MIN_VAL_VECTOR_MIN_H_
#define MODULES_TASK_1_NASEDKIN_A_VECTOR_MIN_VAL_VECTOR_MIN_H_

#include <vector>
#include <string>

std::vector<int> rndVector(int vector_size);
int ParallelMinimum(std::vector<int> global_vec, int vector_size);
int SequentialMininum(std::vector<int> vect);

#endif  // MODULES_TASK_1_NASEDKIN_A_VECTOR_MIN_VAL_VECTOR_MIN_H_
