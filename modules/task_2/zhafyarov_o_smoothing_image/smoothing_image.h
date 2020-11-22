// Copyright 2020 Zhafyarov Oleg
#ifndef MODULES_TASK_2_ZHAFYAROV_O_SMOOTHING_IMAGE_SMOOTHING_IMAGE_H_
#define MODULES_TASK_2_ZHAFYAROV_O_SMOOTHING_IMAGE_SMOOTHING_IMAGE_H_

void DeleteDynamicArray(int** image, int height);

void MedianFilter(int* buffer, int height, int width);

void MedianFilter(int** image, int height, int width);

void SmoothingImageParallel(int** image, int height, int width);

int Comparator(int number);

#endif  // MODULES_TASK_2_ZHAFYAROV_O_SMOOTHING_IMAGE_SMOOTHING_IMAGE_H_
