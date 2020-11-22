// Copyright 2020 Alexey Nekravtsev
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "../../modules/task_2/nekravtsev_a_star/star.h"


TEST(Parallel_Operations_MPI, Test_Topology) {
  int procRank;
  MPI_Comm result;
  int status;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  createStar(MPI_COMM_WORLD, &result);

  MPI_Topo_test(result, &status);

  EXPECT_EQ(MPI_GRAPH, status) << "Wrong result";
}

TEST(Parallel_Operations_MPI, Test_Neighbours_Count) {
  int procRank, procNum;
  MPI_Comm result;
  int status;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  createStar(MPI_COMM_WORLD, &result);

  MPI_Graph_neighbors_count(result, 0, &status);

  EXPECT_EQ(procNum - 1, status) << "Wrong result";
}

TEST(Parallel_Operations_MPI, Test_Neighbours_Set) {
  int procRank, procNum;
  MPI_Comm result;
  int status = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  createStar(MPI_COMM_WORLD, &result);

  if (procRank == 1) {
    int t = 0;
    status = 0;
    for (int i = 1; i < procNum; i++) {
      MPI_Graph_neighbors_count(result, i, &t);
      status += t;
    }

    if (procRank == 1) {
      EXPECT_EQ(procNum - 1, status) << "Wrong result";
    }
  }
}

TEST(Parallel_Operations_MPI, Test_starSend_Transit) {
  int procRank, procNum;
  MPI_Comm result;
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int in[3] = { 1, 2, 3 }, out[3] = { 3, 2, 1 };
  createStar(MPI_COMM_WORLD, &result);

  if (procNum == 1) {
    EXPECT_ANY_THROW(starSend(&in, 3, MPI_INT, procNum - 1, 1, 1, result););
  } else {
    starSend(&in, 3, MPI_INT, procNum - 1, 1, 1, result);
    starRecv(&out, 3, MPI_INT, procNum - 1, 1, 1, result, &status);
  }

  if (procRank == 1 && procNum != 1) {
    EXPECT_EQ(out[0], in[0]) << "Wrong result";
  }
}

TEST(Parallel_Operations_MPI, Test_starSend_To_Center) {
  int procRank, procNum;
  MPI_Comm result;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int in = 3, out = 5;
  createStar(MPI_COMM_WORLD, &result);

  if (procNum == 1) {
    EXPECT_ANY_THROW(starSend(&in, 1, MPI_INT, procNum - 1, 0, 1, result));
  } else {
    starSend(&in, 1, MPI_INT, procNum - 1, 0, 1, result);
    starRecv(&out, 1, MPI_INT, procNum - 1, 0, 1, result, &status);
  }

  if (procRank == 0 && procNum != 1) {
    EXPECT_EQ(out, in) << "Wrong result";
  }
}

TEST(Parallel_Operations_MPI, Test_starSend_From_Center) {
  int procRank, procNum;
  MPI_Comm result;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int in = 3, out = 5;
  createStar(MPI_COMM_WORLD, &result);

  if (procNum == 1) {
    EXPECT_ANY_THROW(starSend(&in, 1, MPI_INT, 0, procNum - 1, 1, result););
  } else {
    starSend(&in, 1, MPI_INT, 0, procNum - 1, 1, result);
    starRecv(&out, 1, MPI_INT, 0, procNum - 1, 1, result, &status);
  }

  if (procRank == procNum - 1 && procNum != 1) {
    EXPECT_EQ(out, in) << "Wrong result";
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
