// Copyright 2020 Kryukov Sergey
#ifndef MODULES_TASK_2_KRYUKOV_S_TOPOLOGY_STAR_TOPOLOGY_STAR_H_
#define MODULES_TASK_2_KRYUKOV_S_TOPOLOGY_STAR_TOPOLOGY_STAR_H_

#include <mpi.h>
#include <vector>
#include <string>

int* getRandomVector(int size);
std::vector<int> initialVectorNodes(std::vector<int> inputV, int Size);
MPI_Comm createTopologyStar(MPI_Comm comm, std::vector<int> MasNodes);
std::vector<int> createMasEdges(std::vector<int> inputV, int Size);
std::vector<int> createMasIndex(std::vector<int> inputV, int Size);
int* SendRecvStar(int* buf, int count, MPI_Datatype datatype,
    int dest, int send_tag, MPI_Comm comm, int source, int rank);

#endif  // MODULES_TASK_2_KRYUKOV_S_TOPOLOGY_STAR_TOPOLOGY_STAR_H_
