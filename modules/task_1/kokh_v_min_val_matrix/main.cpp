// Copyright 2020 Kokh Vladislav
#include <gtest/gtest.h>
#include <vector>
#include <iostream>

#include "./min_val_matrix.h"
#include "../../../3rdparty/gtest-mpi/linux/gtest-mpi-listener.hpp"





TEST(Parallel_Operations_MPI, Test_matrix_6_6) {
    int r = 6;
    int c = 6;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* m = new int[r * c];
    if (rank == 0) {
        m = randomMatrix(m, r, c);
    }
    int min = getParallelOperations(m, r, c);
    if (rank == 0) {
        int easy_min = getSequentialOperations(m, r, c);
        ASSERT_EQ(easy_min, min);
    }
}

TEST(Parallel_Operations_MPI, Test_matrix_4_4) {
    int rank;
    int r = 4;
    int c = 4;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* m = new int[r * c];

    if (rank == 0) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                m[i * c + j] = i * c + j;
            }
        }
    }

    int min = getParallelOperations(m, r, c);

    if (rank == 0) {
        ASSERT_EQ(0, min);
    }
}

TEST(Parallel_Operations_MPI, Test_matrix_23_41_minus) {
    int rank;
    int r = 23;
    int c = 41;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* m = new int[r * c];

    if (rank == 0) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++)
                m[i * c + j] = -(i * c + j);
        }
    }

    int min;
    min = getParallelOperations(m, r, c);

    if (rank == 0) {
        ASSERT_EQ(-942, min);
    }
}

TEST(Parallel_Operations_MPI, Test_matrix_23_41_one_value) {
    int rank;
    int r = 23;
    int c = 41;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* m = new int[r * c];

    if (rank == 0) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++)
                m[i * c + j] = i * c + j;
        }
    }

    m[2 * c + 20] = -1;

    int min;
    min = getParallelOperations(m, r, c);

    if (rank == 0) {
        ASSERT_EQ(-1, min);
    }
}

TEST(Parallel_Operations_MPI, Test_matrix_23_41_plus_many) {
    int rank;
    int r = 23;
    int c = 41;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* m = new int[r * c];

    if (rank == 0) {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++)
                m[i * c + j] = i * c + j + 1234;
        }
    }

    int min;
    min = getParallelOperations(m, r, c);

    if (rank == 0) {
        int easy_min = getSequentialOperations(m, r, c);
        ASSERT_EQ(easy_min, min);
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
