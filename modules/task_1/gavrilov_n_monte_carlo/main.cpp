// Copyright 2020 Gavrilov Nikita
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <stdio.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <algorithm>
#include "./monte_carlo.h"

bool isAlmostEqual(double a, double b) {
    const double e = 0.01;
    return ((a - b) < e) || ((b - a) < e);
}

double myFunc(double x) {
    return x;
}

TEST(Parallel_Operations_MPI, Non_Parallel_Integral) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double result;
        ASSERT_NO_THROW(result = getIntegral(0, 4, 100, myFunc));
        ASSERT_TRUE(isAlmostEqual(result, 8));
    }
}

TEST(Parallel_Operations_MPI, Parallel_Integral) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double result;
    ASSERT_NO_THROW(result = getIntegralParallel(0, 4, 100, myFunc));
    if (rank == 0) {
        ASSERT_TRUE(isAlmostEqual(result, 8));
    }
}

TEST(Parallel_Operations_MPI, Swaped_Min_Max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double result;
        ASSERT_NO_THROW(result = getIntegral(4, 0, 100, myFunc));
        ASSERT_TRUE(isAlmostEqual(result, -8));
    }

    double result;
    ASSERT_NO_THROW(result = getIntegralParallel(4, 0, 100, myFunc));
    if (rank == 0) {
        ASSERT_TRUE(isAlmostEqual(result, -8));
    }
}

TEST(Parallel_Operations_MPI, Splits_Should_Be_More_Than_0) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(getIntegral(0, 4, 0, myFunc));
    }
    ASSERT_ANY_THROW(getIntegralParallel(0, 4, -1344, myFunc));
}

TEST(Parallel_Operations_MPI, Splits_Less_Than_MPI_Comm_Size) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > 1) {
        double result;
        ASSERT_NO_THROW(result = getIntegralParallel(0, 4, size-1, myFunc));
        if (rank == 0) {
            ASSERT_TRUE(isAlmostEqual(result, 8));
        }
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
