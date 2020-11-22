// Copyright 2020 Pasukhin Dmitry

#ifndef MODULES_TASK_2_PASUKHIN_D_HYPERCUBE_HYPERCUBE_H_
#define MODULES_TASK_2_PASUKHIN_D_HYPERCUBE_HYPERCUBE_H_

#include <vector>
int randPath(int min, int max);
int powerHyperCube(const int _size);
void computeRoude(const int rank, const int power, int* collection);
int computeSize(const int power);
int reciver_nearby(const int _rank, const int _reciever, const int _power, const int* _dir);
int findRoude(const int rank_my, const int rank_past, const int power);
bool huperCubeTopology(std::vector<double> _message, const int _receiver, const int _sender);
bool checkProc(const int size);
int hyperTopoRecv(const int rank, const int size, double* collect);

#endif  // MODULES_TASK_2_PASUKHIN_D_HYPERCUBE_HYPERCUBE_H_
