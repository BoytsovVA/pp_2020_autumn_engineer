// Copyright 2020 Sozinov Alex
#ifndef MODULES_TASK_1_SOZINOV_A_MAX_ROW_MATRIX_MAX_ROW_MATRIX_H_
#define MODULES_TASK_1_SOZINOV_A_MAX_ROW_MATRIX_MAX_ROW_MATRIX_H_

#include <vector>

std::vector<int> GenerateMatrix(const int sizeRow, int countRow, int min, int max);
std::vector<int> GetSequentialMax(const std::vector<int> &vect, int sizeRow, int countRow);
std::vector<int> GetParalMax(const std::vector<int> &vect, int sizeRow, int countRow);

#endif    // MODULES_TASK_1_SOZINOV_A_MAX_ROW_MATRIX_MAX_ROW_MATRIX_H_
