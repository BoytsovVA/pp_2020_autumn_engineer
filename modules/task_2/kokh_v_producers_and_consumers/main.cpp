// Copyright 2020 Kokh Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include "./producers_and_consumers.h"

TEST(Parallel_Operations_MPI, Test_1) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lens;
    int first = 2;
    int second = 4;

    if (rank == 0) {
        int value = second * (size / 2) + first * ((size - 1) / 2);
        lens = mainpar(value);
    }
    sleepFunction(0.5);
    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < first; i++) {
                consume();
            }
        } else {
            for (int i = 0; i < second; i++) {
                produce();
            }
        }
    }

    if (rank == 0) {
        int assert_value = second * (size / 2) - first * ((size - 1) / 2);
        ASSERT_EQ(lens, assert_value);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_2) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lens;
    int first = 4;
    int second = 4;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        int value = second * (size / 2) + first * ((size - 1) / 2);
        lens = mainpar(value);
    }

    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < first; i++) {
                consume();
            }
        } else {
            for (int i = 0; i < second; i++) {
                produce();
            }
        }
    }

    if (rank == 0) {
        int assert_value = second * (size / 2) - first * ((size - 1) / 2);
        ASSERT_EQ(lens, assert_value);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_3) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lens;
    int first = 2;
    int second = 4;

    if (rank == 0) {
        int value = second * (size / 2) + first * ((size - 1) / 2);
        lens = mainpar(value);
    }

    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < first; i++) {
                consume();
            }
        } else {
            for (int i = 0; i < second; i++) {
                produce();
            }
        }
    }
    if (rank == 0) {
        int assert_value = second * (size / 2) - first * ((size - 1) / 2);
        ASSERT_EQ(lens, assert_value);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_4) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lens;
    int first = 5;
    int second = 10;

    if (rank == 0) {
        int value = second * (size / 2) + first * ((size - 1) / 2);
        lens = mainpar(value);
    }

    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < first; i++) {
                consume();
            }
        } else {
            for (int i = 0; i < second; i++) {
                produce();
            }
        }
    }
    if (rank == 0) {
        int assert_value = second * (size / 2) - first * ((size - 1) / 2);
        ASSERT_EQ(lens, assert_value);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test_5) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int lens;
    int first = 20;
    int second = 20;

    if (rank == 0) {
        int value = second * (size / 2) + first * ((size - 1) / 2);
        lens = mainpar(value);
    }

    if (rank != 0) {
        if (rank % 2 == 0) {
            for (int i = 0; i < first; i++) {
                consume();
            }
        } else {
            for (int i = 0; i < second; i++) {
                produce();
            }
        }
    }
    if (rank == 0) {
        int assert_value = second * (size / 2) - first * ((size - 1) / 2);
        ASSERT_EQ(lens, assert_value);
    }
    MPI_Barrier(MPI_COMM_WORLD);
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
