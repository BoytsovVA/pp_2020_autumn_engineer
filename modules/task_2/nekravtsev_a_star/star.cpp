// Copyright 2020 Alexey Nekravtsev
#include "../../modules/task_2/nekravtsev_a_star/star.h"
#include <mpi.h>
#include <time.h>
#include <vector>
#include <iostream>

int createStar(MPI_Comm comm, MPI_Comm* comm_graph) {
  int procRank, nodes, result;
  MPI_Comm newComm;

  MPI_Comm_rank(comm, &procRank);
  MPI_Comm_size(comm, &nodes);

  if (nodes == 1) {
    int ind[1] = { 0 };
    int edg[1] = { 0 };
    result = MPI_Graph_create(comm, nodes, ind, edg, 0, &newComm);
    MPI_Comm_dup(newComm, comm_graph);
    return result;
  }

  std::vector<int> index(nodes);
  std::vector<int> edges(2 * nodes - 2);

  index[0] = nodes - 1;
  for (int i = 1; i < nodes; i++) {
    index[i] = index[i - 1] + 1;
  }

  for (int i = 0; i < 2 * nodes - 2; i++) {
    edges[i] = (i < nodes - 1) ? i + 1 : 0;
  }

  int* ind = &index[0];
  int* edg = &edges[0];
  result = MPI_Graph_create(comm, nodes, ind, edg, 0, &newComm);

  MPI_Comm_dup(newComm, comm_graph);
  MPI_Comm_free(&newComm);

  return result;
}

int starSend(void* message, int count, MPI_Datatype dt, int from, int to, int tag, MPI_Comm comm) {
  int procRank, procNum;
  MPI_Status status;

  MPI_Comm_rank(comm, &procRank);
  MPI_Comm_size(comm, &procNum);

  if (procNum == 1) {
    throw "Deadlock: process can't receive message!";
  }

  if (procRank == from) {
    if (from == 0) {
      MPI_Send(message, count, dt, to, tag, comm);
    } else {
      MPI_Send(message, count, dt, 0, tag, comm);
    }
  }

  if (procRank == 0) {
    if (from != 0 && to != 0) {
      MPI_Recv(message, count, dt, from, tag, comm, &status);
      MPI_Send(message, count, dt, to, tag, comm);
    }
  }

  return 1;
}

int starRecv(void* message, int count, MPI_Datatype dt, int from, int to, int tag, MPI_Comm comm, MPI_Status* stat) {
  int procRank, procNum;

  MPI_Comm_rank(comm, &procRank);
  MPI_Comm_size(comm, &procNum);

  if (procNum == 1) {
    throw "Deadlock: process can't receive message!";
  }

  if (procRank == to) {
    if (to == 0) {
      MPI_Recv(message, count, dt, from, tag, comm, stat);
    } else {
      MPI_Recv(message, count, dt, 0, tag, comm, stat);
    }
  }

  return 1;
}
