// Copyright 2020 Elandaev Pavel

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./elandaev_p_find_vector_error.h"

TEST(Parallel_Operations_MPI, Test_Sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 100;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_sum = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_sum = getSequentialOperations(global_vec, count_size_vector);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_sv_long) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec(5);
    if (rank == 0) {
        global_vec[0] = 1;
        global_vec[1] = 3;
        global_vec[2] = 1;
        global_vec[3] = 3;
        global_vec[4] = 10;
    }
    int count = getParallelOperations(global_vec, 5);
    if (rank == 0) {
        ASSERT_EQ(1, count);
    }
}
TEST(Parallel_Operations_MPI, Test_Parallel_sv_short) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec(2);
    if (rank == 0) {
        global_vec[0] = 1;
        global_vec[1] = 10;
    }
    int count = getParallelOperations(global_vec, 2);
    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_sv_small) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec(1);
    if (rank == 0) {
        global_vec[0] = 1;
    }
    int count = getParallelOperations(global_vec, 1);
    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_sv_smallest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec(0);
    int count = getParallelOperations(global_vec, 0);
    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 100;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_max;
    global_max = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_max = getSequentialOperations(global_vec, count_size_vector);
        ASSERT_EQ(reference_max, global_max);
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
