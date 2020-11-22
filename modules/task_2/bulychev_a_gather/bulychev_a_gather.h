// Copyright 2020 Bulychev Andrey
#ifndef MODULES_TASK_2_BULYCHEV_A_GATHER_BULYCHEV_A_GATHER_H_
#define MODULES_TASK_2_BULYCHEV_A_GATHER_BULYCHEV_A_GATHER_H_

#include <mpi.h>
#include <vector>
#include <string>

int My_Gather(void* send_buf, int send_count, MPI_Datatype send_type, void* recv_buf,
    int recv_count, MPI_Datatype recv_type, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_BULYCHEV_A_GATHER_BULYCHEV_A_GATHER_H_
