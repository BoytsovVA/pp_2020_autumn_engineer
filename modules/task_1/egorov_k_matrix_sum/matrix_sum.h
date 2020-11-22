// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_EGOROV_K_MATRIX_SUM_MATRIX_SUM_H_
#define MODULES_TASK_1_EGOROV_K_MATRIX_SUM_MATRIX_SUM_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
int getParallelSum(std::vector<int> global_vec,
                          int count_size_vector);
int getSequentialSum(std::vector<int> vec);

#endif  // MODULES_TASK_1_EGOROV_K_MATRIX_SUM_MATRIX_SUM_H_
