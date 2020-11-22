  // Copyright 2020 Kamskov Eugene
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./word_count.h"


TEST(Count_Words_MPI, Simple_Count) {
    std::string testStr = "Carpe diem. Seize the day, boys. Make your lives extraordinary.";

    int res = SimpleCount(testStr) + 1;
    ASSERT_EQ(res, 10);
}

TEST(Count_Words_MPI, Par_Count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = "Carpe diem. Seize the day, boys. Make your lives extraordinary.";
    int res = ParCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(res, 10);
    }
}

TEST(Count_Words_MPI, Par_Count_Empty_Begin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = " m o m e n t o m o r i";
    int res = ParCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(res, 11);
    }
}
TEST(Count_Words_MPI, Empty_String_Count) {
    std::string testStr("");
    ASSERT_ANY_THROW(SimpleCount(testStr));
}

TEST(Count_Words_MPI, Par_Count_Rand_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = GenRandString(30);
    int par_count = ParCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(par_count, 30);
    }
}

TEST(Count_Words_MPI, Par_Count_Rand_String_More) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = GenRandString(10000);
    int par_count = ParCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(par_count, 10000);
    }
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    std::string testStr = "Carpe diem. Seize the day, boys. Make your lives extraordinary.";
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
}
