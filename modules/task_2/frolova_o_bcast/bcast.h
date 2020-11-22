// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_2_FROLOVA_O_BCAST_BCAST_H_
#define MODULES_TASK_2_FROLOVA_O_BCAST_BCAST_H_
#include <mpi.h>
#include <vector>
#include <string>

std::vector<int> getRandomArray(int size);
std::vector<double> getRandomDoubleArray(int size);
std::vector<float> getRandomFloatArray(int size);
int checkSum(std::vector<int> array, int size);
double checkDoubleSum(std::vector<double> array, int size);
float checkFloatSum(std::vector<float> array, int size);
void my_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
void getSum(std::vector<int> array, int size, int *sum, int root);
void getDoubleSum(std::vector<double> array, int size, double *sum, int root);
void getFloatSum(std::vector<float> array, int size, float *sum, int root);
void difOfTime(int count, int root);

#endif  // MODULES_TASK_2_FROLOVA_O_BCAST_BCAST_H_
