// Copyright 2020 Bulychev Andrey
#include <mpi.h>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <string>
#include "../../../modules/task_2/bulychev_a_gather/bulychev_a_gather.h"

int My_Gather(void* send_buf, int send_count, MPI_Datatype send_type, void* recv_buf, int recv_count,
    MPI_Datatype recv_type, int root, MPI_Comm comm) {
    int size, rank, recv_type_size, send_type_size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    if (root < 0 || root >= size) {
        return MPI_ERR_ROOT;
    }
    MPI_Type_size(send_type, &send_type_size);
    if (send_type_size == MPI_ERR_TYPE) {
        return MPI_ERR_TYPE;
    }
    MPI_Type_size(recv_type, &recv_type_size);
    if (recv_type_size == MPI_ERR_TYPE) {
        return MPI_ERR_TYPE;
    }
    if (send_type != recv_type) {
        return MPI_ERR_TYPE;
    }
    if (send_count != recv_count || send_count <= 0 || recv_count <= 0) {
        return MPI_ERR_COUNT;
    }
    int tmp = root * recv_count * recv_type_size;
    int recv_tmp = recv_count * recv_type_size;
    int send_tmp = send_count * send_type_size;
    if (rank != root) {
        MPI_Send(send_buf, send_count, send_type, root, 0, comm);
    } else {
        std::memcpy(static_cast<char*>(recv_buf) + tmp, send_buf, send_tmp);
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Status status;
                MPI_Recv(static_cast<char*>(recv_buf) + i * recv_tmp, recv_count,
                    recv_type, i, 0, comm, &status);
            }
        }
    }
    return MPI_SUCCESS;
}
