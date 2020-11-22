// Copyright 2020 NDAH RONY
#ifndef MODULES_TASK_1_NDAH_VECTOR_ERROR_VEC_ER_MPI_H_
#define MODULES_TASK_1_NDAH_VECTOR_ERROR_VEC_ER_MPI_H_

#include <vector>

std::vector<int> RV(int s);
std::vector<int> vecNor(std::vector<int> a);
int parallelVec(const std::vector<int>& a, int n);
int countErr(const std::vector<int>& a);


#endif  // MODULES_TASK_1_NDAH_VECTOR_ERROR_VEC_ER_MPI_H_

