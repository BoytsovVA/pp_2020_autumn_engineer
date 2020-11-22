// Copyright 2020 Ivanov Yaroslav
#ifndef MODULES_TASK_2_IVANOV_Y_CONTRAST_CONTRAST_H_
#define MODULES_TASK_2_IVANOV_Y_CONTRAST_CONTRAST_H_

#include <vector>

std::vector<int> CreateRandomImg(const int width, const int height);
std::vector<int> LocalContrast(std::vector<int> Arr, int height, int width);
std::vector<int> ParallelContrast(std::vector<int> Arr, int height, int width);

#endif  // MODULES_TASK_2_IVANOV_Y_CONTRAST_CONTRAST_H_
