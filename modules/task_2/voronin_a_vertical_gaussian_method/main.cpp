// Copyright 2020 Voronin Aleksey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "./vertical_gaussian_method.h"

TEST(Parallel_Operations_MPI, can_get_random_linear_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        const int rows = 3;
        ASSERT_NO_THROW(getRandomMatrixLinear(rows));
    }
}

TEST(Parallel_Operations_MPI, can_get_result_with_random_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        const int rows = 5;
        std::vector<double> sample_matrix = getRandomMatrixLinear(rows);
        ASSERT_NO_THROW(sequentialGaussianMethod(sample_matrix, rows));
    }
}

TEST(Parallel_Operations_MPI, can_get_result_with_sequential_version_three_unknowns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        const int rows = 3;
        std::vector<double> sample_matrix = {2, 1, 1, 2,
                                            1, -1, 0, -2,
                                            3, -1, 2, 2};
        std::vector<double> result = sequentialGaussianMethod(sample_matrix, rows);
        std::vector<double> expectedResult = {-1, 1, 3};
        ASSERT_EQ(result, expectedResult);
    }
}

TEST(Parallel_Operations_MPI, can_get_result_with_sequential_version_two_unknowns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        const int rows = 2;
        std::vector<double> sample_matrix = {1, -1, -5,
                                            2, 1, -7};
        std::vector<double> result = sequentialGaussianMethod(sample_matrix, rows);
        std::vector<double> expectedResult = {-4, 1};
        ASSERT_EQ(result, expectedResult);
    }
}

TEST(Parallel_Operations_MPI, cant_get_result_with_wrong_input) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        const int rows = 4;
        std::vector<double> sample_matrix = {1, -1, -5,
                                            2, 1, -7};
        std::vector<double> result = sequentialGaussianMethod(sample_matrix, rows);
        ASSERT_EQ(result.size(), (unsigned int) 0);
    }
}

TEST(Parallel_Operations_MPI, cant_get_result_with_negative_input) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        const int rows = -1;
        std::vector<double> sample_matrix = {1, -1, -5,
                                            2, 1, -7};
        std::vector<double> result = sequentialGaussianMethod(sample_matrix, rows);
        ASSERT_EQ(result.size(), (unsigned int) 0);
    }
}

TEST(Parallel_Operations_MPI, can_get_result_with_parallel_version_two_unknowns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int rows = 2;
    std::vector<double> sample_matrix = {1, -1, -5,
                                        2, 1, -7};
    std::vector<double> values = parallelGaussianMethod(sample_matrix, rows);
    if (rank == 0) {
        std::vector<double> expectedResult = {-4, 1};
        ASSERT_EQ(values, expectedResult);
    }
}

TEST(Parallel_Operations_MPI, can_get_result_with_parallel_version_three_unknowns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int rows = 3;
        std::vector<double> sample_matrix = {2, 1, 1, 2,
                                            1, -1, 0, -2,
                                            3, -1, 2, 2};
    std::vector<double> result = parallelGaussianMethod(sample_matrix, rows);
    if (rank == 0) {
        std::vector<double> expectedResult = {-1, 1, 3};
        for (size_t i = 0; i < 3; i++) {
            ASSERT_NEAR(result[i], expectedResult[i], 0.1);
        }
    }
}

TEST(Parallel_Operations_MPI, can_get_result_with_parallel_version_random_matrix_three_unknowns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int rows = 3;
    std::vector<double> sample_matrix = getRandomMatrixLinear(rows);
    ASSERT_NO_THROW(parallelGaussianMethod(sample_matrix, rows));
}

TEST(Parallel_Operations_MPI, can_compare_result_with_parallel_version_random_matrix_three_unknowns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int rows = 3;
        std::vector<double> sample_matrix = getRandomMatrixLinear(rows);
    std::vector<double> values = parallelGaussianMethod(sample_matrix, rows);
    if (rank == 0) {
        std::vector<double> expectedResult = calculateResults(sample_matrix, rows, values);
        for (size_t i = 0; i < rows; i++) {
            if (std::isnan(expectedResult[i])) {
                break;
            }
            ASSERT_NEAR(sample_matrix[(rows+1)*i+rows], expectedResult[i], 0.1);
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
