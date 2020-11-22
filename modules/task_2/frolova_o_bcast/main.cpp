// Copyright 2018 Nesterov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./bcast.h"

TEST(Parallel_Operations_MPI, Test_Negative_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  const int size = -1;
  if (rank == 0) {
  ASSERT_ANY_THROW(getRandomArray(size));
  ASSERT_ANY_THROW(getRandomDoubleArray(size));
  ASSERT_ANY_THROW(getRandomFloatArray(size));
  }
}

TEST(Parallel_Operations_MPI, Test_Get_Random_Array) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 100;
  if (rank == 0) {
  ASSERT_NO_THROW(getRandomArray(size));
  ASSERT_NO_THROW(getRandomDoubleArray(size));
  ASSERT_NO_THROW(getRandomFloatArray(size));
  }
}

TEST(Parallel_Operations_MPI, TEST_Check_checkIntSum) {
    int rank, sum;
    sum = -666;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array{4, 5, 2, 9, 4, 3};
    if (rank == 0) {
       sum = checkSum(array, 6);
       EXPECT_EQ(sum, 27);
    }
}

TEST(Parallel_Operations_MPI, TEST_Check_checkDoubleSum) {
    int rank;
    double sum = -666.0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> array{4.0, 5.0, 2.0, 9.0, 4.0, 3.5};
    if (rank == 0) {
       sum = checkDoubleSum(array, 6);
       EXPECT_DOUBLE_EQ(sum, 27.5);
    }
}

TEST(Parallel_Operations_MPI, TEST_Check_checkFloatSum) {
    int rank;
    float sum = -666.0f;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<float> array{4.0f, 5.0f, 2.0f, 9.0f, 4.0f, 3.5f};
    if (rank == 0) {
       sum = checkFloatSum(array, 6);
       EXPECT_FLOAT_EQ(sum, 27.5f);
    }
}

TEST(Parallel_Operations_MPI, TEST_Get_Int_Sum) {
    int rank, sum1, sum2;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    if (rank == root) array = getRandomArray(5);
    MPI_Barrier(MPI_COMM_WORLD);
    getSum(array, 5, &sum1, root);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
       sum2 = checkSum(array, 5);
       EXPECT_EQ(sum1, sum2);
    }
}

TEST(Parallel_Operations_MPI, TEST_Get_Float_Sum) {
    int rank;
    float sum1, sum2;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<float> array;
    if (rank == root) array = getRandomFloatArray(7);
    MPI_Barrier(MPI_COMM_WORLD);
    getFloatSum(array, 7, &sum1, root);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
       sum2 = checkFloatSum(array, 7);
       EXPECT_FLOAT_EQ(sum1, sum2);
    }
}

TEST(Parallel_Operations_MPI, TEST_Get_Double_Sum) {
    int rank;
    double sum1, sum2;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> array;
    if (rank == root) array = getRandomDoubleArray(9);
    MPI_Barrier(MPI_COMM_WORLD);
    getDoubleSum(array, 9, &sum1, root);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
       sum2 = checkDoubleSum(array, 9);
       EXPECT_DOUBLE_EQ(sum1, sum2);
    }
}

TEST(Parallel_Operations_MPI, TEST_Get_Sum2) {
    int rank, sum1, sum2;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    if (rank == root) array = getRandomArray(55);
    MPI_Barrier(MPI_COMM_WORLD);
    getSum(array, 55, &sum1, root);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
       sum2 = checkSum(array, 55);
       EXPECT_EQ(sum1, sum2);
    }
}

TEST(Parallel_Operations_MPI, TEST_Get_Sum3) {
    int rank, sum1, sum2;
    int root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    if (rank == root) array = getRandomArray(50);
    MPI_Barrier(MPI_COMM_WORLD);
    getSum(array, 50, &sum1, root);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
       sum2 = checkSum(array, 50);
       EXPECT_EQ(sum1, sum2);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    difOfTime(10, 0);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
