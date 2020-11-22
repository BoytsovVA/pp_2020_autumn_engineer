// Copyright 2020 Osipov Nikolay
#ifndef MODULES_TASK_1_OSIPOV_N_MATRIX_COLUMN_MAX_MATRIX_COLUMN_MAX_H_
#define MODULES_TASK_1_OSIPOV_N_MATRIX_COLUMN_MAX_MATRIX_COLUMN_MAX_H_

#include <vector>

std::vector<int> getMatrix(int line, int column);
std::vector<int> transposedMatrix(std::vector<int> matrix, int line, int column);
int getColumnMaxSequential(std::vector<int> matrix, int line, int column);
int getColumnMaxParallel(std::vector<int> matrix, int line, int column);

#endif  // MODULES_TASK_1_OSIPOV_N_MATRIX_COLUMN_MAX_MATRIX_COLUMN_MAX_H_
