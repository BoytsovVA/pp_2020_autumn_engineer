// Copyright 2020 Alekhin Denis
#include <stdlib.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "../../modules/task_1/alekhin_d_integration_trapezoidal_method/integration.h"

TEST(Function_Test, MyFunc_Return_0_From_0) {
  EXPECT_EQ(f(0), 0);
}

TEST(Function_Test, MyFunc_Return_50_From_100) {
    EXPECT_DOUBLE_EQ(f(100), 50);
}

TEST(Local_Integration_Test, Local_Integration_Dont_Throws_Exceptions) {
  EXPECT_NO_THROW(localIntegration(f, 1, 2, 3));
}

TEST(Local_Integration_Test, Local_Integration_Return_Right_Value) {
    EXPECT_DOUBLE_EQ(localIntegration(f, 1, 1, 3), 2);
}

TEST(Local_Integration_Test, Local_Integration_Throws_Error_When_N_Less_1) {
  EXPECT_ANY_THROW(localIntegration(f, 0, 1, 3));
}

TEST(Local_Integration_Test, Local_Integration_Throws_Error_When_B_Less_A) {
  EXPECT_ANY_THROW(localIntegration(f, 5, 4, 3));
}

TEST(Integration_Test, Integration_Dont_Throws_Exceptions) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  EXPECT_NO_THROW(integration(f, 2, 1, 3));
}

TEST(Integration_Test, Integration_Return_Right_Value) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  EXPECT_DOUBLE_EQ(integration(f, 10000, 1, 3), 2);
}

TEST(Integration_Test, Integration_Return_Right_Value_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  EXPECT_DOUBLE_EQ(integration(f, 3, 1, 4), 3.75);
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
