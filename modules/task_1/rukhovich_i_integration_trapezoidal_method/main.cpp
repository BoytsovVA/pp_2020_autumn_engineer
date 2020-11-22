// Copyright 2020 Igor Rukhovich
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include "./integration_trapezoidal_method.h"

double epsillon = 1e-8;
double precision = 2e-3;
double strong_precision = 1e-4;

// y = 0 forall x in [0, 10]. int_0^10 y = 0.
TEST(ParallelIntegrationTrapMeth, test_zero) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        auto poly = std::make_shared<QuadraticPolynomial>(0., 0., 0.);
        double integral = getIntegralSequentional(poly, 0., 10., precision);
        ASSERT_NEAR(integral, 0., epsillon);
    }
}

// y = 1 forall x in [0, 10]. int_0^10 y = 10.
TEST(ParallelIntegrationTrapMeth, test_simple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        auto poly = std::make_shared<QuadraticPolynomial>(0., 0., 1.);
        double integral = getIntegralSequentional(poly, 0., 10., precision);
        ASSERT_NEAR(integral, 10., epsillon);
    }
}

// y = -0.5x + 2 forall x in [0, 4]. int_0^4 y = 4.
TEST(ParallelIntegrationTrapMeth, test_linear_seq) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        auto poly = std::make_shared<QuadraticPolynomial>(0., -0.5, 2.);
        double integral = getIntegralSequentional(poly, 0., 4., precision);
        ASSERT_NEAR(integral, 4., epsillon);
    }
}

// y = 0 forall x in [0, 10]. int_0^10 y = 0.
TEST(ParallelIntegrationTrapMeth, test_zero_parallel) {
    auto poly = std::make_shared<QuadraticPolynomial>(0., 0., 0.);
    double par_ans = getIntegralParallel(poly, 0., 10., precision);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double seq_ans = getIntegralSequentional(poly, 0., 10., precision);
        ASSERT_NEAR(seq_ans, par_ans, epsillon);
    }
}

// y = 1 forall x in [0, 15]. int_0^15 y = 15.
TEST(ParallelIntegrationTrapMeth, test_simple_parallel) {
    auto poly = std::make_shared<QuadraticPolynomial>(0., 0., 1.);
    double par_ans = getIntegralParallel(poly, 0., 15., precision);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double seq_ans = getIntegralSequentional(poly, 0., 15., precision);
        ASSERT_NEAR(seq_ans, par_ans, epsillon);
    }
}

// y = -0.5x + 2 forall x in [0, 4]. int_0^4 y = 4.
TEST(ParallelIntegrationTrapMeth, test_linear_parallel) {
    auto poly = std::make_shared<QuadraticPolynomial>(0., -0.5, 2.);
    double par_ans = getIntegralParallel(poly, 0., 4., precision);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double seq_ans = getIntegralSequentional(poly, 0., 4., precision);
        ASSERT_NEAR(seq_ans, par_ans, epsillon);
    }
}

// y = -5x^2 + 25x + 10 forall x in [0, 5]
TEST(ParallelIntegrationTrapMeth, test_quadratic_parallel) {
    auto poly = std::make_shared<QuadraticPolynomial>(-5., 25., 10.);
    double par_ans = getIntegralParallel(poly, 0., 5., precision);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double seq_ans = getIntegralSequentional(poly, 0., 5., precision);
        ASSERT_NEAR(seq_ans, par_ans, epsillon);
    }
}

// y = -5x^2 + 25x + 10 forall x in [0, 5]
TEST(ParallelIntegrationTrapMeth, test_quadratic_parallel_strong) {
    auto poly = std::make_shared<QuadraticPolynomial>(-5., 25., 10.);
    double par_ans = getIntegralParallel(poly, 0., 5., strong_precision);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double seq_ans = getIntegralSequentional(poly, 0., 5., strong_precision);
        ASSERT_NEAR(seq_ans, par_ans, epsillon);
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
