// Copyright 2020 Chesnokov Artyom
#ifndef MODULES_TASK_1_CHESNOKOV_A_MATRIX_COLUMN_SUM_MATRIX_COLUMN_SUM_H_
#define MODULES_TASK_1_CHESNOKOV_A_MATRIX_COLUMN_SUM_MATRIX_COLUMN_SUM_H_

#include <vector>
typedef std::vector<std::vector<int>> Matrix;

// returns random matrix in column-major order
Matrix getRandomMatrix(int columns, int rows);
std::vector<int> getSequentialColumnSum(const Matrix& matrix);
std::vector<int> getParallelColumnSum(const Matrix& matrix);

#endif  // MODULES_TASK_1_CHESNOKOV_A_MATRIX_COLUMN_SUM_MATRIX_COLUMN_SUM_H_
