// Copyright 2020 Nasedkin Nikita
#include "../../modules/task_2/nasedkin_mpi_scatter/scatter.h"
#include <cstring>
#include <cstdlib>

int MyScatter(void* send_buf, int send_count, MPI_Datatype send_type,
  void* recv_buf, int recv_count, MPI_Datatype recv_type, int root, MPI_Comm comm) {
  if (root < 0) {
    throw - 1;
  }
  if (send_count != recv_count) {
    throw - 1;
  }

  int proc_rank, proc_size, send_type_size, recv_type_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

  MPI_Type_size(send_type, &send_type_size);
  MPI_Type_size(recv_type, &recv_type_size);

  int recv_step = recv_count * recv_type_size;
  int send_step = send_count * send_type_size;

  if (proc_rank == root) {
    std::memcpy(recv_buf, static_cast<char*>(send_buf) + root * recv_step, recv_step);

    for (int i = 0; i < proc_size; i++) {
      if (i == root) {
        continue;
      }
      MPI_Send(static_cast<char*>(send_buf) + i * send_step,
        send_count, send_type, i, 0, comm);
    }
  } else {
    MPI_Status status;
    MPI_Recv(recv_buf, recv_count, recv_type, root, 0, comm, &status);
  }
  return MPI_SUCCESS;
}
