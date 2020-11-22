// Copyright 2020 Rachin Igor
#ifndef MODULES_TASK_1_RACHIN_I_MAX_VAL_MAT_COLUMNS_MAX_OF_CLMNS_MTRX_H_
#define MODULES_TASK_1_RACHIN_I_MAX_VAL_MAT_COLUMNS_MAX_OF_CLMNS_MTRX_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int rows, int clmns);
std::vector<int> getParallelMaxOfClmns(std::vector<int> mx, int rows, int clmns);
std::vector<int> getSequentialMaxOfClmns(std::vector<int> mx, int rows, int clmns);

#endif  // MODULES_TASK_1_RACHIN_I_MAX_VAL_MAT_COLUMNS_MAX_OF_CLMNS_MTRX_H_
