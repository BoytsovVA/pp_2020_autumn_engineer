// Copyright 2020 Nasedkin Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./vector_min.h"
// mpiexec -n 2 "C:\Labs\MPI2020\pp_2020_autumn_engineer\build\bin\nasedkin_a_vector_min_val_mpi.exe"
TEST(Parallel_Operations_MPI, Can_Make_Random_Vector) {
  ASSERT_NO_THROW(rndVector(100));
}

TEST(Parallel_Operations_MPI, Cant_Make_Incorrect_Random_Vector) {
    ASSERT_ANY_THROW(rndVector(-100));
}

TEST(Parallel_Operations_MPI, Can_Make_Sequential_Operation_Correct) {
    std::vector<int> vec{ 5, 68, 4, 9, 9999 };
    ASSERT_NO_THROW(SequentialMininum(vec));
}

TEST(Parallel_Operations_MPI, Is_Sequential_Operation_Correct) {
  std::vector<int> vec{5, 68, 4, 9, 9999};
  ASSERT_EQ(4, SequentialMininum(vec));
}

TEST(Parallel_Operations_MPI, Is_Parallel_Operation_Correct) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  std::vector<int> vec;
  int vec_size = 100;
  int sequential_min;
  if (procRank == 0) {
    vec = rndVector(vec_size);
    sequential_min = SequentialMininum(vec);
  }
  int parallel_min = ParallelMinimum(vec, vec_size);
  if (procRank == 0) {
    ASSERT_EQ(sequential_min, parallel_min);
  }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
