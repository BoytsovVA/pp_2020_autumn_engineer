// Copyright 2020 Krasilnikov Alexey

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include "../../../modules/task_1/krasilnikov_a_count_sentences/count_sentences.h"

void CustomTest(const size_t test_size);

TEST(Parallel_Operations_MPI, Test_small_size) {
  CustomTest(10);
}

TEST(Parallel_Operations_MPI, Test_middle_size) {
  CustomTest(100);
}

TEST(Parallel_Operations_MPI, Test_big_size) {
  CustomTest(1000);
}

TEST(Parallel_Operations_MPI, Test_empty) {
  CustomTest(0);
}

TEST(Parallel_Operations_MPI, Test_single) {
  CustomTest(1);
}

void CustomTest(const size_t str_size) {
  int comm_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
  std::string str;
  if (comm_rank == 0) {
    str = getRandomString(str_size);
  }
  const auto parallel_ans = getCountSentencesParallel(str, str_size);
  if (comm_rank == 0) {
    const auto sequential_ans = getCountSentencesSequential(str);
    ASSERT_EQ(parallel_ans, sequential_ans);
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
