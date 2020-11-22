// Copyright 2020 Denis Kadnikov
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/kadnikov_d_multi_scalar/multi_scalar.h"

TEST(multi_scalar, can_create_vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v1, v2;
    const int v_size = 100;

    if (rank == 0) {
        ASSERT_NO_THROW(v1 = createRandomVector(v_size));
    }
}

TEST(multi_scalar, can_multiply_vectors_with_100_range) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1(100);
    std::vector<int> vec2(100);

    if (rank == 0) {
        vec1 = createRandomVector(100);
        vec2 = createRandomVector(100);
    }

    int res1 = getParallelMultiScalar(vec1, vec2);
    if (rank == 0) {
        int res2 = getSequentialMultiScalar(vec1, vec2);
        ASSERT_EQ(res1, res2);
    }
}

TEST(multi_scalar, can_throw_if_multiply_vectors_with_different_ranges) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1 = createRandomVector(100);
    std::vector<int> vec2 = createRandomVector(120);

    if (rank == 0) {
        ASSERT_ANY_THROW(getParallelMultiScalar(vec1, vec2));
    }
}

TEST(multi_scalar, can_throw_if_vector_with_negative_range) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v1, v2;
    const int v_size = -100;

    if (rank == 0) {
        ASSERT_ANY_THROW(createRandomVector(v_size));
    }
}

TEST(multi_scalar, can_multiply_vectors_with_big_ranges) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1(10000);
    std::vector<int> vec2(10000);
    if (rank == 0) {
        vec1 = createRandomVector(10000);
        vec2 = createRandomVector(10000);
    }

    int res1 = getParallelMultiScalar(vec1, vec2);
    if (rank == 0) {
        int res2 = getSequentialMultiScalar(vec1, vec2);
        ASSERT_EQ(res1, res2);
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
