// Copyright 2020 Tronin Dmitry
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/tronin_d_sleeping_hairdresser/sleeping_hairdresser.h"


TEST(MPI_Parallel, Empty_queue) {
    ASSERT_NO_THROW(SleepingBarber(0));
}

TEST(MPI_Parallel, small_queue) {
    ASSERT_NO_THROW(SleepingBarber(3));
}

TEST(MPI_Parallel, medium_queue) {
    ASSERT_NO_THROW(SleepingBarber(10));
}

TEST(MPI_Parallel, big_queue) {
    ASSERT_NO_THROW(SleepingBarber(50));
}

TEST(MPI_Parallel, large_queue) {
    ASSERT_NO_THROW(SleepingBarber(100));
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
