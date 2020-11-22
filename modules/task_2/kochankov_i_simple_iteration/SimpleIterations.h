// Copyright 2020 Kochankov Ilya

#ifndef MODULES_TASK_2_KOCHANKOV_I_SIMPLE_ITERATION_SIMPLEITERATIONS_H_
#define MODULES_TASK_2_KOCHANKOV_I_SIMPLE_ITERATION_SIMPLEITERATIONS_H_

#include "iostream"
#include "vector"

#include "./Matrix.h"

using std::vector;

Matrix get_input_matrix();
double get_input_accuracy();
Matrix get_rand_matrix(int x, int y);
int is_correct_matrix(const Matrix& matrix);
void print_result(const vector<double> &result);
vector<double> linear_simple_iteration(const Matrix& matrix, double accuracy);
vector<double> parallel_simple_iteration(const Matrix& matrix, double accuracy);

#endif  // MODULES_TASK_2_KOCHANKOV_I_SIMPLE_ITERATION_SIMPLEITERATIONS_H_
