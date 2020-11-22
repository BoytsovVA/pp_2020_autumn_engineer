// Copyright 2020 Ptitsyn Pavel
#ifndef  MODULES_TASK_1_PTITSYN_P_MIN_COLUMS_MIN_COLUMS_H_
#define  MODULES_TASK_1_PTITSYN_P_MIN_COLUMS_MIN_COLUMS_H_

#include <vector>

std::vector<int> getRandomVector(int cols, int rows);
std::vector<int> getParallelOperations(std::vector<int> global_mat, int cols, int rows);
std::vector<int> getSequentialOperations(std::vector<int> mat, int cols, int rows);
std::vector<int> tVector(std::vector<int> vec, int line, int column);

#endif  // MODULES_TASK_1_PTITSYN_P_MIN_COLUMS_MIN_COLUMS_H_
