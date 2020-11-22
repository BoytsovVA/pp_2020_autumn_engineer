// Copyright 2020 Rachin Igor
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./max_of_clmns_mtrx.h"

TEST(Parallel_Operations_MPI, Test_manual_matrix_seq_max) {
    std::vector<int> test{ 23, 100, 59, 14, 25, 100,
                          100, 29, 49, 100, 38, 15,
                          25, 31, 100, 29, 100, 28 };
    std::vector<int> result{ 100, 100, 100, 100, 100, 100};
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_EQ(getSequentialMaxOfClmns(test, 3, 6), result);
    }
}

TEST(Parallel_Operations_MPI, Test_manual_matrix_parallel_max) {
    std::vector<int> test{ 23, 100, 59, 14, 25, 100,
                           100, 29, 49, 100, 38, 15,
                           25, 31, 100, 29, 100, 28 };
    std::vector<int> result{ 100, 100, 100, 100, 100, 100 };
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> resultParallel = getParallelMaxOfClmns(test, 3, 6);
    if (rank == 0) {
        ASSERT_EQ(resultParallel, result);
    }
}

TEST(Parallel_Operations_MPI, Test_random_matrix_parallel_max) {
    int rank;
    std::vector<int> test;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        test = getRandomMatrix(5, 5);
    }
    std::vector<int> result = getParallelMaxOfClmns(test, 5, 5);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMaxOfClmns(test, 5, 5));
    }
}

TEST(Parallel_Operations_MPI, Test_random_matrix_100x100_parallel_max) {
    std::vector<int> test;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        test = getRandomMatrix(100, 100);
    }
    std::vector<int> result = getParallelMaxOfClmns(test, 100, 100);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMaxOfClmns(test, 100, 100));
    }
    }

TEST(Parallel_Operations_MPI, Test_matrix_negative_size) {
    std::vector<int> test;
    std::vector<int> clear;  // empty vector
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        test = getRandomMatrix(-2, -2);
    }
    if (rank == 0) {
        ASSERT_EQ(test, clear);
    }
}

TEST(Parallel_Operations_MPI, Test_matrix_1000x1000_size) {
    std::vector<int> test;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        test = getRandomMatrix(1000, 1000);
    }
    std::vector<int> result = getParallelMaxOfClmns(test, 1000, 1000);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMaxOfClmns(test, 1000, 1000));
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
