// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_FROLOVA_O_MIN_VAL_MAT_COLUMNS_MIN_VAL_MAT_COLUMNS_H_
#define MODULES_TASK_1_FROLOVA_O_MIN_VAL_MAT_COLUMNS_MIN_VAL_MAT_COLUMNS_H_
#include <vector>
#include <string>

void getRandomMatrix(int** matrix, int rows, int columns);
void matrixToArray(int** matrix, int* array, int rows, int columns);
void getParallelMin(int* global_array, int* parallel_array, int rows, int columns);
void getCheckArray(int* vector, int* min_array, int rows, int columns);

#endif  // MODULES_TASK_1_FROLOVA_O_MIN_VAL_MAT_COLUMNS_MIN_VAL_MAT_COLUMNS_H_
