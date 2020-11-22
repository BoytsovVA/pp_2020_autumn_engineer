// Copyright 2020 Andrey Avdeev
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cmath>
#include "./rectangle_method.h"

TEST(Parallel_Operations_MPI, Sequential_Test) {
    auto func = [](double _x) -> double {
        return 5;
    };

    double error = pow(10.0, -8);
    double seq_sum = getSequentialIntegration(func, 0, 5, 10);
    ASSERT_NEAR(25, seq_sum, error);
}

TEST(Parallel_Operations_MPI, Parallel_Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [](double _x) -> double {
        return 5;
    };

    double par_sum = getParallelIntegration(func, 0, 5, 10);

    if (rank == 0) {
        double error = pow(10.0, -8);
        ASSERT_NEAR(25, par_sum, error);
    }
}

TEST(Parallel_Operations_MPI, Sequental_Border_Test) {
    auto func = [](double _x) -> double {
        return _x;
    };
    ASSERT_ANY_THROW(getSequentialIntegration(func, 10, 5, 10));
}

TEST(Parallel_Operations_MPI, Parallel_Border_Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [](double _x) -> double {
        return _x;
    };

    if (rank == 0) {
        ASSERT_ANY_THROW(getParallelIntegration(func, 10, 5, 10));
    }
}

TEST(Parallel_Operations_MPI, Sequential_Segments_Test) {
    auto func = [](double _x) -> double {
        return _x;
    };
    ASSERT_ANY_THROW(getSequentialIntegration(func, 0, 5, 0));
}

TEST(Parallel_Operations_MPI, Parallel_Segments_Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [](double _x) -> double {
        return _x;
    };
    if (rank == 0) {
    ASSERT_ANY_THROW(getParallelIntegration(func, 0, 5, 0));
    }
}


TEST(Parallel_Operations_MPI, Equality_Time_Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto func = [](double _x) -> double {
        return _x * _x + _x;
    };

    double time = MPI_Wtime();
    double par_sum = getParallelIntegration(func, 0, 50, 100);
    double par_time = MPI_Wtime() - time;

    if (rank == 0) {
        double error = pow(10.0, -8);

        time = MPI_Wtime();
        double seq_sum = getSequentialIntegration(func, 0, 50, 100);
        double seq_time = MPI_Wtime() - time;

        std::cout << "Parallel time: " << par_time << std::endl;
        std::cout << "Sequential time: " << seq_time << std::endl;

        ASSERT_NEAR(seq_sum, par_sum, error);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::cout << rank;

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
