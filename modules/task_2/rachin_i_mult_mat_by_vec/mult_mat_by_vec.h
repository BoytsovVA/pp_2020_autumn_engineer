// Copyright 2020 Rachin Igor
#ifndef MODULES_TASK_2_RACHIN_I_MULT_MAT_BY_VEC_MULT_MAT_BY_VEC_H_
#define MODULES_TASK_2_RACHIN_I_MULT_MAT_BY_VEC_MULT_MAT_BY_VEC_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int rows, int clmns);
std::vector<int> getRandomVector(int vsize);
std::vector<int> getParallelMult(std::vector<int> mx, int rows, int clmns, std::vector<int> vec, int vsize);
std::vector<int> getSequentialMult(std::vector<int> mx, int rows, int clmns, std::vector<int> vec, int vsize);

#endif  // MODULES_TASK_2_RACHIN_I_MULT_MAT_BY_VEC_MULT_MAT_BY_VEC_H_
