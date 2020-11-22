  // Copyright 2020 Zlobin George
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./method_rectangles.h"

TEST(Parallel_Operations_MPI, Test_Sequential_Is_Correct) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        auto func = [] (double _x) -> double {
                return 2;
            };

        double error = pow(10.0, -8);
        double seq_sum = getSequentialIntegration(func, 0, 2, 10);
        ASSERT_NEAR(4, seq_sum, error);
    }
}

TEST(Parallel_Operations_MPI, Test_Parallel_Is_Correct) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [] (double _x) -> double {
            return 2;
        };

    double par_sum = getParallelIntegration(func, 0, 2, 10);

    if (rank == 0) {
        double error = pow(10.0, -8);
        ASSERT_NEAR(4, par_sum, error);
    }
}

class Test_Different_Submethods :public ::testing::TestWithParam<Methods> {};

TEST_P(Test_Different_Submethods, Test_Sequential_Submethods) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Methods method = GetParam();

    if (rank == 0) {
        auto func = [] (double _x) -> double {
                return _x;
            };

        double error = pow(10.0, -2);
        double seq_sum = getSequentialIntegration(func, 0, 2, 1000, method);
        ASSERT_NEAR(2, seq_sum, error);
    }
}

TEST_P(Test_Different_Submethods, Test_Parallel_Submethods) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Methods method = GetParam();

    auto func = [] (double _x) -> double {
            return _x;
        };

    double par_sum = getParallelIntegration(func, 0, 2, 1000, method);

    if (rank == 0) {
        double error = pow(10.0, -2);
        ASSERT_NEAR(2, par_sum, error);
    }
}

TEST_P(Test_Different_Submethods, Test_Sequential_And_Parallel_Are_Equival_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Methods method = GetParam();

    auto func = [] (double _x) -> double {
            return _x * _x + 5;
        };

    double par_sum = getParallelIntegration(func, 0, 2, 100, method);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double seq_sum = getSequentialIntegration(func, 0, 2, 100, method);
        ASSERT_NEAR(seq_sum, par_sum, error);
    }
}

TEST_P(Test_Different_Submethods, Test_Sequential_And_Parallel_Are_Equival_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Methods method = GetParam();

    auto func = [] (double _x) -> double {
            return _x * cos(_x) - 8;
        };

    double par_sum = getParallelIntegration(func, 0, 2, 100, method);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double seq_sum = getSequentialIntegration(func, 0, 2, 100, method);
        ASSERT_NEAR(seq_sum, par_sum, error);
    }
}

TEST_P(Test_Different_Submethods, Test_Sequential_And_Parallel_Are_Equival_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Methods method = GetParam();

    auto func = [] (double _x) -> double {
            return pow(0.1, _x) * sin(_x);
        };

    double par_sum = getParallelIntegration(func, 0, 2, 100, method);

    if (rank == 0) {
        double error = pow(10.0, -8);
        double seq_sum = getSequentialIntegration(func, 0, 2, 100, method);
        ASSERT_NEAR(seq_sum, par_sum, error);
    }
}

TEST_P(Test_Different_Submethods, DISABLED_Test_Sequential_And_Parallel_Time_Compare) {
    int rank, size;
    double time, par_time, seq_time;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Methods method = GetParam();

    auto func = [] (double _x) -> double {
            return _x * _x * _x - 3 * _x + 55;
        };

    time = MPI_Wtime();
    double par_sum = getParallelIntegration(func, 0, 20, static_cast<int>(pow(10, 6) ), method);
    par_time = MPI_Wtime() - time;

    if (rank == 0) {
        double error = pow(10.0, -8);

        time = MPI_Wtime();
        double seq_sum = getSequentialIntegration(func, 0, 20, static_cast<int>(pow(10, 6) ), method);
        seq_time = MPI_Wtime() - time;

        std::cout << "Parallel time: " << par_time << std::endl;
        std::cout << "Sequential time: " << seq_time << std::endl;

        ASSERT_NEAR(seq_sum, par_sum, error);
        if (size > 1) {
            ASSERT_TRUE(par_time < seq_time);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
        Parallel_Operations_MPI,
        Test_Different_Submethods,
        ::testing::Values(
                LEFT_RECTANGLES, RIGHT_RECTANGLES, MEAN_RECTANGLES));

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
