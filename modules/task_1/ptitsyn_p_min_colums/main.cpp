// Copyright 2020 Ptitsyn Pavel
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./min_colums.h"

TEST(min_colums, np_vec3x3) {
  int line = 3;
  int column = 3;
  int rank;
  std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> res{1, 2, 3};
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::vector<int> gvec = getSequentialOperations(vec, line, column);
    ASSERT_EQ(gvec, res);
  }
}

TEST(min_colums, np_vec1x3) {
  int line = 1;
  int column = 3;
  int rank;
  std::vector<int> vec{1, 2, 3};
  std::vector<int> res{1, 2, 3};
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::vector<int> gvec = getSequentialOperations(vec, line, column);
    ASSERT_EQ(res, gvec);
  }
}

TEST(min_colums, np_vec3x1) {
  int line = 3;
  int column = 1;
  int rank;
  std::vector<int> vec{1, 2, 3};
  std::vector<int> res{1};
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::vector<int> gvec = getSequentialOperations(vec, line, column);
    ASSERT_EQ(res, gvec);
  }
}

TEST(min_colums, p_vec1x51) {
  int line = 1;
  int column = 51;
  int rank;
  std::vector<int> vec;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = getRandomVector(line, column);
  }
  std::vector<int> gvec = getParallelOperations(vec, line, column);
  if (rank == 0) {
    std::vector<int> res = getSequentialOperations(vec, line, column);
    ASSERT_EQ(gvec, res);
  }
}

TEST(min_colums, p_vec51x1) {
  int line = 51;
  int column = 1;
  int rank;
  std::vector<int> vec;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = getRandomVector(line, column);
  }
  std::vector<int> gvec = getParallelOperations(vec, line, column);
  if (rank == 0) {
    std::vector<int> res = getSequentialOperations(vec, line, column);
    ASSERT_EQ(gvec, res);
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
