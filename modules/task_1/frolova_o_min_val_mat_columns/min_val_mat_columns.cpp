// Copyright 2018 Nesterov Alexander
#include "../../../modules/task_1/frolova_o_min_val_mat_columns/min_val_mat_columns.h"
#include <mpi.h>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

void getRandomMatrix(int** matrix, int rows, int columns) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)
            matrix[i][j] = gen() % 100;
        }
}

void matrixToArray(int** matrix, int* matrix_array, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)
            matrix_array[i * columns + j] = matrix[i][j];
        }
}

void getParallelMin(int* matrix_array, int* parallel_array, int rows, int columns) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int min = 2000000000;
    int delta = columns / size;
    if (columns % size != 0)
       delta++;
    MPI_Bcast(matrix_array, rows * columns, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = delta * rank; i < delta * (rank + 1); i++) {
         if (i > columns) break;
         min = matrix_array[i];
         for (int j = 0; j < rows; j++) {
             if (matrix_array[i + j * columns] <= min) {
                min = matrix_array[i + j * columns];
                }
             }
             if (rank == 0) {
                parallel_array[i] = min;
             } else {
                  MPI_Send(&min, 1, MPI_INT, 0, 15, MPI_COMM_WORLD);
             }
    }
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < delta; j++) {
                if (i * delta + j > columns) break;
                MPI_Status status;
                MPI_Recv(&min, 1, MPI_INT, i, 15, MPI_COMM_WORLD, &status);
                parallel_array[i * delta + j] = min;
            }
        }
    }
}

void getCheckArray(int* array, int* min_array, int rows, int columns) {
    for (int i = 0; i < columns; i++) {
        min_array[i] = array[i];
        for (int j = 0; j < rows; j++)
            if (array[i + j * columns] <= min_array[i])
               min_array[i] = array[i + j * columns];
    }
}
