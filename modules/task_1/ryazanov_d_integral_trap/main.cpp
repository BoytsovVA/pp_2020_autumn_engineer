// Copyright 2020 Dmitriy Ryazanov
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/ryazanov_d_integral_trap/integral_trap.h"

TEST(Local_Integration_Test, Local_Integration_Throws) {
  EXPECT_EQ(f(6), 1);
}

TEST(Parallel_Operations_MPI, Compare_Local_Integral_And_Global_Integral) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double global_result = integration(f, 1, 2, 3);
  if (rank == 0) {
    double local_result = integral(f, 1, 2, 3);
    ASSERT_LT((global_result - local_result), 2.84722);
  }
}

TEST(Local_Integration_Test, Local_Integration_Throws_Error_When_N_Less_1) {
  EXPECT_ANY_THROW(integral(f, 0, 1, 3));
}

TEST(Local_Integration_Test, Local_Integration_Throws_Error_When_B_Less_A) {
  EXPECT_ANY_THROW(integral(f, 5, 4, 3));
}

TEST(Function_Test, f_Return_0_From_7) { EXPECT_EQ(f(0), 7); }


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
