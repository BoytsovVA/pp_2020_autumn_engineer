// Copyright 2020 Osipov Nikolay
#include <mpi.h>
#include <time.h>
#include <vector>
#include <random>
#include <cassert>
#include <climits>
#include "../../../modules/task_1/osipov_n_matrix_column_max/matrix_column_max.h"

std::vector<int> getMatrix(int line, int column) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<> dist(0, 100);
    std::vector<int> matrix(line * column);
    for (int i = 0; i < line * column; ++i) {
        matrix[i] = dist(gen);
    }
    return matrix;
}

std::vector<int> transposedMatrix(std::vector<int> matrix, int line, int column) {
    assert(matrix.size() != 0 && line != 0 && column != 0);
    std::vector<int> transMatrix(line * column);
    for (int i = 0; i < line; ++i) {
        for (int j = 0; j < column; ++j) {
            transMatrix[j * line + i] = matrix[i * column + j];
        }
    }
    return transMatrix;
}

int getColumnMaxSequential(std::vector<int> matrix, int line, int column) {
    std::vector<int> transMatrix = transposedMatrix(matrix, line, column);
    int currentMax = 0;
    int max = 0;
    int k = 0;
    for (int i = 1; i < column + 1; ++i) {
        while (k != i * line) {
            if (transMatrix[k] > currentMax)
                currentMax = transMatrix[k];
            k += 1;
        }
        if (currentMax > max) {
            max = currentMax;
        }
        currentMax = 0;
    }

    return max;
}

int getColumnMaxParallel(std::vector<int> matrix, int line, int column) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // number of process
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // kolichestvo of process

    const int delta = column / size;
    int remainder = column - size * delta;
    std::vector<int> transMatrix;
    // send to slave proccess
    if (rank == 0) {
        transMatrix.resize(line * column);
        transMatrix = transposedMatrix(matrix, line, column);
        if (delta > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&transMatrix[(proc * delta + remainder) * line], delta * line,
                    MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }
    }

    int* localVec;
    // receive from master proccess
    if (rank == 0) {
        localVec = &transMatrix[0];
    } else {
        if (delta > 0) {
            localVec = new int[delta * line];
            MPI_Status status;
            MPI_Recv(localVec, delta * line, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }

    int verySuperMax;
    int localMax = INT_MIN;
    int max = INT_MIN;
    MPI_Op op_code = MPI_MAX;
    if (rank == 0) {
        for (int i = 0; i < delta + remainder; ++i) {
            for (int j = 0; j < line; ++j) {
                if (localVec[i * line + j] > max)
                max = localVec[i * line + j];
            }
            if (max > localMax) localMax = max;
            max = INT_MIN;
        }
        MPI_Reduce(&localMax, &verySuperMax, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 0; i < delta; ++i) {
            for (int j = 0; j < line; ++j) {
                if (localVec[i * line + j] > max)
                    max = localVec[i * line + j];
            }
            if (max > localMax) localMax = max;
            max = INT_MIN;
        }
        MPI_Reduce(&localMax, &verySuperMax, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    }
    return verySuperMax;
}
