// Copyright 2020 Kucheryavykh Yan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/kucheryavykh_y_matrix_column_sum/matrix_columns_sum.h"

TEST(MATRIX_COLUMNS_SUM, can_throw_if_create_wrong_matrix) {
    EXPECT_ANY_THROW(std::vector <int> matrix = createRandomMatrix(-1, -1));
}

TEST(MATRIX_COLUMNS_SUM, can_get_sequintial_columns_sum_matrix2x2) {
    std::vector<int> matrix(4);
    matrix = { 1, 2, 3, 4 };
    std::vector<int> sum = getSequintialColumnsSum(matrix, 2, 2);
    std::vector<int> res(2);
    res = { 3, 7 };
    ASSERT_EQ(res, sum);
}

TEST(MATRIX_COLUMNS_SUM, can_get_sum_of_square_matrix2x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix(4);
    matrix = { 1, 2, 3, 4 };
    std::vector<int> par_sum = getParallelColumnsSum(matrix, 2, 2);
    std::vector<int> seq_sum = getSequintialColumnsSum(matrix, 2, 2);
    if (rank == 0) {
        ASSERT_EQ(seq_sum, par_sum);
    }
}

TEST(MATRIX_COLUMNS_SUM, can_get_sum_of_square_matrix4x4_random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> matrix;
    int rows = 4;
    int cols = 4;
    matrix = createRandomMatrix(rows, cols);
    std::vector<int> par_sum = getParallelColumnsSum(matrix, rows, cols);
    std::vector<int> seq_sum = getSequintialColumnsSum(matrix, rows, cols);
    if (rank == 0) {
        ASSERT_EQ(seq_sum, par_sum);
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
