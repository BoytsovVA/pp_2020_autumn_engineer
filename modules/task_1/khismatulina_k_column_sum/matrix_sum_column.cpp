// Copyright 2020 Khismatulina Karina
#include <mpi.h>
#include <time.h>
#include <vector>
#include <random>
#include <cassert>
#include <climits>
#include "../../../modules/task_1/khismatulina_k_column_sum/matrix_sum_column.h"

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

std::vector<int> getSequential(std::vector<int> matrix, int line, int column) {
    std::vector<int> result(line);
    for (int i = 0; i < line; ++i) {
        for (int j = 0; j < column; ++j) {
            result[i] += matrix[i * column + j];
        }
    }
    return result;
}

std::vector<int> getColumnSumParallel(std::vector<int> matrix, int line, int column) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // number of process
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // kolichestvo of process

    int delta = line / size;
    int remainder = line - size * delta;

    // send to slave proccess
    if (rank == 0) {
        if (delta > 0) {
            for (int proc = 1; proc < size; proc++) {
                MPI_Send(&matrix[(proc * delta + remainder) * column], delta * column,
                    MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
        }
    }

    std::vector<int> localVec;
    // receive from master proccess
    if (rank == 0) {
        localVec = matrix;
        localVec.resize((delta + remainder) * column);
    } else {
        if (delta > 0) {
            localVec.resize(delta * column);
            MPI_Status status;
            MPI_Recv(&localVec[0], delta * column, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }

    std::vector<int> verySuperSumOfLine(line);
    int* recvcounts = new int[size] {0};
    int* displs = new int[size] {0};

    for (int i = 0; i < size; ++i) {
        recvcounts[i] = delta;
        if (i == 0) {
            recvcounts[i] += remainder;
            displs[i] = 0;
        } else {
            displs[i] = displs[i - 1] + delta;
            if (i == 1) {
                displs[i] += remainder;
            }
        }
    }

    if (rank == 0) {
        delta += remainder;
    }

    std::vector<int> result(1);
    if (delta > 0 || rank == 0) {
        result = getSequential(localVec, delta, column);
    }
    MPI_Gatherv(&result[0], delta, MPI_INT, &verySuperSumOfLine[0], recvcounts, displs,
        MPI_INT, 0, MPI_COMM_WORLD);
    return verySuperSumOfLine;
}
