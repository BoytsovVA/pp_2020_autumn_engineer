// Copyright 2018 Nesterov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_sum.h"

TEST(matrix_sum_test, sequential_test_casual) {  // casual test of sequential operaton
    int rank;
    std::vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    int expect = 45, res;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        res = getSequentialSum(vec);
        ASSERT_EQ(res, expect);
    }
}

TEST(matrix_sum_test, parallel_test_casual) {  // testing basics on small matrix
    int rank;
    std::vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    int res = getParallelSum(vec, 10);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int expect = getSequentialSum(vec);  // we're assuming that it works fine
        ASSERT_EQ(res, expect);
    }
}

TEST(matrix_sum_test, general_parallel_test) {  // general test of parallel summing of 10000 elements
    int rank;
    std::vector<int> vec = getRandomVector(10000);

    int res = getParallelSum(vec, 10000);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int expect = getSequentialSum(vec);  // we're assuming that it works fine
        ASSERT_EQ(res, expect);
    }
}

TEST(matrix_sum_test, general_parallel_test_large) {  // same test on 10000000 elements
    int rank;
    std::vector<int> vec = getRandomVector(10000000);

    int res = getParallelSum(vec, 10000000);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int expect = getSequentialSum(vec);  // we're assuming that it works fine
        ASSERT_EQ(res, expect);
    }
}

/* TEST(matrix_sum_test, time_camparing_test) {  // test passed if parallel time < sequential time
    int rank;
    std::vector<int> vec = getRandomVector(10000);
    double starttime, endtime, dif_parallel_time;

    starttime = MPI_Wtime();
    getParallelSum(vec, 10000);
    endtime = MPI_Wtime();
    dif_parallel_time = endtime - starttime;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        clock_t start, end;
        double dif_sequential_time;

        start = clock();
        getSequentialSum(vec);
        end = clock();

        dif_sequential_time = static_cast<double>(end - start);

        ASSERT_TRUE(dif_sequential_time <= dif_parallel_time);
    }
} */

TEST(matrix_sum_test, time_camparing_test_large) {  // test passed if parallel time < sequential time
    int rank;
    std::vector<int> vec = getRandomVector(10000000);
    double starttime, endtime, dif_parallel_time;

    starttime = MPI_Wtime();
    getParallelSum(vec, 10000000);
    endtime = MPI_Wtime();
    dif_parallel_time = endtime - starttime;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        clock_t start, end;
        double dif_sequential_time;

        start = clock();
        getSequentialSum(vec);
        end = clock();

        dif_sequential_time = static_cast<double>(end - start);

        ASSERT_TRUE(dif_sequential_time >= dif_parallel_time);
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
