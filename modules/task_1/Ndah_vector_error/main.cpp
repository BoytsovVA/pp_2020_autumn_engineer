// Copyright 2020 NDAH RONY
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./vec_er_mpi.h"

TEST(VEC_MPI, THROW_WHEN_SIZE_IS_LOWER_THAN_0) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int s = -10;
    if (r == 0) {
        ASSERT_ANY_THROW(vec = RV(s));
    }
}

TEST(VEC_MPI, THROW_WHEN_SIZE_IS_0) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int s = 0;
    if (r == 0) {
        ASSERT_ANY_THROW(vec = RV(s));
    }
}

TEST(VEC_MPI, SUCCESSFUL_SWAP_10_ELEM) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int s = 10;
    if (r == 0) {
        vec = RV(s);
        vec = vecNor(vec);
    }
    int res = parallelVec(vec, s);
    if (r == 0) {
        ASSERT_EQ(res, 0);
    }
}

TEST(VEC_MPI, SUCCESSFUL_SWAP_500_ELEM) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int vec_size = 500;
    if (r == 0) {
        vec = RV(vec_size);
        vec = vecNor(vec);
    }
    int res = parallelVec(vec, vec_size);
    if (r == 0) {
        ASSERT_EQ(res, 0);
    }
}

TEST(VEC_MPI, SUCCESSFUL_SWAP_1000_ELEM) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int s = 1000;
    if (r == 0) {
        vec = RV(s);
        vec = vecNor(vec);
    }
    int res = parallelVec(vec, s);
    if (r == 0) {
        ASSERT_EQ(res, 0);
    }
}

TEST(VEC_MPI, SUCCESSFUL_SWAP_10000_ELEM) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int s = 10000;
    if (r == 0) {
        vec = RV(s);
        vec = vecNor(vec);
    }
    int res = parallelVec(vec, s);
    if (r == 0) {
        ASSERT_EQ(res, 0);
    }
}

TEST(VEC_MPI, COUNT_ERROR_NOT_A_NULL) {
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    std::vector<int> vec;
    const int s = 100;
    if (r == 0) {
        vec = RV(s);
    }
    int res = parallelVec(vec, s);
    if (r == 0) {
        ASSERT_NE(res, 0);
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

