// Copyright 2020 Tronin Dmitry
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include <algorithm>
#include "./min_row_element.h"

void TestCreate(const size_t size);

TEST(ParallelMibRowElement, medium) {
    TestCreate(50);
}

TEST(ParallelMibRowElement, empty) {
    TestCreate(0);
}

TEST(ParallelMibRowElement, small) {
    TestCreate(1);
}

TEST(ParallelMibRowElement, big) {
    TestCreate(100);
}

TEST(ParallelMibRowElement, very_big) {
    TestCreate(1000);
}

void TestCreate(const size_t size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int32_t> vector;
    if (rank == 0) {
        vector = getRandomVector(size);
    }

    std::vector<int32_t> parallel_answer =
            getMinRowElementParallel(vector, size);

    if (rank == 0) {
        std::vector<int32_t> sequential_answer =
                getMinRowElementSequential(vector, size);
        ASSERT_EQ(sequential_answer, parallel_answer);
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
