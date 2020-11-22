// Copyright 2020 Chesnokov Artyom
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_column_sum.h"

TEST(Task_1, Test_Random_Matrix_No_Throw) {
    EXPECT_NO_THROW(Matrix mat = getRandomMatrix(10, 10));
}

TEST(Task_1, Test_Sequential_On_Predefined_Matrix) {
  Matrix matrix(3);
  std::vector<int> res;
  std::vector<int> seq_res;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    matrix = Matrix({{2, 4, 2, 1}, {1, 4, 1, 0}, {2, 3, 2, 3}});
    res = std::vector<int>({9, 6, 10});

    seq_res = getSequentialColumnSum(matrix);

    EXPECT_EQ(seq_res, res);
  }
}

TEST(Task_1, Test_Parallel_On_Predefined_Matrix) {
  Matrix matrix(3);
  std::vector<int> res;
  std::vector<int> par_res;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    matrix = Matrix({{2, 4, 2, 1}, {1, 4, 1, 0}, {2, 3, 2, 3}});
    res = std::vector<int>({9, 6, 10});

    par_res = getParallelColumnSum(matrix);
    if (rank == 0) {
    EXPECT_EQ(par_res, res);
  }
}

TEST(Task_1, Test_Sequential_And_Parallel_Sums_Are_The_Same_11x11) {
  Matrix mat = getRandomMatrix(11, 11);
  auto seq_res = getSequentialColumnSum(mat);
  auto par_res = getParallelColumnSum(mat);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    EXPECT_EQ(seq_res, par_res);
  }
}

TEST(Task_1, Test_Sequential_And_Parallel_Sums_Are_The_Same_1x10) {
  Matrix mat = getRandomMatrix(1, 10);
  auto seq_res = getSequentialColumnSum(mat);
  auto par_res = getParallelColumnSum(mat);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    EXPECT_EQ(seq_res, par_res);
  }
}

TEST(Task_1, Test_Sequential_And_Parallel_Sums_Are_The_Same_9x1) {
  Matrix mat = getRandomMatrix(9, 1);
  auto seq_res = getSequentialColumnSum(mat);
  auto par_res = getParallelColumnSum(mat);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    EXPECT_EQ(seq_res, par_res);
  }
}

TEST(Task_1, Test_Sequential_And_Parallel_Sums_Are_The_Same_23x32) {
  Matrix mat = getRandomMatrix(23, 32);
  auto seq_res = getSequentialColumnSum(mat);
  auto par_res = getParallelColumnSum(mat);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    EXPECT_EQ(seq_res, par_res);
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
