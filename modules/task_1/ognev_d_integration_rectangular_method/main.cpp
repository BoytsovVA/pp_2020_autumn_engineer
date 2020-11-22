// Copyright 2020 Ognev Denis

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "../../modules/task_1/ognev_d_integration_rectangular_method/integration.h"

TEST(Sequential_Integration_Test, Throws_Exception_When_Left_Border_Less_Then_Right) {
  ASSERT_ANY_THROW(integrationSequential(function, 3, 2, 100));
}

TEST(Sequential_Integration_Test, Throws_When_Number_Of_Segments_Less_Then_1) {
  ASSERT_ANY_THROW(integrationSequential(function, 2, 3, 0));
}

TEST(Sequential_Integration_Test, Throws_Exception_When_Function_Is_Empty) {
  std::function<double(double)> func;
  ASSERT_ANY_THROW(integrationSequential(func, 2, 3, 100));
}

TEST(Sequential_Integration_Test, Return_Right_Value) {
  double integral = integrationSequential(function, 0, 2, 16);
  ASSERT_DOUBLE_EQ(integral, 2.6640625);
}

TEST(Parallel_Integration_Test, Throws_When_Left_Border_Less_Then_Right) {
  ASSERT_ANY_THROW(integrationParallel(function, 3, 2, 100));
}

TEST(Parallel_Integration_Test, Throws_When_Number_Of_Segemnts_Less_Then_1) {
  ASSERT_ANY_THROW(integrationParallel(function, 2, 3, 0));
}

TEST(Parallel_Integration_Test, Throws_Exception_When_Function_Is_Empty) {
  std::function<double(double)> func;
  ASSERT_ANY_THROW(integrationParallel(func, 2, 3, 100));
}

TEST(Parallel_Integration_Test, Return_Right_Value) {
  double integral = integrationParallel(function, 0, 2, 16);
  ASSERT_DOUBLE_EQ(integral, 2.6640625);
}

TEST(Compare_Test, Parralel_Equals_Sequential) {
  double sequential_integral = integrationSequential(function, 0, 3, 10001);
  double parallel_integral = integrationParallel(function, 0, 3, 10001);
  ASSERT_DOUBLE_EQ(sequential_integral, parallel_integral);
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
