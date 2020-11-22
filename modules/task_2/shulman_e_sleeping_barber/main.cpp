// Copyright 2020 Shulman Egor
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./sleeping_barber.h"

TEST(Parallel_Operations_MPI, Chairs_Negative) {
    const int chairs = -1;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    EXPECT_ANY_THROW(Sleeping_Barber(chairs));
}

TEST(Parallel_Operations_MPI, Chairs_0) {
    const int chairs = 0;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    int ReturnCode = Sleeping_Barber(chairs);
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_EQ(0, ReturnCode);
}

TEST(Parallel_Operations_MPI, Chairs_5) {
    const int chairs = 5;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    int ReturnCode = Sleeping_Barber(chairs);
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_EQ(0, ReturnCode);
}

TEST(Parallel_Operations_MPI, Chairs_10) {
    const int chairs = 10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    int ReturnCode = Sleeping_Barber(chairs);
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_EQ(0, ReturnCode);
}

TEST(Parallel_Operations_MPI, Chairs_100) {
    const int chairs = 100;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    int ReturnCode = Sleeping_Barber(chairs);
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_EQ(0, ReturnCode);
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
