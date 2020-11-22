// Copyright 2020 Bulychev Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./max_val_of_vector.h"


TEST(Parallel_Operations_MPI, Generate_correct_vector) {
    ASSERT_NO_THROW(GetRndVector(35));
}

TEST(Parallel_Operations_MPI, Not_generated_Vector_with_incorrect_size) {
    ASSERT_ANY_THROW(GetRndVector(0));
}

TEST(Parallel_Operations_MPI, Correct_work_SequentialMAX_function) {
    std::vector<int> vec{ -10, 45, -90, 7566, -10000, 876 };
    ASSERT_NO_THROW(getSequentialMAX(vec));
}
TEST(Parallel_Operations_MPI, Test_SequentialMAX) {
    std::vector<int> vec{ -10, 45, -90, 7566, -10000, 876 };
    ASSERT_EQ(7566, getSequentialMAX(vec));
}


TEST(Parallel_Operations_MPI, Correct_work_ParallelMAX_function) {
    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    std::vector<int> vec;
    const int vector_size = 75;
    if (Rank == 0) {
        vec = GetRndVector(vector_size);
    }
    ASSERT_NO_THROW(getParallelMAX(vec, vector_size));
}

TEST(Parallel_Operations_MPI, Test_ParallelMax) {
    int Rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    std::vector<int> vec;
    const int vector_size = 150;
    if (Rank == 0) {
        vec = GetRndVector(vector_size);
    }
    int parallel_max = getParallelMAX(vec, vector_size);
    if (Rank == 0) {
        int local_max = getSequentialMAX(vec);
        ASSERT_EQ(local_max, parallel_max);
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
