// Copyright 2020 Sharov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "./vector_different.h"

TEST(Parallel_Operations_MPI, Test_Create_Vector) {
    std::vector<int> global_vec;
    const int count_size_vector = 27;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        ASSERT_NO_THROW(global_vec = getRandomVector(count_size_vector));
    }
}

TEST(Parallel_Operations_MPI, Test_Create_Vector_With_Wrong_Size) {
    std::vector<int> global_vec;
    const int count_size_vector = -27;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        ASSERT_ANY_THROW(getRandomVector(count_size_vector));
    }
}

TEST(Parallel_Operations_MPI, Test_Create_Vector_With_Small_Size) {
    std::vector<int> global_vec;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    const int count_size_vector = 3;
    int max_diff_parallel[2] = { 0, 0 };

    if (ProcRank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    ASSERT_NO_THROW(getParallelOperations(global_vec, count_size_vector, max_diff_parallel));
}

TEST(Parallel_Operations_MPI, Test_Compare_Parallel_And_Sequential) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> global_vec;
    const int count_size_vector = 59;
    int max_diff_parallel[2] = { 0, 0 }, max_diff_sequential[2] = { 0, 0 };

    if (ProcRank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    getParallelOperations(global_vec, count_size_vector, max_diff_parallel);

    if (ProcRank == 0) {
        getSequentialOperations(global_vec, max_diff_sequential);
        ASSERT_EQ(abs(max_diff_parallel[1] - max_diff_parallel[0]),
            abs(max_diff_sequential[1] - max_diff_sequential[0]));
    }
}

TEST(Parallel_Operations_MPI, Test_Compare_Parallel_And_Sequential_With_Vector_With_Small_Size) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> global_vec;
    const int count_size_vector = 3;
    int max_diff_parallel[2] = { 0, 0 }, max_diff_sequential[2] = { 0, 0 };

    if (ProcRank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    getParallelOperations(global_vec, count_size_vector, max_diff_parallel);

    if (ProcRank == 0) {
        getSequentialOperations(global_vec, max_diff_sequential);
        ASSERT_EQ(abs(max_diff_parallel[1] - max_diff_parallel[0]),
            abs(max_diff_sequential[1] - max_diff_sequential[0]));
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
