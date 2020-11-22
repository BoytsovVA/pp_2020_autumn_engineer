  // Copyright 2020 Maksimov Andrey

#include <mpi.h>
#include <time.h>
#include <random>
#include <vector>

#include "../../modules/task_2/maksimov_a_matr_vec_mult/matr_vec_mult.h"

#define MAX_GEN 100

std::vector<int> getRandomVector(int size) {
    if (size < 1) {
        throw "size < 1 error";
    }

    std::mt19937 gen;
    gen.seed(time(0));

    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % MAX_GEN;
    }
    return vec;
}

std::vector<int> getRandomMatrix(int rows, int columns) {
    if (rows < 1 || columns < 1) {
        throw "rows or columns < 1 error";
    }

    std::mt19937 gen;
    gen.seed(time(0));

    std::vector<int> matr(rows * columns);
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            matr[row * columns + column] = gen() % MAX_GEN;
        }
    }
    return matr;
}

std::vector<int> multiplyMatrixByVectorNotParall(
    std::vector<int> matr, int rows, int columns,
    std::vector<int> vec, int vecSize) {

    if (columns != vecSize)
        throw "columns != vecSize error";

    std::vector<int> result(rows);
    int sum = 0;
    for (int row = 0; row < rows; row++) {
        sum = 0;
        for (int column = 0; column < columns; column++) {
            sum += matr[row * columns + column] * vec[column];
        }
        result[row] = sum;
    }

    return result;
}

std::vector<int> multiplyMatrixByVector(
    std::vector<int> matr, int rows, int columns,
    std::vector<int> vec, int vecSize) {

    if (columns != vecSize)
        throw "columns != vecSize error";

    MPI_Status mpiStatus;
    int procNum, procRank, sum, rowsPart, size;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    rowsPart = rows / procNum;
    size = rowsPart * columns;

    if (procRank != 0) {
        vec = std::vector<int>(columns);
        matr = std::vector<int>(size);
    }

    MPI_Bcast(vec.data(), columns, MPI_INT, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        for (int currProc = 1; currProc < procNum; currProc++) {
            MPI_Send(matr.data() + currProc * size, size, MPI_INT, currProc, 0, MPI_COMM_WORLD);
        }
    }

    if (procRank != 0) {
        MPI_Recv(matr.data(), size, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpiStatus);
    }

    std::vector<int> returnVec(rows);

    std::vector<int> resultVec(rowsPart);
    if (procRank == 0) {
        for (int row = 0; row < rowsPart; row++) {
            sum = 0;
            for (int column = 0; column < columns; column++) {
                sum += matr[row * columns + column] * vec[column];
            }
            returnVec[row] = sum;
        }
    } else {
        for (int row = 0; row < rowsPart; row++) {
            sum = 0;
            for (int column = 0; column < columns; column++) {
                sum += matr[row * columns + column] * vec[column];
            }
            resultVec[row] = sum;
        }
    }

    if (procRank != 0) {
        MPI_Send(resultVec.data(), rowsPart, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (procRank == 0) {
        for (int currProc = 1; currProc < procNum; currProc++) {
            MPI_Recv(returnVec.data() + currProc * rowsPart, rowsPart, MPI_INT, currProc,
                0, MPI_COMM_WORLD, &mpiStatus);
        }

        for (int i = 0; i < rows % procNum; i++) {
            sum = 0;
            for (int j = 0; j < columns; j++) {
                sum += matr[(rowsPart * procNum + i) * columns + j] * vec[j];
            }
            returnVec[rowsPart * procNum + i] = sum;
        }
    }

    return returnVec;
}
