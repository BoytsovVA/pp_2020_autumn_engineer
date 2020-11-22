// Copyright 2020 Novikov Danil
#include <mpi.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "../../../modules/task_2/novikov_d_allreduce/allreduce.h"

template <typename Type>
void MPI_operation(Type* array1, Type* array2, int count, MPI_Op op) {
    if (op == MPI_SUM) {
        for (int i = 0; i < count; i++) {
            array1[i] += array2[i];
        }
    } else if (op == MPI_MAX) {
        for (int i = 0; i < count; i++) {
            array1[i] = std::max(array1[i], array2[i]);
        }
    } else if (op == MPI_MIN) {
        for (int i = 0; i < count; i++) {
            array1[i] = std::min(array1[i], array2[i]);
        }
    } else if (op == MPI_PROD) {
        for (int i = 0; i < count; i++) {
            array1[i] *= array2[i];
        }
    }
}

void my::MPI_Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    int size;
    int rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    void* tmp;
    if (datatype == MPI_INT) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(int));
        tmp = new int[count];
    } else {
        if (datatype == MPI_FLOAT) {
            std::memcpy(recvbuf, sendbuf, count * sizeof(float));
            tmp = new float[count];
        } else {
            if (datatype == MPI_DOUBLE) {
                std::memcpy(recvbuf, sendbuf, count * sizeof(double));
                tmp = new double[count];
            }
        }
    }

    int s = size;
    int m = 1;

    while (s > 1) {
        s = s / 2 + s % 2;

        if ((rank - m) % (2 * m) == 0) {
            MPI_Send(recvbuf, count, datatype, rank - m, 0, MPI_COMM_WORLD);
        }

        if ((rank % (2 * m) == 0) && (size - rank > m)) {
            MPI_Status status;
            MPI_Recv(tmp, count, datatype, rank + m, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (datatype == MPI_INT) {
                MPI_operation(static_cast<int*>(recvbuf), static_cast<int*>(tmp), count, op);
            } else {
                if (datatype == MPI_FLOAT) {
                    MPI_operation(static_cast<float*>(recvbuf), static_cast<float*>(tmp), count, op);
                } else {
                    if (datatype == MPI_DOUBLE) {
                        MPI_operation(static_cast<double*>(recvbuf), static_cast<double*>(tmp), count, op);
                    }
                }
            }
        }
        m = 2 * m;
    }

    if (datatype == MPI_INT) {
        delete[] static_cast<int*>(tmp);
    } else {
        if (datatype == MPI_FLOAT) {
            delete[] static_cast<float*>(tmp);
        } else {
            if (datatype == MPI_DOUBLE) {
                delete[] static_cast<double*>(tmp);
            }
        }
    }
}
