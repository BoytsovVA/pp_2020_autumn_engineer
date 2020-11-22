// Copyright 2020 Gorbunova Valeria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <string>
#include "./lexicograph.h"

TEST(Lexicograph_ordered, string_generation) {
    std::string s;
    ASSERT_NO_THROW(s = generateString(20));
}

TEST(lexicograph_ordered, orderd_strings_of_different_length) {
    std::string s1 = "112233AABBCCaabbcc", s2 = "1Aa";
    ASSERT_EQ(getParallelOperation(s1, s2), true);
}

TEST(lexicograph_ordered, orderd_and_unordered_strings_of_different_length) {
    std::string s1 = "dqbA3dvkdns14", s2 = "1Aabbd";
    ASSERT_EQ(getParallelOperation(s1, s2), false);
}

TEST(lexicograph_ordered, unordered_strings_of_different_length) {
    std::string s1 = "dnw2F", s2 = "MDBECvscta63DGvschB3DVXCX";
    ASSERT_EQ(getParallelOperation(s1, s2), false);
}

TEST(lexicograph_ordered, orderd_strings_of_same_length) {
    std::string s1 = "569AMNv", s2 = "568AKNv";
    ASSERT_EQ(getParallelOperation(s1, s2), true);
}

TEST(lexicograph_ordered, orderd_and_unordered_strings_of_same_length) {
    std::string s1 = "ABXC2", s2 = "ABCDX";
    ASSERT_EQ(getParallelOperation(s1, s2), false);
}

TEST(lexicograph_ordered, unordered_strings_of_same_length) {
    std::string s1 = "Nhwbgv4bsi3a", s2 = "skdh2b4bfh12k";
    ASSERT_EQ(getParallelOperation(s1, s2), false);
}

TEST(lexicograph_orderd, two_generating_strings) {
    std::string s1;
    std::string s2;
    s1 = generateString(1001);
    s2 = generateString(1002);
    getParallelOperation(s1, s2);
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
