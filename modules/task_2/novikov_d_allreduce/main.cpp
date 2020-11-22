// Copyright 2020 Novikov Danil
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./allreduce.h"

TEST(Parallel_Operations_MPI, Test_task_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int count_size = 5000000;
    std::vector<int> vec;
    std::vector<int> res_vec1(count_size), res_vec2(count_size);

    vec = getRandomVector<int>(count_size);

    double t1;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    my::MPI_Allreduce(&vec[0], &res_vec1[0], count_size, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "My Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        t1 = MPI_Wtime();
    }

    MPI_Allreduce(&vec[0], &res_vec2[0], count_size, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "MPI Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        ASSERT_EQ(res_vec1, res_vec2);
    }
}

TEST(Parallel_Operations_MPI, Test_task_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int count_size = 5000000;
    std::vector<int> vec;
    std::vector<int> res_vec1(count_size), res_vec2(count_size);

    vec = getRandomVector<int>(count_size);

    double t1;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    my::MPI_Allreduce(&vec[0], &res_vec1[0], count_size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "My Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        t1 = MPI_Wtime();
    }

    MPI_Allreduce(&vec[0], &res_vec2[0], count_size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "MPI Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        ASSERT_EQ(res_vec1, res_vec2);
    }
}

TEST(Parallel_Operations_MPI, Test_task_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int count_size = 5000000;
    std::vector<double> vec;
    std::vector<double> res_vec1(count_size), res_vec2(count_size);

    vec = getRandomVector<double>(count_size);

    double t1;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    my::MPI_Allreduce(&vec[0], &res_vec1[0], count_size, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "My Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        t1 = MPI_Wtime();
    }

    MPI_Allreduce(&vec[0], &res_vec2[0], count_size, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "MPI Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        ASSERT_EQ(res_vec1, res_vec2);
    }
}

TEST(Parallel_Operations_MPI, Test_task_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int count_size = 5000000;
    std::vector<double> vec;
    std::vector<double> res_vec1(count_size), res_vec2(count_size);

    vec = getRandomVector<double>(count_size);

    double t1;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    my::MPI_Allreduce(&vec[0], &res_vec1[0], count_size, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "My Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        t1 = MPI_Wtime();
    }

    MPI_Allreduce(&vec[0], &res_vec2[0], count_size, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "MPI Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        ASSERT_EQ(res_vec1, res_vec2);
    }
}

TEST(Parallel_Operations_MPI, Test_task_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int count_size = 5000000;
    std::vector<float> vec;
    std::vector<float> res_vec1(count_size), res_vec2(count_size);

    vec = getRandomVector<float>(count_size);

    double t1;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    my::MPI_Allreduce(&vec[0], &res_vec1[0], count_size, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "My Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        t1 = MPI_Wtime();
    }

    MPI_Allreduce(&vec[0], &res_vec2[0], count_size, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "MPI Allreduce Time: " << MPI_Wtime() - t1 << std::endl;
        ASSERT_EQ(res_vec1, res_vec2);
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
