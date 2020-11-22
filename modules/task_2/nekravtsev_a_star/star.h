// Copyright 2020 Alexey Nekravtsev
#ifndef MODULES_TASK_2_NEKRAVTSEV_A_STAR_STAR_H_
#define MODULES_TASK_2_NEKRAVTSEV_A_STAR_STAR_H_

#include <mpi.h>

int createStar(MPI_Comm oldcomm, MPI_Comm* comm_graph);

int starSend(void* message, int count, MPI_Datatype dt, int from, int to, int tag, MPI_Comm comm);

int starRecv(void* message, int count, MPI_Datatype dt, int from, int to, int tag, MPI_Comm comm, MPI_Status* status);

#endif  // MODULES_TASK_2_NEKRAVTSEV_A_STAR_STAR_H_
