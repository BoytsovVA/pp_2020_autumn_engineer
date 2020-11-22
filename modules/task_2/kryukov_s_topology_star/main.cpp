// Copyright 2020 Kryukov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "./topology_star.h"


TEST(topology_star, realy_topology_star) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }

    std::vector<int> nodes;
    MPI_Comm commstar = createTopologyStar(MPI_COMM_WORLD, nodes);
    int status = MPI_UNDEFINED;
    MPI_Topo_test(commstar, &status);
    ASSERT_EQ(status, MPI_GRAPH);
}

TEST(topology_star, topology_star_get_info) {
    std::vector<int> nodes;
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }

    MPI_Comm commstar = createTopologyStar(MPI_COMM_WORLD, nodes);
    int nnodes, nedges;
    MPI_Graphdims_get(commstar, &nnodes, &nedges);
    EXPECT_EQ(nnodes, size);
    EXPECT_EQ(nedges, (size - 1) * 2);
}

TEST(topology_star, num_neighbor) {
    std::vector<int> nodes;
    int size, procrank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }

    MPI_Comm commstar = createTopologyStar(MPI_COMM_WORLD, nodes);
    MPI_Comm_rank(commstar, &procrank);
    int neighbors_num;
    if (procrank == 0) {
        MPI_Graph_neighbors_count(commstar, procrank, &neighbors_num);
        ASSERT_EQ(neighbors_num, size - 1);
    } else {
        MPI_Graph_neighbors_count(commstar, procrank, &neighbors_num);
        ASSERT_EQ(neighbors_num, 1);
    }
}

TEST(topology_star, message_exchange) {
    std::vector<int> nodes;
    int size, procrank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }

    MPI_Comm star = createTopologyStar(MPI_COMM_WORLD, nodes);

    MPI_Comm_rank(star, &procrank);
    int size_vec = 10;
    int source = 0, dest = 1;
    int* sendMessage = new int[size_vec];
    sendMessage = getRandomVector(size_vec);
    int* result_message = SendRecvStar(sendMessage, size_vec, MPI_INT, dest, 0, star, source, procrank);
    if (procrank == dest) {
        for (int i = 0; i < size_vec; i++) {
            ASSERT_EQ(sendMessage[i], result_message[i]);
        }
    }
}

TEST(topology_star, message_exchange1) {
    std::vector<int> nodes;
    int size, procrank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size == 1) {
        ASSERT_EQ(true, true);
        return;
    }

    MPI_Comm star = createTopologyStar(MPI_COMM_WORLD, nodes);

    MPI_Comm_rank(star, &procrank);
    int size_vec = 10;
    int source = 1, dest = 0;
    int* sendMessage = new int[size_vec];
    sendMessage = getRandomVector(size_vec);
    int* result_message = SendRecvStar(sendMessage, size_vec, MPI_INT, dest, 0, star, source, procrank);


    if (procrank == dest) {
        for (int i = 0; i < size_vec; i++) {
            ASSERT_EQ(sendMessage[i], result_message[i]);
        }
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
