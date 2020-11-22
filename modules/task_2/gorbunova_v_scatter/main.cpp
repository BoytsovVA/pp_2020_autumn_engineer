// Copyright 2020 Gorbunova Valeria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <string>
#include "./Scatter.h"


TEST(Scatter, Test_Sum_part_int) {
    int len = 10000;
    int* arr = new int[len];
    int sum = 0, res_sum = 0;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0;
    arr[len] = gen_int(arr, len);
    int num = len / size;
    int* recv_arr = new int[num];
    MyScatter(arr, num, MPI_INT, recv_arr, num, MPI_INT, root, MPI_COMM_WORLD);
    for (int j = 0; j < num; j++) sum += arr[num * rank + j];
    for (int i = 0; i < num; i++) res_sum += recv_arr[i];
    ASSERT_EQ(res_sum, sum);
}

TEST(Scatter, Test_Sum_part_double) {
    int len = 10000;
    double* arr = new double[len];
    double sum = 0, res_sum = 0;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0;
    arr[len] = gen_double(arr, len);
    int num = len / size;
    double* recv_arr = new double[num];
    MyScatter(arr, num, MPI_DOUBLE, recv_arr, num, MPI_DOUBLE, root, MPI_COMM_WORLD);
    for (int j = 0; j < num; j++) sum += arr[num * rank + j];
    for (int i = 0; i < num; i++) res_sum += recv_arr[i];
    ASSERT_EQ(res_sum, sum);
}

TEST(Scatter, Test_Sum_part_float) {
    int len = 10000;
    float* arr = new float[len];
    float sum = 0, res_sum = 0;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0;
    arr[len] = gen_float(arr, len);
    int num = len / size;
    float* recv_arr = new float[num];
    MyScatter(arr, num, MPI_FLOAT, recv_arr, num, MPI_FLOAT, root, MPI_COMM_WORLD);
    for (int j = 0; j < num; j++) sum += arr[num * rank + j];
    for (int i = 0; i < num; i++) res_sum += recv_arr[i];
    ASSERT_EQ(res_sum, sum);
}

TEST(Scatter, Test_time_of_different_scatter) {
    int len = 10000;
    int* arr = new int[len];
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int i = 0; i < len; i++) arr[i] = 1;
    int num = len / size;
    int* recv_arr = new int[num];
    double myScatterStart = MPI_Wtime();
    MyScatter(arr, num, MPI_INT, recv_arr, num, MPI_INT, 0, MPI_COMM_WORLD);
    double myScatterEnd = MPI_Wtime();
    double MPIScatterStart = MPI_Wtime();
    MPI_Scatter(arr, num, MPI_INT, recv_arr, num, MPI_INT, 0, MPI_COMM_WORLD);
    double MPIScatterEnd = MPI_Wtime();
    if (rank == 0) {
        std::cout << std::fixed << std::setprecision(8) << "My_scatter :    "
            << myScatterEnd - myScatterStart << std::endl;
        std::cout << std::fixed << std::setprecision(8) << "MPIscatter :    "
            << MPIScatterEnd - MPIScatterStart << std::endl;
    }
}

TEST(Scatter, Test_time_of_different_scatter_double) {
    int len = 10000;
    double* arr = new double[len];
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int i = 0; i < len; i++) arr[i] = 1.00;
    int num = len / size;
    double* recv_arr = new double[num];
    double myScatterStart = MPI_Wtime();
    MyScatter(arr, num, MPI_DOUBLE, recv_arr, num, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double myScatterEnd = MPI_Wtime();
    double MPIScatterStart = MPI_Wtime();
    MPI_Scatter(arr, num, MPI_DOUBLE, recv_arr, num, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double MPIScatterEnd = MPI_Wtime();
    if (rank == 0) {
        std::cout << std::fixed << std::setprecision(8) << "My_scatter :    "
            << myScatterEnd - myScatterStart << std::endl;
        std::cout << std::fixed << std::setprecision(8) << "MPIscatter :    "
            << MPIScatterEnd - MPIScatterStart << std::endl;
    }
}


TEST(Scatter, Test_time_of_different_scatter_float) {
    int len = 10000;
    float* arr = new float[len];
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int i = 0; i < len; i++) arr[i] = 1.0f;
    int num = len / size;
    float* recv_arr = new float[num];
    double myScatterStart = MPI_Wtime();
    MyScatter(arr, num, MPI_FLOAT, recv_arr, num, MPI_FLOAT, 0, MPI_COMM_WORLD);
    double myScatterEnd = MPI_Wtime();
    double MPIScatterStart = MPI_Wtime();
    MPI_Scatter(arr, num, MPI_FLOAT, recv_arr, num, MPI_FLOAT, 0, MPI_COMM_WORLD);
    double MPIScatterEnd = MPI_Wtime();
    if (rank == 0) {
        std::cout << std::fixed << std::setprecision(8) << "My_scatter :    "
            << myScatterEnd - myScatterStart << std::endl;
        std::cout << std::fixed << std::setprecision(8) << "MPIscatter :    "
            << MPIScatterEnd - MPIScatterStart << std::endl;
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
