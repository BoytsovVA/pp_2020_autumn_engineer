// Copyright 2020 Chesnokov Artyom

#include "../../../modules/task_2/chesnokov_a_scatter/scatter.h"
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <utility>

int My_Scatter_By_Tree(const void* sendbuf, int sendcount,
                       MPI_Datatype sendtype, void* recvbuf, int recvcount,
                       MPI_Datatype recvtype, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    // local storage fot intermidiate data
    // first sendcount elements belong to process
    void * temp_buf = nullptr;
    // just memory move for root process
    if (rank == root) {
        if (sendtype == MPI_INT) {
            temp_buf = new int[sendcount * size];
            // cpy own data first
            memcpy(temp_buf, reinterpret_cast<const int*>(sendbuf) + sendcount * root,
                static_cast<size_t>(sendcount * sizeof(int)));
            // cpy all data before root
            memcpy(reinterpret_cast<int*>(temp_buf) + sendcount, reinterpret_cast<const int*>(sendbuf),
                static_cast<size_t>(sendcount * sizeof(int) * root));
            // cpy all data after root
            memcpy(reinterpret_cast<int*>(temp_buf) + sendcount * (root + 1), reinterpret_cast<const int*>(sendbuf)
                + sendcount * (root + 1), static_cast<size_t>(sendcount * sizeof(int) * (size - root - 1)));
        } else if (sendtype == MPI_FLOAT) {
            temp_buf = new float[sendcount * size];
            // cpy own data first
            memcpy(temp_buf, reinterpret_cast<const float*>(sendbuf) + sendcount * root,
                static_cast<size_t>(sendcount * sizeof(float)));
            // cpy all data before root
            memcpy(reinterpret_cast<float*>(temp_buf) + sendcount, reinterpret_cast<const float*>(sendbuf),
                static_cast<size_t>(sendcount * sizeof(float) * root));
            // cpy all data after root
            memcpy(reinterpret_cast<float*>(temp_buf) + sendcount * (root + 1), reinterpret_cast<const float*>(sendbuf)
                + sendcount * (root + 1), static_cast<size_t>(sendcount * sizeof(float) * (size - root - 1)));
        } else if (sendtype == MPI_DOUBLE) {
            temp_buf = new double[sendcount * size];
            // cpy own data first
            memcpy(temp_buf, reinterpret_cast<const double*>(sendbuf) + sendcount * root,
                static_cast<size_t>(sendcount * sizeof(double)));
            // cpy all data before root
            memcpy(reinterpret_cast<double*>(temp_buf) + sendcount, reinterpret_cast<const double*>(sendbuf),
                static_cast<size_t>(sendcount * sizeof(double) * root));
            // cpy all data after root
            memcpy(reinterpret_cast<double*>(temp_buf) + sendcount * (root + 1),
                reinterpret_cast<const double*>(sendbuf) + sendcount * (root + 1),
                static_cast<size_t>(sendcount * sizeof(double) * (size - root - 1)));
        }
    }
    std::vector<int> * O = new std::vector<int>[size];
    // Initialzing O
    O[root].push_back(root);
    for (int i = 0; i < size; i++) {
        if (i != root) O[root].push_back(i);
    }
    int count;
    do {
        count = 0;
        for (int i = 0; i < size; i++) {
            if (O[i].size() <= 1) continue;
            // case when O[i] contain not only its data
            count++;
            size_t half = O[i].size() / 2;
            int recipient = O[i][half];
            // update state
            O[recipient].resize(O[i].size() - half);
            std::move(O[i].begin() + half, O[i].end(), O[recipient].begin());
            O[i].resize(half);
            if (rank == i) {
                // sending
                if (sendtype == MPI_INT) {
                    MPI_Send(reinterpret_cast<int*>(temp_buf) + half * sendcount,
                        sendcount * static_cast<int>(O[recipient].size()),
                        sendtype, recipient, 0, comm);
                } else if (sendtype == MPI_FLOAT) {
                    MPI_Send(reinterpret_cast<float*>(temp_buf) + half * sendcount,
                        sendcount * static_cast<int>(O[recipient].size()),
                        sendtype, recipient, 0, comm);
                } else if (sendtype == MPI_DOUBLE) {
                    MPI_Send(reinterpret_cast<double*>(temp_buf) + half * sendcount,
                        sendcount * static_cast<int>(O[recipient].size()),
                        sendtype, recipient, 0, comm);
                }
            } else if (rank == recipient) {
                // recieving
                if (temp_buf == nullptr) {
                    if (sendtype == MPI_INT) {
                        temp_buf = new int[sendcount * O[recipient].size()];
                    } else if (sendtype == MPI_FLOAT) {
                        temp_buf = new float[sendcount * O[recipient].size()];
                    } else if (sendtype == MPI_DOUBLE) {
                        temp_buf = new double[sendcount * O[recipient].size()];
                    }
                }
                MPI_Status status;
                MPI_Recv(temp_buf, static_cast<int>(sendcount * O[recipient].size()),
                    recvtype, i, 0, comm, &status);
            }
        }
    } while (count != 0);  // main cycle ends when all proccess have only its data

    // moving data to recvbuf
    int Size;
    MPI_Type_size(recvtype, &Size);
    memcpy(recvbuf, temp_buf, static_cast<size_t>(Size * recvcount));

    if (sendtype == MPI_INT) {
        delete[] static_cast<int*>(temp_buf);
    } else if (sendtype == MPI_FLOAT) {
        delete[] static_cast<float*>(temp_buf);
    } else if (sendtype == MPI_DOUBLE) {
        delete[] static_cast<double*>(temp_buf);
    }
    delete[] O;
    return MPI_SUCCESS;
}

int My_Scatter_Naive(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                     void* recvbuf, int recvcount, MPI_Datatype recvtype,
                     int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    if (rank == root) {
        if (sendtype == MPI_INT) {
            // local moving
            for (int i = 0; i < sendcount; i++) {
                reinterpret_cast<int*>(recvbuf)[i] = reinterpret_cast<const int*>(sendbuf)[i];
            }
            for (int i = 0; i < size; i++) {
                if (i != root)
                    MPI_Send(reinterpret_cast<const int*>(sendbuf) + i * sendcount,
                        sendcount, sendtype, i, 0, comm);
            }
        } else if (sendtype == MPI_FLOAT) {
            // local moving
            for (int i = 0; i < sendcount; i++) {
                reinterpret_cast<float*>(recvbuf)[i] = reinterpret_cast<const float*>(sendbuf)[i];
            }
            for (int i = 0; i < size; i++) {
                if (i != root)
                    MPI_Send(reinterpret_cast<const float*>(sendbuf) + i * sendcount,
                        sendcount, sendtype, i, 0, comm);
            }
        } else if (sendtype == MPI_DOUBLE) {
            for (int i = 0; i < sendcount; i++) {
                reinterpret_cast<double*>(recvbuf)[i] = reinterpret_cast<const double*>(sendbuf)[i];
            }
            for (int i = 0; i < size; i++) {
                if (i != root)
                    MPI_Send(reinterpret_cast<const double*>(sendbuf) + i * sendcount,
                        sendcount, sendtype, i, 0, comm);
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, &status);
    }
    return MPI_SUCCESS;
}
