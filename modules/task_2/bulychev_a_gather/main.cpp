// Copyright 2020 Bulychev Andrey
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <vector>
#include "./bulychev_a_gather.h"

TEST(Parallel_Operations_MPI, Test_My_Gather_No_Throw) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(size);
    int tmp = rank;
    ASSERT_NO_THROW(My_Gather(&tmp, 1, MPI_INT, &vec[0], 1, MPI_INT, 0, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Test_My_Gather_ERROR_root) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vec(size);
    int tmp = rank;
    ASSERT_EQ(My_Gather(&tmp, 10, MPI_INT, &vec[0], 10, MPI_INT, -4, MPI_COMM_WORLD), MPI_ERR_ROOT);
}

TEST(Parallel_Operations_MPI, Test_My_Gather_ERROR_TYPE) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(size);
    int tmp = rank;
    ASSERT_EQ(MPI_ERR_TYPE, My_Gather(&tmp, 1, MPI_INT, &vec[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Test_My_Gather_ERROR_COUNT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(size);
    int tmp = rank;
    ASSERT_EQ(MPI_ERR_COUNT, My_Gather(&tmp, 1, MPI_INT, &vec[0], 2, MPI_INT, 0, MPI_COMM_WORLD));
}

TEST(Paral, Test_My_Gather_INT) {
    int rank, size , count = 5, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int tmp_size = count * size;
    std::vector<int> buf(count);
    std::vector<int> mpi_vec(tmp_size);
    std::vector<int> my_vec(tmp_size);
    if (rank == root) {
        for (int i = 0; i < count; i++) {
            buf[i] = count*i+7;
        }
    }
    MPI_Gather(buf.data(), count, MPI_INT, mpi_vec.data(), count, MPI_INT, root, MPI_COMM_WORLD);
    My_Gather(buf.data(), count, MPI_INT, my_vec.data(), count, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < tmp_size; i++) {
            ASSERT_EQ(mpi_vec[i], my_vec[i]);
        }
    }
}

TEST(Paral, Test_My_Gather_DOUBLE) {
    int rank, size, count = 7, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int tmp_size = count * size;
    std::vector<double> buf(count);
    std::vector<double> mpi_vec(tmp_size);
    std::vector<double> my_vec(tmp_size);
    if (rank == root) {
        for (int i = 0; i < count; i++) {
            buf[i] = count * i + 13.0;
        }
    }
    MPI_Gather(buf.data(), count, MPI_DOUBLE, mpi_vec.data(), count, MPI_DOUBLE, root, MPI_COMM_WORLD);
    My_Gather(buf.data(), count, MPI_DOUBLE, my_vec.data(), count, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < tmp_size; i++) {
            ASSERT_EQ(mpi_vec[i], my_vec[i]);
        }
    }
}

TEST(Paral, Test_My_Gather_FLOAT) {
    int rank, size, count = 17, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int tmp_size = count * size;
    std::vector<float> buf(count);
    std::vector<float> mpi_vec(tmp_size);
    std::vector<float> my_vec(tmp_size);
    if (rank == root) {
        for (int i = 0; i < count; i++) {
            buf[i] = count * i;
        }
    }
    MPI_Gather(buf.data(), count, MPI_FLOAT, mpi_vec.data(), count, MPI_FLOAT, root, MPI_COMM_WORLD);
    My_Gather(buf.data(), count, MPI_FLOAT, my_vec.data(), count, MPI_FLOAT, root, MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < tmp_size; i++) {
            ASSERT_EQ(mpi_vec[i], my_vec[i]);
        }
    }
}

TEST(Parallel_Lab2_MPI, Test_time) {
    int rank, size; int root = 0, count = 5;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int tmp_size = count * size;
    std::vector<int> buf(count);
    std::vector<int> mpi_vec(tmp_size);
    std::vector<int> my_vec(tmp_size);
    if (rank == root) {
        for (int i = 0; i < count; i++) {
            buf[i] = count * i + 9;
        }
    }
    double mpi_time_start = MPI_Wtime();
    MPI_Gather(buf.data(), count, MPI_INT, mpi_vec.data(), count, MPI_INT, root, MPI_COMM_WORLD);
    double mpi_time_end = MPI_Wtime();
    double my_time_start = MPI_Wtime();
    My_Gather(buf.data(), count, MPI_INT, my_vec.data(), count, MPI_INT, root, MPI_COMM_WORLD);
    double my_time_end = MPI_Wtime();
    std::cout << "My Gather : " << my_time_end - my_time_start << std::endl;
    std::cout << "MPI Gather : " << mpi_time_end - mpi_time_start << std::endl;
    if (my_time_end - my_time_start == mpi_time_end - mpi_time_start) {
        std::cout << "Respect" << std::endl;
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
