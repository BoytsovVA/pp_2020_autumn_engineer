// Copyright 2020 Rachin Igor
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../modules/task_1/rachin_i_max_val_mat_columns/max_of_clmns_mtrx.h"

std::vector<int> getRandomMatrix(int rows, int clmns) {
    if (rows <= 0 || clmns <= 0) {
        return std::vector<int>();
    }
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> mx(rows * clmns);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < clmns; j++) {
            mx[i * clmns + j] = gen() % 100;
        }
    }
    return mx;
}

std::vector<int> getSequentialMaxOfClmns(std::vector<int> mx, int rows, int clmns) {
    if (rows <= 0 || clmns <= 0) {
        return std::vector<int>();
    }
    std::vector<int> maxValues(clmns);
    for (int j = 0; j < clmns; j++) {
        maxValues[j] = mx[j];
    }
    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < clmns; j++) {
            if (mx[i * clmns + j] > maxValues[j]) {
                maxValues[j] = mx[i * clmns + j];
            }
        }
    }
    return maxValues;
}

std::vector<int> getParallelMaxOfClmns(std::vector<int> localMtrx, int rows, int clmns) {
    if (rows <= 0 || clmns <= 0) {
        return std::vector<int>();
    }
    int size, rank;
    std::vector<int> maxValues(clmns);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size > clmns) {
        if (rank == 0) {
            maxValues = getSequentialMaxOfClmns(localMtrx, rows, clmns);
        }
    } else {
        int delta = clmns / size;
        int tail = clmns % size;
        std::vector<int> localMain(delta * rows);
        std::vector<int> localTail(tail * rows);
        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                for (int i = 0; i < rows; i++) {
                    MPI_Send(&localMtrx[proc * delta + i * clmns], delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
                }
            }
            for (int i = 0; i < rows; i++) {
                localMain.insert(localMain.begin() + i * delta,
                    localMtrx.begin() + i * clmns, localMtrx.begin() + i * clmns + delta);
            }
            if (tail != 0) {
                for (int i = 0; i < rows; i++) {
                    localTail.insert(localTail.begin() + i * tail,
                        localMtrx.begin() + delta * size + clmns * i,
                        localMtrx.begin() + delta * size + clmns * i + tail);
                }
                localTail = getSequentialMaxOfClmns(localTail, rows, tail);
                for (int i = 0; i < tail; i++) {
                    maxValues[clmns - tail + i] = localTail[i];
                }
            }
        } else {
            MPI_Status status;
            for (int i = 0; i < rows; i++) {
                MPI_Recv(&localMain[0 + i * delta], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            }
        }
        localMain = getSequentialMaxOfClmns(localMain, rows, delta);
        MPI_Gather(&localMain[0], delta, MPI_INT, &maxValues[0], delta, MPI_INT, 0, MPI_COMM_WORLD);
    }
    return maxValues;
}
