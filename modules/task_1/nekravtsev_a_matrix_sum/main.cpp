// Copyright 2020 Alexey Nekravtsev
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../modules/task_1/nekravtsev_a_matrix_sum/matrix_sum.h"


TEST(Parallel_Operations_MPI, Test_10x10_matrix) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  std::vector<int> matrix = setRandomMatrix(10, 10);

  int seq_result = SequentialCalc(matrix, 100);
  int par_result = ParallelCalc(matrix, 100);

  if (procRank == 0) {
    EXPECT_EQ(seq_result, par_result) << "Parallel and sequential functions give different results";
  }
}

TEST(Parallel_Operations_MPI, Test_20114x777_matrix) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  std::vector<int> matrix = setRandomMatrix(20114, 777);

  int seq_result = SequentialCalc(matrix, matrix.size());
  int par_result = ParallelCalc(matrix, matrix.size());
  if (procRank == 0) {
    EXPECT_EQ(seq_result, par_result) << "Parallel and sequential functions give different results";
  }
}

TEST(Parallel_Operations_MPI, Test_100x2_matrix) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  std::vector<int> matrix = setRandomMatrix(100, 2);

  int seq_result = SequentialCalc(matrix, 200);
  int par_result = ParallelCalc(matrix, 200);

  if (procRank == 0) {
    EXPECT_EQ(seq_result, par_result) << "Parallel and sequential functions give different results";
  }
}

TEST(Parallel_Operations_MPI, Test_2x100_matrix) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  std::vector<int> matrix = setRandomMatrix(2, 100);

  int seq_result = SequentialCalc(matrix, 200);
  int par_result = ParallelCalc(matrix, 200);

  if (procRank == 0) {
    EXPECT_EQ(seq_result, par_result) << "Parallel and sequential functions give different results";
  }
}

TEST(Parallel_Operations_MPI, Right_Calculate_80) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  std::vector<int> matrix(16);
  for (int i = 0; i < 16; i++) {
    matrix[i] = 5;
  }

  int par_result = ParallelCalc(matrix, 16);

  if (procRank == 0) {
    EXPECT_EQ(par_result, 80) << "Wrong result";
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
