// Copyright 2020 Oganyan Robert

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include "../../../modules/task_1/oganyan_r_letters_amount/letters_amount.h"

void CustomTest(const int& test_size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string global_str;
  if (rank == 0) {
    global_str = getRandomString(test_size);
  }
  auto paral_ans = getParallelOperations(global_str, test_size);
  if (rank == 0) {
    auto seq_ans = getSequentialOperations(global_str);
    ASSERT_EQ(seq_ans, paral_ans);
  }
}

TEST(Parallel_Operations_MPI, Test_tiny) {
  CustomTest(1);
}

TEST(Parallel_Operations_MPI, Test_small) {
  CustomTest(5);
}

TEST(Parallel_Operations_MPI, Test_middle) {
  CustomTest(15);
}

TEST(Parallel_Operations_MPI, Test_large) {
  CustomTest(100);
}

TEST(Parallel_Operations_MPI, Test_huge) {
  CustomTest(1000);
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
