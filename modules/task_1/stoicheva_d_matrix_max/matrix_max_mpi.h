// Copyright 2020 Stoicheva Darya
#ifndef MODULES_TASK_1_STOICHEVA_D_MATRIX_MAX_MATRIX_MAX_MPI_H_
#define MODULES_TASK_1_STOICHEVA_D_MATRIX_MAX_MATRIX_MAX_MPI_H_

#include <vector>
#include <string>

std::vector<int> get_max_elements_of_rows_sequentional(std::vector<int> matrix,
    const size_t rows, const size_t columns);
std::vector<int> get_max_elements_of_rows_parallel(const std::vector<int>& matrix,
    const size_t rows, const size_t columns);
std::vector<int> create_random_matrix(size_t rows, size_t columns);
void print_matrix(std::vector<int> matrix, const size_t rows, const size_t columns,
    std::string prefix);

// #define DEBUG_PRINT

#endif  // MODULES_TASK_1_STOICHEVA_D_MATRIX_MAX_MATRIX_MAX_MPI_H_
