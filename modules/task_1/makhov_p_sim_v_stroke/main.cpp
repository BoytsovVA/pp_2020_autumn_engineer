// Copyright 2020 Makhov Pavel
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "../../../modules/task_1/makhov_p_sim_v_stroke/sim_v_stroke.h"


TEST(Parallel_Operations_MPI, create_random_string_small) {
    std::string gen_str;
    const int string_size = 2;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        ASSERT_NO_THROW(gen_str = RanStr(string_size));
    }
}

TEST(Parallel_Operations_MPI, test_string_big) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string gen_str;
    const int string_size = 100000;
    if (rank == 0) {
        gen_str = RanStr(string_size);
    }
    int gen_sum = FrequencyParallel(gen_str, 'z', string_size);
    if (rank == 0) {
        int reference_sum = FrequencySec(gen_str, 'z', string_size);
        ASSERT_EQ(reference_sum, gen_sum);
    }
}

TEST(Parallel_Operations_MPI, create_random_string) {
    std::string gen_str;
    const int string_size = 1111;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        ASSERT_NO_THROW(gen_str = RanStr(string_size));
    }
}

TEST(Parallel_Operations_MPI, create_string_with_wrong) {
    std::string gen_str;
    const int string_size = -20;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_rank == 0) {
        ASSERT_ANY_THROW(gen_str = RanStr(string_size));
    }
}

TEST(Parallel_Operations_MPI, test_comparison) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string gen_str;
    const int string_size = 100;
    if (rank == 0) {
        gen_str = RanStr(string_size);
    }
    int gen_sum = FrequencyParallel(gen_str, 'x', string_size);
    if (rank == 0) {
        int reference_sum = FrequencySec(gen_str, 'x', string_size);
        ASSERT_EQ(reference_sum, gen_sum);
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

