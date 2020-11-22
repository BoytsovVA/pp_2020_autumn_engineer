// Copyright 2020 Anisimova Polina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <array>
#include "./anisimova_p_philosopher.h"


TEST(Dining_Philosophers, 3_operation_with_5_elements) {
  int operation     = 3;
  int sz            = 5;
  int rank          = 0;
  int comm_sz       = 0;
  int result        = 0;
  int Local_result  = 0;
  int Global_result = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Table(sz, comm_sz);

    for (int i = 1; i < comm_sz; i++) {
      result += i + 4;
    }
  } else {
    std::array<int, 5U> array{ { rank, rank + 1, rank + 2, rank + 3, rank + 4 } };

    Pilosophers(&Local_result, array.data(), rank, comm_sz, operation, sz);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&Local_result, &Global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(Global_result, result);
  }
}

TEST(Dining_Philosophers, 3_operation_with_2_elements) {
  int operation     = 3;
  int sz            = 2;
  int rank          = 0;
  int comm_sz       = 0;
  int result        = 0;
  int Local_result  = 0;
  int Global_result = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Table(sz, comm_sz);

    for (int i = 1; i < comm_sz; i++) {
      result += i + 1;
    }
  } else {
    std::array<int, 2U> array{ { rank, rank + 1} };

    Pilosophers(&Local_result, array.data(), rank, comm_sz, operation, sz);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&Local_result, &Global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(Global_result, result);
  }
}

TEST(Dining_Philosophers, 3_operation_with_0_elements) {
  int operation     = 3;
  int sz            = 0;
  int rank          = 0;
  int comm_sz       = 0;
  int result        = 0;
  int Local_result  = 0;
  int Global_result = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Table(sz, comm_sz);

    for (int i = 1; i < comm_sz; i++) {
      result += 0;
    }
  } else {
    std::array<int, 0U> array{ { } };

    Pilosophers(&Local_result, array.data(), rank, comm_sz, operation, sz);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&Local_result, &Global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(Global_result, result);
  }
}


TEST(Dining_Philosophers, 2_operation_with_3_elements) {
  int operation     = 2;
  int sz            = 3;
  int rank          = 0;
  int comm_sz       = 0;
  int result        = 0;
  int Local_result  = 0;
  int Global_result = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Table(sz, comm_sz);

    for (int i = 1; i < comm_sz; i++) {
      result += i + (i + 1) + (i + 2);
    }
  } else {
    std::array<int, 3U> array{ { rank, rank + 1, rank + 2 } };

    Pilosophers(&Local_result, array.data(), rank, comm_sz, operation, sz);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&Local_result, &Global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(Global_result, result);
  }
}


TEST(Dining_Philosophers, 1_operation_with_3_elements) {
  int operation     = 1;
  int sz            = 3;
  int rank          = 0;
  int comm_sz       = 0;
  int result        = 0;
  int Local_result  = 1;
  int Global_result = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Table(sz, comm_sz);

    for (int proc = 1; proc < comm_sz; proc++) {
      result += proc * (proc + 1) * (proc + 2);
    }
  } else {
    std::array<int, 3U> array{ { rank, rank + 1, rank + 2 } };

    Pilosophers(&Local_result, array.data(), rank, comm_sz, operation, sz);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Reduce(&Local_result, &Global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    Global_result--;
    ASSERT_EQ(Global_result, result);
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

