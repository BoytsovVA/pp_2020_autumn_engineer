// Copyright 2020 Slepov Konstantin
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_column_sum.h"


TEST(Task_1, Test_Sequential_On_Predefined_Matrix) {
  int data[] = {3, 3, 3, 3,  2, 2, 2, 2,  1, 1, 1, 1};

  std::vector<int> Result;
  std::vector<int> SeqResult;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    Matrix matrix(3, 4, data);
    Result = std::vector<int>({12, 8, 4});
    SeqResult = SeqColumnSum(matrix);

    EXPECT_EQ(SeqResult, Result);
  }
}

TEST(Task_1, Test_Parallel_On_Predefined_Matrix) {
  int data[] = {3, 3, 3, 3,  2, 2, 2, 2,  1, 1, 1, 1};

  std::vector<int> Result;
  std::vector<int> ParResult;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  Matrix matrix(3, 4, data);
  Result = std::vector<int>({12, 8, 4});

  ParResult = ParColumnSum(matrix);
  if (rank == 0) {
  EXPECT_EQ(ParResult, Result);
  }
}

TEST(Task_1, Test_Sequential_And_Parallel_Sums_Are_The_Same_246x232) {
  Matrix mat = getRandomMatrix(246, 232);
  auto SeqRes = SeqColumnSum(mat);
  auto ParRes = ParColumnSum(mat);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    EXPECT_EQ(SeqRes, ParRes);
  }
}

TEST(Task_1, Test_Sequential_And_Parallel_Sums_Are_The_Same_1024x1) {
  Matrix mat = getRandomMatrix(1024, 1);
  auto SeqRes = SeqColumnSum(mat);
  auto ParRes = ParColumnSum(mat);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    EXPECT_EQ(SeqRes, ParRes);
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
