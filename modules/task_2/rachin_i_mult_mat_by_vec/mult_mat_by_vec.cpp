// Copyright 2020 Rachin Igor
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../modules/task_2/rachin_i_mult_mat_by_vec/mult_mat_by_vec.h"

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

std::vector<int> getRandomVector(int vsize) {
    if (vsize <= 0) {
        return std::vector<int>();
    }
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(vsize);
    for (int i = 0; i < vsize; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

std::vector<int> getSequentialMult(std::vector<int> mx, int rows, int clmns, std::vector<int> vec, int vsize) {
    if (rows <= 0 || clmns <= 0 || vsize != clmns) {
        return std::vector<int>();
    }
    std::vector<int> result(rows);
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < clmns; j++) {
            result[i] += mx[i * clmns + j] * vec[j];
        }
    }
    return result;
}

std::vector<int> getParallelMult(std::vector<int> mx, int rows, int clmns, std::vector<int> vec, int vsize) {
    if (rows <= 0 || clmns <= 0 || vsize != clmns) {
        return std::vector<int>();
    }
    int size, rank;
    std::vector<int> result(rows);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size > clmns) {
        if (rank == 0) {
            result = getSequentialMult(mx, rows, clmns, vec, vsize);
        }
    } else {
        int delta = clmns / size;
        int tail = clmns % size;
        std::vector<int> localMx(delta * rows);
        std::vector<int> localVec(delta);
        std::vector<int> localTail(rows * tail);
        if (rank == 0) {
            for (int proc = 1; proc < size; proc++) {
                for (int i = 0; i < rows; i++) {
                    MPI_Send(&mx[proc * delta + i * clmns], delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
                }
                MPI_Send(&vec[proc * delta], delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
            for (int i = 0; i < rows; i++) {
                localMx.insert(localMx.begin() + i * delta,
                    mx.begin() + i * clmns, mx.begin() + i * clmns + delta);
            }
            localVec.insert(localVec.begin(),
                    vec.begin(),
                    vec.begin() + delta);
            if (tail != 0) {
                for (int i = 0; i < rows; i++) {
                    localTail.insert(localTail.begin() + i * tail,
                        mx.begin() + delta * size + clmns * i,
                        mx.begin() + delta * size + clmns * i + tail);
                }
                std::vector<int> tmp(tail);
                tmp.insert(tmp.begin(),
                        vec.begin() + delta * size,
                        vec.begin() + delta * size + tail);
                localTail = getSequentialMult(localTail, rows, tail, tmp, tail);
            }
        } else {
            MPI_Status status;
            for (int i = 0; i < rows; i++) {
                MPI_Recv(&localMx[0 + i * delta], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            }
            MPI_Recv(&localVec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
        std::vector<int> localResult(rows);
        localResult = getSequentialMult(localMx, rows, delta, localVec, delta);
        MPI_Reduce(&localResult[0], &result[0], rows, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rank == 0 && tail != 0) {
            for (int i = 0; i < rows; i++) {
                result[i] += localTail[i];
            }
        }
    }
    return result;
}
