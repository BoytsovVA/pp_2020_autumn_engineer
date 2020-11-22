// Copyright 2020 Maksimov Andrey
#include <mpi.h>
#include <time.h>
#include <vector>
#include <random>

#define MAX_GEN 100

int* getRandomVector(int size) {
    if (size < 1) {
        throw "size < 1 error";
    }

    std::mt19937 gen;
    gen.seed(time(0));

    int* vec = new int[size];
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % MAX_GEN;
    }
    return vec;
}

double getVectorAvgNotParall(int* vec, int vecSize) {
    int sum = 0;
    for (int i = 0; i < vecSize; i++) {
        sum += vec[i];
    }
    return static_cast<double>(sum) / vecSize;
}

double getVectorAvg(int* vec, int vecSize) {
    int procNum, procRank, size, sum, sumAll;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    MPI_Bcast(&vecSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (procRank != procNum - 1) {
        size = vecSize / procNum;
    } else {
        size = vecSize / procNum + vecSize % procNum;
    }

    if (procRank == 0) {
        for (int i = 1; i < procNum - 1; i++) {
            MPI_Send(&vec[0] + vecSize / procNum * i, size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        if (procNum > 1) {
            MPI_Send(&vec[0] + vecSize / procNum * (procNum - 1),
                     size + vecSize % procNum, MPI_INT, (procNum - 1), 0, MPI_COMM_WORLD);
        }
    }

    int* localVec = new int[size];

    if (procRank == 0) {
        for (int i = 0; i < size; i++) {
            localVec[i] = vec[i];
        }
    } else {
        MPI_Status mpiStatus;
        MPI_Recv(&localVec[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
    }

    sum = 0;
    for (int i = 0; i < size; i++) {
        sum += localVec[i];
    }
    MPI_Reduce(&sum, &sumAll, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    delete[] localVec;

    return static_cast<double>(sumAll) / vecSize;
}
