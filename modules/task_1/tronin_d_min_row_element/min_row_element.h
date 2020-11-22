// Copyright 2020 Tronin Dmitry
#ifndef MODULES_TASK_1_TRONIN_D_MIN_ROW_ELEMENT_MIN_ROW_ELEMENT_H_
#define MODULES_TASK_1_TRONIN_D_MIN_ROW_ELEMENT_MIN_ROW_ELEMENT_H_

#include <vector>
#include <random>
#include <algorithm>

std::vector<int32_t> getRandomVector(size_t size);

std::vector<int32_t> getMinRowElementSequential(
        const std::vector<int32_t>& vector,
        size_t size);

std::vector<int32_t> getMinRowElementParallel(
        const std::vector<int32_t>& vector,
        size_t size);

#endif  // MODULES_TASK_1_TRONIN_D_MIN_ROW_ELEMENT_MIN_ROW_ELEMENT_H_
