// Copyright 2020 Gorbunova Valeria
#include <mpi.h>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iostream>
#include <cstring>
#include "./../../modules/task_2/gorbunova_v_scatter/Scatter.h"

int gen_int(int arr[], int len) {
    for (int i = 0; i < len; i++) {
        arr[i] = std::rand() % 101 - 50;
    }
    return arr[len];
}

double gen_double(double arr[], int len) {
    for (int i = 0; i < len; i++)
        arr[i] = std::rand() % 101 - 50;
    return arr[len];
}

float gen_float(float arr[], int len) {
    for (int i = 0; i < len; i++)
        arr[i] = std::rand() % 101 - 50;
    return arr[len];
}


int MyScatter(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount,
    MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int size, rank, sendtype_size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Type_size(sendtype, &sendtype_size);
    if (sendcount != recvcount || sendcount <= 0 || recvcount <= 0 || root < 0 || root >= size)
        return MPI_ERR_COUNT;
    if (rank == root) {
        int p = sendcount * sendtype_size;
        std::memcpy(recvbuf, reinterpret_cast<char*>(sendbuf) + root * p, p);
        for (int i = 0; i < size; i++) {
            if (i != root) {
                int startval = i * sendcount;
                MPI_Send(static_cast<char*>(sendbuf) + startval * sendtype_size, sendcount, sendtype, i, 1, comm);
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(recvbuf, recvcount, recvtype, root, 1, comm, &status);
    }
    return MPI_SUCCESS;
}
