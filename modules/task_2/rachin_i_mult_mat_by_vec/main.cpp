// Copyright 2020 Rachin Igor
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./mult_mat_by_vec.h"

TEST(Parallel_Operations_MPI, Test_manual_matrix_seq_mult_by_manual_vect) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> test{ 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1};
    std::vector<int> vec{ 100000, 10000, 1000, 100, 10, 1};
    std::vector<int> result{ 111111, 111111, 111111};
    if (rank == 0) {
        ASSERT_EQ(getSequentialMult(test, 3, 6, vec, 6), result);
    }
}

TEST(Parallel_Operations_MPI, Test_manual_matrix_parallel_mult_by_manual_vect) {
    std::vector<int> test{ 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1 };
    std::vector<int> vec{ 100000, 10000, 1000, 100, 10, 1 };
    std::vector<int> result{ 111111, 111111, 111111 };
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> resultParallel = getParallelMult(test, 3, 6, vec, 6);
    if (rank == 0) {
        ASSERT_EQ(resultParallel, result);
    }
}

TEST(Parallel_Operations_MPI, Test_random_matrix_parallel_mult_by_random_vect) {
    int rank;
    std::vector<int> testMx;
    std::vector<int> testVec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        testMx = getRandomMatrix(5, 5);
        testVec = getRandomVector(5);
    }
    std::vector<int> result = getParallelMult(testMx, 5, 5, testVec, 5);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMult(testMx, 5, 5, testVec, 5));
    }
}

TEST(Parallel_Operations_MPI, Test_matrix_negative_size) {
    std::vector<int> test;
    std::vector<int> clear;  // empty vector
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        test = getRandomMatrix(-2, -2);
    }
    if (rank == 0) {
        ASSERT_EQ(test, clear);
    }
}

TEST(Parallel_Operations_MPI, Test_vector_negative_size) {
    std::vector<int> test;
    std::vector<int> clear;  // empty vector
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        test = getRandomVector(-2);
    }
    if (rank == 0) {
        ASSERT_EQ(test, clear);
    }
}

TEST(Parallel_Operations_MPI, Test_vector_size_diff_matrix_clmns) {
    int rank;
    std::vector<int> testMx;
    std::vector<int> testVec;
    std::vector<int> clear;  // empty vector
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        testMx = getRandomMatrix(5, 10);
        testVec = getRandomVector(5);
    }
    std::vector<int> result = getParallelMult(testMx, 5, 10, testVec, 5);
    if (rank == 0) {
        ASSERT_EQ(result, clear);
    }
}

TEST(Parallel_Operations_MPI, Test_random_matrix_100x150_parallel_mult_by_rand_vect) {
    int rank;
    std::vector<int> testMx;
    std::vector<int> testVec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        testMx = getRandomMatrix(100, 150);
        testVec = getRandomVector(150);
    }
    std::vector<int> result = getParallelMult(testMx, 100, 150, testVec, 150);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMult(testMx, 100, 150, testVec, 150));
    }
}

TEST(Parallel_Operations_MPI, Test_random_matrix_100x100_parallel_mult_by_rand_vect) {
    int rank;
    std::vector<int> testMx;
    std::vector<int> testVec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        testMx = getRandomMatrix(100, 100);
        testVec = getRandomVector(100);
    }
    std::vector<int> result = getParallelMult(testMx, 100, 100, testVec, 100);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMult(testMx, 100, 100, testVec, 100));
    }
}

TEST(Parallel_Operations_MPI, Test_random_matrix_5000x10000_parallel_mult_by_rand_vect) {
    int rank;
    std::vector<int> testMx;
    std::vector<int> testVec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        testMx = getRandomMatrix(5000, 10000);
        testVec = getRandomVector(10000);
    }
    std::vector<int> result = getParallelMult(testMx, 5000, 10000, testVec, 10000);
    if (rank == 0) {
        ASSERT_EQ(result, getSequentialMult(testMx, 5000, 10000, testVec, 10000));
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
