// Copyright 2020 Maksimov Andrey
#ifndef MODULES_TASK_2_MAKSIMOV_A_MATR_VEC_MULT_MATR_VEC_MULT_H_
#define MODULES_TASK_2_MAKSIMOV_A_MATR_VEC_MULT_MATR_VEC_MULT_H_

#include <vector>

std::vector<int> getRandomVector(int size);
std::vector<int> getRandomMatrix(int rows, int columns);
std::vector<int> multiplyMatrixByVectorNotParall(
    std::vector<int> matr, int rows, int columns,
    std::vector<int> vec, int vecSize);
std::vector<int> multiplyMatrixByVector(
    std::vector<int> matr, int rows, int columns,
    std::vector<int> vec, int vecSize);

#endif  // MODULES_TASK_2_MAKSIMOV_A_MATR_VEC_MULT_MATR_VEC_MULT_H_
