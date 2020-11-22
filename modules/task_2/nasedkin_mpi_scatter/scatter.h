// Copyright 2020 Nasedkin Nikita
#ifndef MODULES_TASK_2_NASEDKIN_MPI_SCATTER_SCATTER_H_
#define MODULES_TASK_2_NASEDKIN_MPI_SCATTER_SCATTER_H_

#include <mpi.h>
#include <vector>
#include <string>

int MyScatter(void* send_buf, int send_count, MPI_Datatype send_type,
  void* recv_buf, int recv_count, MPI_Datatype recv_type, int root, MPI_Comm comm);

#endif  //  MODULES_TASK_2_NASEDKIN_MPI_SCATTER_SCATTER_H_
