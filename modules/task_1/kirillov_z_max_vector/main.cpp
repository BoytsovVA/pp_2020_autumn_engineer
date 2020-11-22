// Copyright 2020 Kirillov Zahar
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./max_vector.h"

TEST(Parallel_Operations_MPI, MAX_COUNT) {
    int grade;
    MPI_Comm_rank(MPI_COMM_WORLD, &grade);
    std::vector<int> vec;
    const int length = 100;
    if (grade == 0) {
        vec = makeVector(length);
    }
    ASSERT_NO_THROW(parallelMax(vec, length));
}

TEST(Parallel_Operations_MPI, MAX_TEST) {
    int grade;
    MPI_Comm_rank(MPI_COMM_WORLD, &grade);
    std::vector<int> vec;
    const int length = 100;
    if (grade == 0) {
        vec = makeVector(length);
    }
    int parMax = parallelMax(vec, length);
    if (grade == 0) {
        int defMax = locMax(vec);
        ASSERT_EQ(defMax, parMax);
    }
}

TEST(Parallel_Operations_MPI, LOC_MAX) {
    int grade;
    MPI_Comm_rank(MPI_COMM_WORLD, &grade);
    std::vector<int> vec = { 3, 4, 9, -6 };
    if (grade == 0) {
        int defMax = locMax(vec);
        ASSERT_EQ(9, defMax);
    }
}

TEST(Parallel_Operations_MPI, RANDOM_VECTOR) {
    int grade;
    MPI_Comm_rank(MPI_COMM_WORLD, &grade);
    const int length = 100;
    if (grade == 0) {
        ASSERT_NO_THROW(makeVector(length));
    }
}

TEST(Parallel_Operations_MPI, NEGATIVE_LENGTH) {
    int grade;
    MPI_Comm_rank(MPI_COMM_WORLD, &grade);
    std::vector<int> vec;
    const int length = -20;
    if (grade == 0) {
        ASSERT_ANY_THROW(makeVector(length));
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
