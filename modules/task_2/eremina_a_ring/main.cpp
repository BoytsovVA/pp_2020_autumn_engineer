// Copyright 2020 Eremina Alena
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./ring.h"

TEST(Topology_Ring, Default_Number_Of_Neighboring_Processes_Is_2) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm RingComm;

    /* 0 - 1 - 2 - ... - (ProcNum-1) */
    RingComm = topologyRing(NULL);

    if (ProcRank == 0) {
        for (int proc = 0; proc < ProcNum; proc++) {
            int nneighbors;
            MPI_Graph_neighbors_count(RingComm, proc, &nneighbors);
            ASSERT_EQ(nneighbors, 2);
        }
    }
}

TEST(Topology_Ring, Default_Left_Neighbor_Process_Rank_Is_Correct) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm RingComm;

    /* 0 - 1 - 2 - ... - (ProcNum-1) */
    RingComm = topologyRing(NULL);

    if (ProcRank == 0) {
        for (int rank = 0; rank < ProcNum; rank++) {
            int neighbors[2];
            MPI_Graph_neighbors(RingComm, rank, 2, &neighbors[0]);
            ASSERT_EQ((neighbors[0] + 1) % ProcNum, rank);
        }
    }
}

TEST(Topology_Ring, Default_Right_Neighbor_Process_Rank_Is_Correct) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm RingComm;

    /* 0 - 1 - 2 - ... - (ProcNum-1) */
    RingComm = topologyRing(NULL);

    if (ProcRank == 0) {
        for (int rank = 0; rank < ProcNum; rank++) {
            int neighbors[2];
            MPI_Graph_neighbors(RingComm, rank, 2, &neighbors[0]);
            ASSERT_EQ(neighbors[1], (rank + 1) % ProcNum);
        }
    }
}

TEST(Topology_Ring, Ordered_Number_Of_Neighboring_Processes_Is_2) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm RingComm;

    /* if ProcNum = 10 */
    /* 0 - 2 - 4 - 6 - 8 - 9 - 7 - 5 - 3 - 1 */
    int *order = new int[ProcNum];
    for (int i = 0; i < ProcNum/2; i++) {
        order[i] = i*2;
        order[ProcNum - 1 - i] = i * 2 + 1;
    }
    if (ProcNum % 2 == 1) order[ProcNum / 2] = ProcNum - 1;

    RingComm = topologyRing(order);

    if (ProcRank == 0) {
        for (int proc = 0; proc < ProcNum; proc++) {
            int nneighbors;
            MPI_Graph_neighbors_count(RingComm, proc, &nneighbors);
            ASSERT_EQ(nneighbors, 2);
        }
    }
}

TEST(Topology_Ring, Ordered_Left_Neighbor_Process_Rank_Is_Correct) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm RingComm;

    /* if ProcNum = 10 */
    /* 0 - 2 - 4 - 6 - 8 - 9 - 7 - 5 - 3 - 1 */
    int *order = new int[ProcNum];
    for (int i = 0; i < ProcNum / 2; i++) {
        order[i] = i * 2;
        order[ProcNum - 1 - i] = i * 2 + 1;
    }
    if (ProcNum % 2 == 1) order[ProcNum / 2] = ProcNum - 1;

    RingComm = topologyRing(order);

    if (ProcRank == 0) {
        for (int i = 0; i < ProcNum; i++) {
            int neighbors[2];
            MPI_Graph_neighbors(RingComm, i, 2, &neighbors[0]);
            int left = order[ProcNum - 1];
            if (i != 0) left = order[i - 1];
            ASSERT_EQ(neighbors[0], left);
        }
    }
}

TEST(Topology_Ring, Ordered_Right_Neighbor_Process_Rank_Is_Correct) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm RingComm;

    /* if ProcNum = 10 */
    /* 0 - 2 - 4 - 6 - 8 - 9 - 7 - 5 - 3 - 1 */
    int *order = new int[ProcNum];
    for (int i = 0; i < ProcNum / 2; i++) {
        order[i] = i * 2;
        order[ProcNum - 1 - i] = i * 2 + 1;
    }
    if (ProcNum % 2 == 1) order[ProcNum / 2] = ProcNum - 1;

    RingComm = topologyRing(order);

    if (ProcRank == 0) {
        for (int i = 0; i < ProcNum; i++) {
            int neighbors[2];
            MPI_Graph_neighbors(RingComm, i, 2, &neighbors[0]);
            int right = order[0];
            if (i != ProcNum - 1) right = order[i + 1];
            ASSERT_EQ(neighbors[1], right);
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
