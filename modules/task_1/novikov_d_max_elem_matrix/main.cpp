// Copyright 2020 Novikov Danil
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "./max_elem_matrix.h"




TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    int rows = 30;
    int columns = 30;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* mat = new int[rows * columns];

    if (rank == 0) {
        mat = RandomMatrixValues(mat, rows, columns);
    }

    int global_max;
    global_max = ParallelOperations(mat, rows, columns);

    if (rank == 0) {
        int reference_max = ConsistentOperations(mat, rows, columns);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_48) {
    int rank;
    int rows = 7;
    int columns = 7;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* mat = new int[rows * columns];

    if (rank == 0) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++)
                mat[i * columns + j] = i * columns + j;
        }
    }

    int global_max;
    global_max = ParallelOperations(mat, rows, columns);

    if (rank == 0) {
        ASSERT_EQ(48, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_896) {
    int rank;
    int rows = 23;
    int columns = 39;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* mat = new int[rows * columns];

    if (rank == 0) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++)
                mat[i * columns + j] = i * columns + j;
        }
    }

    int global_max;
    global_max = ParallelOperations(mat, rows, columns);

    if (rank == 0) {
        ASSERT_EQ(896, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_Less_Than_Ziro) {
    int rank;
    int rows = 140;
    int columns = 151;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* mat = new int[rows * columns];

    if (rank == 0) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++)
                mat[i * columns + j] = i * columns + j - 400000;
        }
    }

    int global_max;
    global_max = ParallelOperations(mat, rows, columns);

    if (rank == 0) {
        int reference_max = ConsistentOperations(mat, rows, columns);
        ASSERT_EQ(reference_max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Test_Max_100000) {
    int rank;
    int rows = 111;
    int columns = 102;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* mat = new int[rows * columns];

    if (rank == 0) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++)
                mat[i * columns + j] = i * columns + j;
        }
    }

    mat[32 * columns + 63] = 100000;

    int global_max;
    global_max = ParallelOperations(mat, rows, columns);

    if (rank == 0) {
        ASSERT_EQ(100000, global_max);
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
