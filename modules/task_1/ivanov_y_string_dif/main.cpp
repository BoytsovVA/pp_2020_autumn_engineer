// Copyright 2020 Ivanov Yaroslav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./string_dif.h"


TEST(Parallel_Operations_MPI, Test_Can_Create_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const int size = 100;

    if (rank == 0) {
        ASSERT_NO_THROW(global_str1 = generateString(size));
    }
}

TEST(Parallel_Operations_MPI, Test_Can_Get_Empty_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 0;
    std::string global_str1 = generateString(size);
    std::string global_str2 = generateString(size);

    if (rank == 0) {
        ASSERT_EQ(getLocalDif(global_str1, global_str2, size), 0);
    }
}


TEST(Parallel_Operations_MPI, Test_Can_Get_Local_Operations) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int size = 30;
    std::string global_str1 = generateString(size);
    std::string global_str2 = generateString(size);

    if (rank == 0) {
        ASSERT_NO_THROW(getLocalDif(global_str1, global_str2, size));
    }
}

TEST(Parallel_Operations_MPI, Test_Can_Create_And_Count_Long_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const int size = 10000;
    global_str1 = generateString(size);
    global_str2 = generateString(size);
    if (rank == 0) {
        ASSERT_GE(getLocalDif(global_str1, global_str2, size), 0);
    }
}

TEST(Parallel_Operations_MPI, Test_Two_Strings_Dif) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string global_str1, global_str2;
    const int size = 30;
    global_str1 = generateString(size);
    global_str2 = generateString(size);
    if (rank == 0) {
        ASSERT_GE(getLocalDif(global_str1, global_str2, size), 0);
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
