// Copyright 2020 Ekaterina Burmistrova
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "./mpi_fun.h"

TEST(Parallel_Operations_MPI, Test_Sum) {
    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    const int rows = 100;
    const int columns = 100;
    std::string oper = "+";
    std::vector<int> matrix(rows*columns);
    if (mynode == 0) {
        matrix = getRandomMatrix(rows, columns);
    }

    int par_sum = Parallel_method(matrix, rows, columns, oper);

    if (mynode == 0) {
        int ref_sum = 0;
        ref_sum = Sequential_method(matrix, oper);
        ASSERT_EQ(ref_sum, par_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Lower_Bound) {
    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    const int rows = 56;
    const int columns = 33;
    std::string oper = "min";
    std::vector<int> matrix(rows*columns);
    if (mynode == 0) {
         matrix = getRandomMatrix(rows, columns);
    }

    int par_sum = Parallel_method(matrix, rows, columns, "+");

    if (mynode == 0) {
        int ref_min_sum = 0;
        ref_min_sum = rows*columns * Sequential_method(matrix, oper);
        ASSERT_LE(ref_min_sum, par_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Upper_Bound) {
    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    const int rows = 73;
    const int columns = 20;
    std::string oper = "max";
    std::vector<int> matrix(rows*columns);
    if (mynode == 0) {
        matrix = getRandomMatrix(rows, columns);
    }

    int par_sum = Parallel_method(matrix, rows, columns, "+");

    if (mynode == 0) {
        int ref_max_sum = 0;
        ref_max_sum = rows * columns * Sequential_method(matrix, oper);
        ASSERT_GE(ref_max_sum, par_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Diff_Size_F) {
    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    const int rows = 100000;
    const int columns = 5;
    std::string oper = "+";
    std::vector<int> matrix(rows*columns);
    if (mynode == 0) {
        matrix = getRandomMatrix(rows, columns);
    }

    int par_sum = Parallel_method(matrix, rows, columns, oper);

    if (mynode == 0) {
        int ref_sum = 0;
        ref_sum = Sequential_method(matrix, oper);
        ASSERT_EQ(ref_sum, par_sum);
    }
}
TEST(Parallel_Operations_MPI, Test_Upper_Bound_P_S) {
    int mynode;
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    const int rows = 10000;
    const int columns = 500;
    std::string oper = "max";
    std::vector<int> matrix(rows*columns);
    if (mynode == 0) {
        matrix = getRandomMatrix(rows, columns);
    }

    int par_max_sum = Parallel_method(matrix, rows, columns, oper);

    if (mynode == 0) {
        int ref_max_sum = 0;
        ref_max_sum = Sequential_method(matrix, oper);
        ASSERT_EQ(ref_max_sum, par_max_sum);
    }
}
int main(int argc, char **argv) {
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
