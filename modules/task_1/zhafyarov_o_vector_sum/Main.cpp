  // Copyright 2020 Zhafyarov Oleg
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./vector_sum.h"

TEST(Parallel_Operations_MPI, Test_Sum) {
    int size = 1001;
    std::vector<int> vec(size);
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
      vec = GetRandomVector(size);
    }

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
      int sequential_sum = GetSequentialSum(vec);
      ASSERT_EQ(parallel_sum, sequential_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector1) {
    int size = 5;
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    int sum = 15;
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
        ASSERT_EQ(parallel_sum, sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector2) {
    int size = 5;
    std::vector<int> vec = { 0, 0, 0, 0, 0 };
    int sum = 0;
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
        ASSERT_EQ(parallel_sum, sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector3) {
    int size = 5;
    std::vector<int> vec = { -1, -2, -3, -4, -5 };
    int sum = -15;
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
        ASSERT_EQ(parallel_sum, sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector4) {
    int size = 10;
    std::vector<int> vec(size);
    int sequential_sum = 0;
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
        vec = GetRandomVector(size);
        vec.at(size - 1) = 5;
    }

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
        vec.pop_back();
        sequential_sum = GetSequentialSum(vec);
        ASSERT_NE(parallel_sum, sequential_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector5) {
    int size = 0;
    std::vector<int> vec(size);
    int sum = 0;
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
        vec = GetRandomVector(size);
    }

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
        ASSERT_EQ(parallel_sum, sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Vector6) {
    int size = 1;
    std::vector<int> vec(size);
    int sum = 5;
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0) {
        vec[0] = 5;
    }

    int parallel_sum = GetParallelSum(vec, size);

    if (process_rank == 0) {
        ASSERT_EQ(parallel_sum, sum);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}


