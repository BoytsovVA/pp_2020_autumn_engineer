// Copyright 2020 Stoicheva Darya
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/stoicheva_d_matrix_max/matrix_max_mpi.h"

void test_with(const size_t rows, const size_t columns) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> matrix;
    if (ProcRank == 0) {
        matrix = create_random_matrix(rows, columns);
        print_matrix(matrix, rows, columns, "[" + std::to_string(ProcRank) + "]");
    }

    std::vector<int> parallel_max =
        get_max_elements_of_rows_parallel(matrix, rows, columns);

    if (ProcRank == 0) {
        std::vector<int> sequential_max =
            get_max_elements_of_rows_sequentional(matrix, rows, columns);
        ASSERT_EQ(sequential_max, parallel_max);
    }
}


TEST(Parallel_Operations_MPI, Test_Empty) {
    test_with(0, 0);
}

TEST(Parallel_Operations_MPI, Test_One_Element) {
    test_with(1, 1);
}

TEST(Parallel_Operations_MPI, Test_One_Row_Ten_Columns) {
    test_with(1, 10);
}

TEST(Parallel_Operations_MPI, Test_Two_Rows_One_Column) {
    test_with(2, 1);
}

TEST(Parallel_Operations_MPI, Test_Ten_Rows_One_Column) {
    test_with(10, 1);
}

TEST(Parallel_Operations_MPI, Test_Ten_Rows_Ten_Column) {
    test_with(10, 10);
}

TEST(Parallel_Operations_MPI, Test_100_Rows_200_Columns) {
  test_with(100, 200);
}

TEST(Parallel_Operations_MPI, Test_1000_Rows_2000_Columns) {
  test_with(1000, 2000);
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
