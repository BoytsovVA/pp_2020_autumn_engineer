// Copyright 2020 Kren Polina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include "./grid_torus_topology.h"

TEST(Grid_Torus_Topology, can_create_grid_torus) {
    MPI_Comm comm_torus = getCommTorus(MPI_COMM_WORLD);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ASSERT_TRUE(testGridTorus(comm_torus));
    }
}

TEST(Grid_Torus_Topology, not_cart_comm_has_no_torus_topology) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ASSERT_FALSE(testGridTorus(MPI_COMM_WORLD));
    }
}

TEST(Grid_Torus_Topology, none_period_comm_has_no_torus_topology) {
    MPI_Comm main_comm;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dims[] = { 0, 0 };
    int period[] = { 1, 0 };
    MPI_Dims_create(size, 2, dims);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, 1, &main_comm);

    if (rank == 0) {
        ASSERT_FALSE(testGridTorus(main_comm));
    }
}

TEST(Grid_Torus_Topology, cant_create_with_wrong_size) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm main_comm = getCommTorus(MPI_COMM_WORLD, size + 1);

    if (rank == 0) {
        ASSERT_EQ(main_comm, MPI_COMM_NULL);
    }
}

TEST(Grid_Torus_Topology, shift_works_correctly) {
    MPI_Comm comm_torus = getCommTorus(MPI_COMM_WORLD);

    int rank;
    MPI_Comm_rank(comm_torus, &rank);

    bool flag = testRelation(comm_torus);

    if (rank == 0) {
        ASSERT_TRUE(flag);
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
