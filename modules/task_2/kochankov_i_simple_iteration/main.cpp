// Copyright 2020 Kochankov Ilya

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>

#include "iostream"
#include "vector"
#include "../../../modules/task_2/kochankov_i_simple_iteration/Matrix.h"
#include "../../../modules/task_2/kochankov_i_simple_iteration/SimpleIterations.h"

TEST(Linear_simple_method_works, Matrix_1x1) {
    Matrix matrix;
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(1, 2);
        matrix[0][0] = 5;
        matrix[0][1] = 5;

        eps = 0.0001;

        auto result = linear_simple_iteration(matrix, eps);

        EXPECT_TRUE((result[0] - 1) * (result[0] - 1) < eps);
    }
}

TEST(Linear_simple_method_works, Matrix_2x2) {
    Matrix matrix;
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(2, 3);
        matrix[0][0] = 2;
        matrix[0][1] = 1;
        matrix[0][2] = 3;

        matrix[1][0] = 2;
        matrix[1][1] = 4;
        matrix[1][2] = 12;

        eps = 0.1;

        auto result = linear_simple_iteration(matrix, eps);

        EXPECT_TRUE((result[0] - 0) * (result[0] - 0) < eps);
        EXPECT_TRUE((result[1] - 3) * (result[1] - 3) < eps);
    }
}

TEST(Linear_simple_method_works, Matrix_3x3) {
    Matrix matrix;
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(3, 4);
        matrix[0][0] = 4.5;
        matrix[0][1] = -1.7;
        matrix[0][2] = 3.5;
        matrix[0][3] = 2;

        matrix[1][0] = 3.1;
        matrix[1][1] = 2.3;
        matrix[1][2] = -1.1;
        matrix[1][3] = 1;

        matrix[2][0] = 1.8;
        matrix[2][1] = 2.5;
        matrix[2][2] = 4.7;
        matrix[2][3] = 4;

        eps = 0.00001;

        auto result = linear_simple_iteration(matrix, eps);

        EXPECT_TRUE((result[0] - 0.188038) * (result[0] - 0.188038) < eps);
        EXPECT_TRUE((result[1] - 0.441589) * (result[1] - 0.441589) < eps);
        EXPECT_TRUE((result[2] - 0.544157) * (result[2] - 0.544157) < eps);
    }
}

TEST(Linear_simple_method_works, Wrong_matrix) {
    Matrix matrix;
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(3, 4);
        matrix[0][0] = 0;
        matrix[0][1] = -1.7;
        matrix[0][2] = 3.5;
        matrix[0][3] = 2;

        matrix[1][0] = 3.1;
        matrix[1][1] = 0;
        matrix[1][2] = -1.1;
        matrix[1][3] = 1;

        matrix[2][0] = 1.8;
        matrix[2][1] = 2.5;
        matrix[2][2] = 0;
        matrix[2][3] = 4;

        eps = 0.00001;

        EXPECT_ANY_THROW(linear_simple_iteration(matrix, eps));
    }
}

TEST(Parallel_simple_method_works, Matrix_1x1) {
    Matrix matrix;
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(1, 2);
        matrix[0][0] = 5;
        matrix[0][1] = 5;
    }
    eps = 0.0001;

    auto result = parallel_simple_iteration(matrix, eps);

    if (rank == 0) {
        EXPECT_TRUE((result[0] - 1) * (result[0] - 1) < eps * eps);
    }
}

TEST(Parallel_simple_method_works, Matrix_2x2) {
    Matrix matrix(2, 3);
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(2, 3);
        matrix[0][0] = 2;
        matrix[0][1] = 1;
        matrix[0][2] = 3;

        matrix[1][0] = 2;
        matrix[1][1] = 4;
        matrix[1][2] = 12;
    }
    eps = 0.1;

    auto result = parallel_simple_iteration(matrix, eps);

    if (rank == 0) {
        EXPECT_TRUE((result[0] - 0) * (result[0] - 0) < eps);
        EXPECT_TRUE((result[1] - 3) * (result[1] - 3) < eps);
    }
}

TEST(Parallel_simple_method_works, Matrix_3x3) {
    Matrix matrix(3, 4);
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(3, 4);
        matrix[0][0] = 4.5;
        matrix[0][1] = -1.7;
        matrix[0][2] = 3.5;
        matrix[0][3] = 2;

        matrix[1][0] = 3.1;
        matrix[1][1] = 2.3;
        matrix[1][2] = -1.1;
        matrix[1][3] = 1;

        matrix[2][0] = 1.8;
        matrix[2][1] = 2.5;
        matrix[2][2] = 4.7;
        matrix[2][3] = 4;
    }
    eps = 0.00001;

    auto result = parallel_simple_iteration(matrix, eps);

    if (rank == 0) {
        EXPECT_TRUE((result[0] - 0.188038) * (result[0] - 0.188038) < eps);
        EXPECT_TRUE((result[1] - 0.441589) * (result[1] - 0.441589) < eps);
        EXPECT_TRUE((result[2] - 0.544157) * (result[2] - 0.544157) < eps);
    }
}

TEST(Parallel_simple_method_works, Wrong_matrix) {
    Matrix matrix(3, 4);
    double eps = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = Matrix(3, 4);
        matrix[0][0] = 0;
        matrix[0][1] = -1.7;
        matrix[0][2] = 3.5;
        matrix[0][3] = 2;

        matrix[1][0] = 3.1;
        matrix[1][1] = 0;
        matrix[1][2] = -1.1;
        matrix[1][3] = 1;

        matrix[2][0] = 1.8;
        matrix[2][1] = 2.5;
        matrix[2][2] = 0;
        matrix[2][3] = 4;
    }
    eps = 0.00001;
    EXPECT_ANY_THROW(parallel_simple_iteration(matrix, eps));
}

TEST(Parallel_simple_method_works, random_matrix) {
    Matrix matrix(100, 101);
    double eps = 0.001;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = get_rand_matrix(100, 101);
    }

    auto result = parallel_simple_iteration(matrix, eps);

    if (rank == 0) {
        auto lin_result = linear_simple_iteration(matrix, eps);
        EXPECT_EQ(lin_result, result);
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
