// Copyright 2020 Alexey Nekravtsev
#ifndef MODULES_TASK_1_NEKRAVTSEV_A_MATRIX_SUM_MATRIX_SUM_H_
#define MODULES_TASK_1_NEKRAVTSEV_A_MATRIX_SUM_MATRIX_SUM_H_

#include <vector>

std::vector<int> setRandomMatrix(int rows, int cols);

int SequentialCalc(std::vector<int> matrix, int size);

int ParallelCalc(std::vector<int> matrix, int size);

#endif  // MODULES_TASK_1_NEKRAVTSEV_A_MATRIX_SUM_MATRIX_SUM_H_
