// Copyright 2020 Sozinov Alex
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./max_row_matrix.h"


TEST(Parallel_Operations_MPI, find_max_from_myself_matrix) {
    int sizeRow = 5;
    int countRow = 4;
    int ProcessRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    std::vector<int> vec = {23,  10, -30, 32, -100,
                            100, 21, -10, 20, 100,
                            69,  29, 10,  20, 1,
                            8,   40, -12, 41, -99};
    std::vector<int> res{32, 100, 69, 41};
    std::vector<int> vecSeq = GetSequentialMax(vec, sizeRow, countRow);
    std::vector<int> vecPar = GetParalMax(vec, sizeRow, countRow);
    if (ProcessRank == 0) {
        ASSERT_EQ(res, vecSeq);
        ASSERT_EQ(res, vecPar);
    }
}

TEST(Parallel_Operations_MPI, find_max) {
    int sizeRow = 51;
    int countRow = 20;
    std::vector<int> vec = GenerateMatrix(sizeRow, countRow, -1, 121);

    std::vector<int> vecSeq = GetSequentialMax(vec, sizeRow, countRow);
    std::vector<int> vecPar = GetParalMax(vec, sizeRow, countRow);
    int ProcessRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    if (ProcessRank == 0) {
        ASSERT_EQ(vecPar, vecSeq);
     }
}

TEST(Parallel_Operations_MPI, find_max_2) {
    int sizeRow = 26;
    int countRow = 21;
    std::vector<int> vec = GenerateMatrix(sizeRow, countRow, -100, 121);

    std::vector<int> vecSeq = GetSequentialMax(vec, sizeRow, countRow);
    std::vector<int> vecPar = GetParalMax(vec, sizeRow, countRow);

    int ProcessRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    if (ProcessRank == 0) {
        ASSERT_EQ(vecPar, vecSeq);
    }
}

TEST(Parallel_Operations_MPI, find_max_3) {
    int sizeRow = 111;
    int countRow = 14;
    std::vector<int> vec = GenerateMatrix(sizeRow, countRow, -221, 121);
    std::vector<int> vecSeq = GetSequentialMax(vec, sizeRow, countRow);
    std::vector<int> vecPar = GetParalMax(vec, sizeRow, countRow);
    int ProcessRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    if (ProcessRank == 0) {
        ASSERT_EQ(vecPar, vecSeq);
    }
}

TEST(Parallel_Operations_MPI, find_max_4) {
    int sizeRow = 235;
    int countRow = 224;
    std::vector<int> vec = GenerateMatrix(sizeRow, countRow, -100, 100);
    std::vector<int> vecSeq = GetSequentialMax(vec, sizeRow, countRow);
    std::vector<int> vecPar = GetParalMax(vec, sizeRow, countRow);
    int ProcessRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    if (ProcessRank == 0) {
        ASSERT_EQ(vecPar, vecSeq);
    }
}

TEST(Parallel_Operations_MPI, find_max_5) {
    int sizeRow = 123;
    int countRow = 11;
    std::vector<int> vec = GenerateMatrix(sizeRow, countRow, -100, 100);
    std::vector<int> vecSeq = GetSequentialMax(vec, sizeRow, countRow);
    std::vector<int> vecPar = GetParalMax(vec, sizeRow, countRow);
    int ProcessRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);
    if (ProcessRank == 0) {
        ASSERT_EQ(vecPar, vecSeq);
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
