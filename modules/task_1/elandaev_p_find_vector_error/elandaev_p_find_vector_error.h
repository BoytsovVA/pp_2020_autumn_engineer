// Copyright 2020 Elandaev Pavel

#ifndef MODULES_TASK_1_ELANDAEV_P_FIND_VECTOR_ERROR_ELANDAEV_P_FIND_VECTOR_ERROR_H_
#define MODULES_TASK_1_ELANDAEV_P_FIND_VECTOR_ERROR_ELANDAEV_P_FIND_VECTOR_ERROR_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector);
int getSequentialOperations(std::vector<int> vec, int sz);

#endif  // MODULES_TASK_1_ELANDAEV_P_FIND_VECTOR_ERROR_ELANDAEV_P_FIND_VECTOR_ERROR_H_
