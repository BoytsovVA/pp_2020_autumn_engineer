// Copyright 2020 Ekaterina Burmistrova
#ifndef MODULES_TASK_1_BOURMISTROVA_E_MATRIX_ELEM_SUM_MPI_FUN_H_
#define MODULES_TASK_1_BOURMISTROVA_E_MATRIX_ELEM_SUM_MPI_FUN_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int  sz1, int sz2);
int Sequential_method(std::vector<int> vect, std::string op);
int Parallel_method(std::vector<int> matr,
        int n_lin, int n_col, std::string op);
int Parallel_method_choose_num_proc(std::vector<int> matr, int n_lin,
        int n_col, std::string op, int total);

#endif  // MODULES_TASK_1_BOURMISTROVA_E_MATRIX_ELEM_SUM_MPI_FUN_H_

