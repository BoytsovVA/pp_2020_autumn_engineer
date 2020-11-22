// Copyright 2018 Nesterov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./min_val_mat_columns.h"

TEST(Parallel_Operations_MPI, TEST_Check_Array_Square_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int columns = 4;
    int size = rows * columns;
    int* res_array, * check_array, * array;
    array = new int[size]{3, 8, 10, 15, 2, 6, 1, 3, 7, 4, 9, 5, 7, 0, 2, 6};
    res_array = new int[columns]{2, 0, 1, 3};
    check_array = new int[columns];
    if (rank == 0) {
       getCheckArray(array, check_array, rows, columns);
       for (int i = 0; i < columns; ++i)
           EXPECT_EQ(check_array[i], res_array[i]) << "Arrays check_array and res_array differ at index " << i;
    }
}


TEST(Parallel_Operations_MPI, TEST_Check_Array_Of_Not_Square_Matrix1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 2;
    int columns = 3;
    int size = rows * columns;
    int* res_array, * check_array, * array;
    array = new int[size]{4, 5, 2, 9, 4, 3};
    res_array = new int[columns]{4, 4, 2};
    check_array = new int[columns];
    if (rank == 0) {
       getCheckArray(array, check_array, rows, columns);
       for (int i = 0; i < columns; ++i)
           EXPECT_EQ(check_array[i], res_array[i]) << "Arrays check_array and res_array differ at index " << i;
    }
}

TEST(Parallel_Operations_MPI, TEST_Check_Array_Of_Not_Square_Matrix2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int columns = 2;
    int size = rows * columns;
    int* res_array, * check_array, * array;
    array = new int[size]{4, 5, 2, 9, 4, 3};
    res_array = new int[columns]{2, 3};
    check_array = new int[columns];
    if (rank == 0) {
       getCheckArray(array, check_array, rows, columns);
       for (int i = 0; i < columns; ++i)
           EXPECT_EQ(check_array[i], res_array[i]) << "Arrays check_array and res_array differ at index " << i;
    }
}

TEST(Parallel_Operations_MPI, Test_Square_Matrix_Delta_Divisible) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int columns = 4;
    int size = rows * columns;
    int** matrix;
    int* matrix_array, * check_array, * parallel_array;
    matrix_array = new int[size];
    check_array = new int[columns];
    parallel_array = new int[columns];
    matrix = new int* [rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[columns];
    if (rank == 0) {
        getRandomMatrix(matrix, rows, columns);
        matrixToArray(matrix, matrix_array, rows, columns);
    }
    getParallelMin(matrix_array, parallel_array, rows, columns);
    if (rank == 0) {
        getCheckArray(matrix_array, check_array, rows, columns);
    for (int i = 0; i < columns; i++)
        EXPECT_EQ(check_array[i], parallel_array[i]) << "Arrays check_array and parallel_array differ at index " << i;
    }
}

TEST(Parallel_Operations_MPI, Test_Square_Matrix_Delta_Not_Divisible) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 5;
    int columns = 5;
    int size = rows * columns;
    int** matrix;
    int* matrix_array, * check_array, * parallel_array;
    matrix_array = new int[size];
    check_array = new int[columns];
    parallel_array = new int[columns];
    matrix = new int* [rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[columns];
    if (rank == 0) {
        getRandomMatrix(matrix, rows, columns);
        matrixToArray(matrix, matrix_array, rows, columns);
    }
    getParallelMin(matrix_array, parallel_array, rows, columns);
    if (rank == 0) {
        getCheckArray(matrix_array, check_array, rows, columns);
    for (int i = 0; i < columns; i++)
        EXPECT_EQ(check_array[i], parallel_array[i]) << "Arrays check_array and parallel_array differ at index " << i;
    }
}

TEST(Parallel_Operations_MPI, Test_Not_Square_Matrix_Delta_Divisible) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 100;
    int columns = 101;
    int size = rows * columns;
    int** matrix;
    int* matrix_array, * check_array, * parallel_array;
    matrix_array = new int[size];
    check_array = new int[columns];
    parallel_array = new int[columns];
    matrix = new int* [rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[columns];
    if (rank == 0) {
        getRandomMatrix(matrix, rows, columns);
        matrixToArray(matrix, matrix_array, rows, columns);
    }
    getParallelMin(matrix_array, parallel_array, rows, columns);
    if (rank == 0) {
        getCheckArray(matrix_array, check_array, rows, columns);
    for (int i = 0; i < columns; i++)
        EXPECT_EQ(check_array[i], parallel_array[i]) << "Arrays check_array and parallel_array differ at index " << i;
    }
}

TEST(Parallel_Operations_MPI, Test_Not_Square_Matrix_Delta_Not_Divisible) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int columns = 7;
    int size = rows * columns;
    int** matrix;
    int* matrix_array, * check_array, * parallel_array;
    matrix_array = new int[size];
    check_array = new int[columns];
    parallel_array = new int[columns];
    matrix = new int* [rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[columns];
    if (rank == 0) {
        getRandomMatrix(matrix, rows, columns);
        matrixToArray(matrix, matrix_array, rows, columns);
    }
    getParallelMin(matrix_array, parallel_array, rows, columns);
    if (rank == 0) {
        getCheckArray(matrix_array, check_array, rows, columns);
    for (int i = 0; i < columns; i++)
        EXPECT_EQ(check_array[i], parallel_array[i]) << "Arrays check_array and parallel_array differ at index " << i;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
