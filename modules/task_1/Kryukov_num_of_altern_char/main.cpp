  // Copyright 2020 Kryukov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./Kryukov_number_of_alternating_char.h"

TEST(Parallel_Operations_MPI, Test_num_altern) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> general_vec;
    const int size_vector = 10;
    if (rank == 0) {
        general_vec = getRandomVector(size_vector);
    }
    int global_sum = getParallelNUMalternCHAR(general_vec, size_vector);
    if (rank == 0) {
        int received_summa = getNUMalternCHAR(general_vec);
        ASSERT_EQ(global_sum, received_summa);
    }
}

TEST(Parallel_Operations_MPI, Test_create_vector_with_correct_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> general_vec;
    const int size_vector = 10;
    general_vec = getRandomVector(size_vector);
    if (rank == 0) {
        ASSERT_EQ((int)general_vec.size(), size_vector);
    }
}

TEST(Parallel_Operation_MPI, Test_create_vector_with_negative_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> general_vec;
    const int size_vector = -10;
    if (rank == 0) {
        ASSERT_ANY_THROW(getRandomVector(size_vector));
    }
}

TEST(Parallel_Operation_MPI, Test_create_vector_with_positive_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> general_vec;
    const int size_vector = 1110;
    if (rank == 0) {
         ASSERT_NO_THROW(general_vec = getRandomVector(size_vector));
    }
}

TEST(Parallel_Operation_MPI, check_correct_realized_algorithm_work) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> general_vec = { 1, -1, 0, 2, -2, 5 };
    std::vector<int> general_vec1 = { 1, 1, 0, 2, 5 };
    int result1 = getNUMalternCHAR(general_vec);
    int result2 = getNUMalternCHAR(general_vec1);
    if (rank == 0) {
        ASSERT_EQ(result1, 4);
        ASSERT_EQ(result2, 0);
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
