// Copyright 2020 Kochankov Ilya

#ifndef MODULES_TASK_1_KOCHANKOV_I_MATRIX_COLUMNS_SUM_MATRIX_COLUMNS_SUM_H_
#define MODULES_TASK_1_KOCHANKOV_I_MATRIX_COLUMNS_SUM_MATRIX_COLUMNS_SUM_H_

#include <vector>
#include "Matrix.h"

using std::vector;

Matrix get_rand_matrix(int x, int y);
vector<int> sequential_operations(const Matrix& matrix);
vector<int> parallel_sum(const Matrix& matrix);

#endif  // MODULES_TASK_1_KOCHANKOV_I_MATRIX_COLUMNS_SUM_MATRIX_COLUMNS_SUM_H_
