// Copyright 2020 Kartashev Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./lexical_order.h"


TEST(Parallel_Operations_MPI, parallel_works) {
    // Need for windos git action
    char* str_1 = new char[6];
    char* str_2 = new char[6];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        str_1[0] = 'a';
        str_1[1] = 'a';
        str_1[2] = 'a';
        str_1[3] = 'a';
        str_1[4] = 'a';
        str_1[5] = '\0';

        str_2[0] = 'b';
        str_2[1] = 'b';
        str_2[2] = 'b';
        str_2[3] = 'b';
        str_2[4] = 'b';
        str_2[5] = '\0';
    }
    int parallel_result = get_parallel_operations(str_1, str_2, 6, 6);
    if (rank == 0) {
        EXPECT_EQ(parallel_result, 2);
    }
    delete [] str_1;
    delete [] str_2;
}

TEST(Parallel_Operations_MPI, sequential_operations_works) {
    char * str_1, * str_2;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        str_1 = new char[2];
        str_1[0] = 'a';
        str_1[1] = 'b';

        str_2 = new char[3];
        str_2[0] = 'a';
        str_2[1] = 'b';
        str_2[1] = 'c';

        auto result = get_sequential_operations(str_1, str_2, 2, 3);

        EXPECT_EQ(result, 2);
        delete [] str_1;
        delete [] str_2;
    }
}

TEST(Parallel_Operations_MPI, sequential_operations_false_work) {
    char * str_1, * str_2;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        str_1 = new char[2];
        str_1[0] = 'b';
        str_1[1] = 'c';

        str_2 = new char[2];
        str_2[0] = 'b';
        str_2[1] = 'b';

        auto result = get_sequential_operations(str_1, str_2, 2, 2);

        EXPECT_EQ(result, 0);
        delete [] str_1;
        delete [] str_2;
    }
}

TEST(Parallel_Operations_MPI, sequential_operations_equal_work) {
    char * str_1, * str_2;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        str_1 = new char[2];
        str_1[0] = 'b';
        str_1[1] = 'c';

        str_2 = new char[2];
        str_2[0] = 'b';
        str_2[1] = 'c';

        auto result = get_sequential_operations(str_1, str_2, 2, 2);

        EXPECT_EQ(result, 1);
        delete [] str_1;
        delete [] str_2;
    }
}

TEST(Parallel_Operations_MPI, parallel_str_1_length) {
    char * str_1 = new char[2];
    char * str_2 = new char[2];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        str_1[0] = 'd';
        str_1[1] = '\0';

        str_2[0] = 'a';
        str_2[1] = '\0';
    }

    auto parallel_result = get_parallel_operations(str_1, str_2, 1, 1);

    if (rank == 0) {
        EXPECT_EQ(parallel_result, 0);
    }

    delete [] str_1;
    delete [] str_2;
}

TEST(Parallel_Operations_MPI, get_rand_string_works) {
    int lenght = 100;
    char * str;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_NO_THROW(str = get_rand_string(lenght));
    }
    str = nullptr;
    delete [] str;
}

TEST(Parallel_Operations_MPI, get_rand_str_negative_size) {
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        EXPECT_ANY_THROW(get_rand_string(-5));
    }
}

TEST(Parallel_Operations_MPI, parallel_works_random_str_equal_size) {
    // Need for windows git action
    int length = 100000;
    char * str_1 = new char[length];
    char * str_2 = new char[length];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        delete [] str_1;
        delete [] str_2;
        str_1 = get_rand_string(length);
        str_2 = get_rand_string(length);
    }

    auto parallel_result = get_parallel_operations(str_1, str_2, length, length);

    if (rank == 0) {
        auto sequential_result = get_sequential_operations(str_1, str_2, length, length);
        EXPECT_EQ(parallel_result, sequential_result);
    }

    delete [] str_1;
    delete [] str_2;
}

TEST(Parallel_Operations_MPI, parallel_works_random_str_not_equal_size) {
    // Need for windows git action
    int length_1 = 100000;
    int length_2 = 10000;
    char * str_1 = new char[length_1];
    char * str_2 = new char[length_2];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        delete [] str_1;
        delete [] str_2;
        str_1 = get_rand_string(length_1);
        str_2 = get_rand_string(length_2);
    }

    auto parallel_result = get_parallel_operations(str_1, str_2, length_1, length_2);

    if (rank == 0) {
        auto sequential_result = get_sequential_operations(str_1, str_2, length_1, length_2);
        EXPECT_EQ(parallel_result, sequential_result);
    }

    delete [] str_1;
    delete [] str_2;
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
