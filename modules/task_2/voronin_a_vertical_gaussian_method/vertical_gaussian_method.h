// Copyright 2020 Voronin Aleksey
#ifndef MODULES_TASK_2_VORONIN_A_VERTICAL_GAUSSIAN_METHOD_VERTICAL_GAUSSIAN_METHOD_H_
#define MODULES_TASK_2_VORONIN_A_VERTICAL_GAUSSIAN_METHOD_VERTICAL_GAUSSIAN_METHOD_H_

#include <vector>
std::vector<double> getRandomMatrixLinear(const int rows);
std::vector<double> calculateResults(std::vector<double> matrix, int rows, std::vector<double> values);
std::vector<double> sequentialGaussianMethod(std::vector<double> matrix, int rows);
std::vector <double> parallelGaussianMethod(std::vector <double> matrix, size_t rows);
#endif  // MODULES_TASK_2_VORONIN_A_VERTICAL_GAUSSIAN_METHOD_VERTICAL_GAUSSIAN_METHOD_H_
