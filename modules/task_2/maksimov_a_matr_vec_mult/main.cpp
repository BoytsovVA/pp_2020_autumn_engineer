// Copyright 2020 Maksimov Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>

#include "../../modules/task_2/maksimov_a_matr_vec_mult/matr_vec_mult.h"

#define MAX_GEN 100

TEST(Parallel_Operations_MPI, Can_Generate_Vector) {
    const int vecSize = 40;

    std::vector<int> vec = getRandomVector(vecSize);
    ASSERT_EQ(static_cast<int>(vec.size()), vecSize);
}

TEST(Parallel_Operations_MPI, Test_Negative_VecSize) {
    const int vecSize = -10;

    ASSERT_ANY_THROW(getRandomVector(vecSize););
}

TEST(Parallel_Operations_MPI, Can_Generate_Matrix) {
    const int rows = 20;
    const int columns = 10;

    std::vector<int> matr = getRandomMatrix(rows, columns);
    ASSERT_EQ(static_cast<int>(matr.size()), rows * columns);
}

TEST(Parallel_Operations_MPI, Test_VecSize_Not_Equal_Columns) {
    const int rows = 4;
    const int columns = 5;

    const int vecSize = 14;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matr;
    std::vector<int> vec;
    if (rank == 0) {
        matr = getRandomMatrix(rows, columns);
        vec = getRandomVector(vecSize);
    }
    ASSERT_ANY_THROW(multiplyMatrixByVector(
        matr, rows, columns, vec, vecSize));
    ASSERT_ANY_THROW(multiplyMatrixByVectorNotParall(
        matr, rows, columns, vec, vecSize));
}

TEST(Parallel_Operations_MPI, Test_MultiplyNonParall) {
    const int rows = 3;
    const int columns = 2;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matr;
    std::vector<int> vec;
    if (rank == 0) {
        matr = std::vector<int>(rows * columns);
        matr[0] = 1;
        matr[1] = 2;
        matr[2] = 2;
        matr[3] = 3;
        matr[4] = 3;
        matr[5] = 4;
        vec = std::vector<int>(2);
        vec[0] = 3;
        vec[1] = 4;

        std::vector<int> multiplyNotParall
            = multiplyMatrixByVectorNotParall(matr, rows, columns, vec, columns);
        ASSERT_EQ(11, multiplyNotParall[0]);
        ASSERT_EQ(18, multiplyNotParall[1]);
        ASSERT_EQ(25, multiplyNotParall[2]);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_Tiny) {
    const int rows = 2;
    const int columns = 3;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matr;
    std::vector<int> vec;
    if (rank == 0) {
        matr = getRandomMatrix(rows, columns);
        vec = getRandomVector(columns);
    }
    std::vector<int> multiply = multiplyMatrixByVector(
        matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multiplyMatrixByVectorNotParall(
            matr, rows, columns, vec, columns);

        ASSERT_EQ(multiplyNotParall, multiply);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_Medium) {
    const int rows = 10;
    const int columns = 5;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matr;
    std::vector<int> vec;
    if (rank == 0) {
        matr = getRandomMatrix(rows, columns);
        vec = getRandomVector(columns);
    }
    std::vector<int> multiply = multiplyMatrixByVector(
        matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multiplyMatrixByVectorNotParall(
            matr, rows, columns, vec, columns);

        ASSERT_EQ(multiplyNotParall, multiply);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_Huge) {
    const int rows = 20;
    const int columns = 14;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matr;
    std::vector<int> vec;
    if (rank == 0) {
        matr = getRandomMatrix(rows, columns);
        vec = getRandomVector(columns);
    }
    std::vector<int> multiply = multiplyMatrixByVector(
        matr, rows, columns, vec, columns);
    if (rank == 0) {
        std::vector<int> multiplyNotParall = multiplyMatrixByVectorNotParall(
            matr, rows, columns, vec, columns);

        ASSERT_EQ(multiplyNotParall, multiply);
    }
}

int main(int argc, char* argv[]) {
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
