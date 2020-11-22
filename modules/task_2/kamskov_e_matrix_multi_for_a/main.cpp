// Copyright 2020 Kamskov Eugene
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_multi_for_a.h"

TEST(Parallel_Operations_MPI, Test_2_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, second_global_matrix;
    const int rows = 100;
    const int columns = 100;

    if (rank == 0) {
        global_matrix = getRandomMatrix(rows, columns);
        second_global_matrix = getRandomMatrix(rows, columns);
    }

    std::vector<int> global_sum = getParallelOperations(global_matrix, second_global_matrix, rows);

    if (rank == 0) {
       std::vector<int> reference_sum = getRes(global_matrix, second_global_matrix, rows);
       ASSERT_EQ(global_sum, reference_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, second_global_matrix;
    const int rows = 7;
    const int columns = 7;

    if (rank == 0) {
        global_matrix = getRandomMatrix(rows, columns);
        second_global_matrix = getRandomMatrix(rows, columns);
    }

    std::vector<int> global_sum = getParallelOperations(global_matrix, second_global_matrix, rows);

    if (rank == 0) {
       std::vector<int> reference_sum = getRes(global_matrix, second_global_matrix, rows);
       ASSERT_EQ(global_sum, reference_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Can_Create_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    const int count_size_matrix = 10;

    if (rank == 0) {
        ASSERT_NO_THROW(global_matrix = getRandomMatrix(count_size_matrix, count_size_matrix));
    }
}

TEST(Parallel_Operations_MPI, Test_Can_Get_Parallel_Operations) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, second_global_matrix;
    const int rows = 10;
    const int columns = 10;

    if (rank == 0) {
        global_matrix = getRandomMatrix(rows, columns);
        second_global_matrix = getRandomMatrix(rows, columns);
    }

    ASSERT_NO_THROW(getParallelOperations(global_matrix, second_global_matrix, rows));
}

TEST(Parallel_Operations_MPI, Test_Can_Get_Sequential_Operations) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix, second_global_matrix;
    const int rows = 10;
    const int columns = 10;

    if (rank == 0) {
        global_matrix = getRandomMatrix(rows, columns);
        second_global_matrix = getRandomMatrix(rows, columns);
         ASSERT_NO_THROW(getRes(global_matrix, second_global_matrix, rows));
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
