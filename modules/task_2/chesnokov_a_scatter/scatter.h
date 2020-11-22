// Copyright 2020 Chesnokov Artyom
#ifndef MODULES_TASK_2_CHESNOKOV_A_SCATTER_SCATTER_H_
#define MODULES_TASK_2_CHESNOKOV_A_SCATTER_SCATTER_H_

#include <mpi.h>

int My_Scatter_By_Tree(const void *sendbuf, int sendcount,
                         MPI_Datatype sendtype, void *recvbuf, int recvcount,
                         MPI_Datatype recvtype, int root, MPI_Comm comm);

int My_Scatter_Naive(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                     void *recvbuf, int recvcount, MPI_Datatype recvtype,
                     int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_CHESNOKOV_A_SCATTER_SCATTER_H_
