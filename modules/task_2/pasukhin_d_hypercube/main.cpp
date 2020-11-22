//  Copyright 2020 Pasukhin Dmitry

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <limits>
#include "./hypercube.h"

TEST(Parallel_Operations_MPI, Test_Forward_Sending) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sender = randPath(0, size / 2 - 1);
  int reciver = randPath((size / 2), size - 1);

  std::vector<double> vec{ 5.5 };
  huperCubeTopology(vec, reciver, sender);
  double* coll = new double[vec.size()];
  hyperTopoRecv(reciver, vec.size(), coll);
  if (rank == reciver) {
    ASSERT_DOUBLE_EQ(vec[0], coll[0]);
  }
}

TEST(Parallel_Operations_MPI, Test_Reverse_Many_Data_Sending) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sender = randPath((size / 2), size - 1);
  int reciver = randPath(0, size / 2 - 1);

  std::vector<double> vec{ 5.5, 4.4, 7.7, 8.8 };
  huperCubeTopology(vec, reciver, sender);
  double* coll = new double[vec.size()];
  hyperTopoRecv(reciver, vec.size(), coll);
  if (rank == reciver) {
    bool isOK = true;
    for (int i = 0; i < static_cast<int>(vec.size()) && isOK; ++i) {
      if (fabs(vec[i] - coll[i]) >= std::numeric_limits<double>::epsilon())
        isOK = false;
    }
    ASSERT_EQ(isOK, true);
  }
}

TEST(Parallel_Operations_MPI, Test_Forward_One_Rand_Data_Sending) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sender = randPath(0, size / 2 - 1);
  int reciver = randPath((size / 2), size - 1);

  double data = randPath(0, 200);
  std::vector<double> vec{ data };
  huperCubeTopology(vec, reciver, sender);
  double* coll = new double[vec.size()];
  hyperTopoRecv(reciver, vec.size(), coll);
  if (rank == reciver) {
    ASSERT_DOUBLE_EQ(vec[0], coll[0]);
  }
}

TEST(Parallel_Operations_MPI, Test_Reverse_Sending) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sender = randPath((size / 2), size - 1);
  int reciver = randPath(0, size / 2 - 1);

  std::vector<double> vec{ 5.5 };
  huperCubeTopology(vec, reciver, sender);
  double* coll = new double[vec.size()];
  hyperTopoRecv(reciver, vec.size(), coll);
  if (rank == reciver) {
    ASSERT_DOUBLE_EQ(vec[0], coll[0]);
  }
}


TEST(Parallel_Operations_MPI, Test_Forward_Many_Data_Sending) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int sender = randPath(0, size / 2 - 1);
  int reciver = randPath((size / 2), size - 1);

  std::vector<double> vec{ 5.5, 4.4, 7.7, 8.8 };
  huperCubeTopology(vec, reciver, sender);
  double* coll = new double[vec.size()];
  hyperTopoRecv(reciver, vec.size(), coll);
  if (rank == reciver) {
    bool isOK = true;
    for (int i = 0; i < static_cast<int>(vec.size()) && isOK; ++i) {
      if (fabs(vec[i] - coll[i]) >= std::numeric_limits<double>::epsilon())
        isOK = false;
    }
    ASSERT_EQ(isOK, true);
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
