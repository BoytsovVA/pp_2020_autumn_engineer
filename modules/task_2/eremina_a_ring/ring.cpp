// Copyright 2018 Eremina Alena
#include "../../../modules/task_2/eremina_a_ring/ring.h"

MPI_Comm topologyRing(int* order) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int *index = new int[ProcNum];
    for (int proc = 0; proc < ProcNum; proc++) {
        index[proc] = 2 * (proc + 1);
    }

    int *edges = new int[ProcNum * 2];
    if (order != NULL) {
        for (int i = 0; i < ProcNum; i++) {
            edges[(2 + (i * 2)) % (ProcNum * 2)] = order[i];
            edges[(ProcNum * 2 - 1 + i * 2) % (ProcNum * 2)] = order[i];
        }
    } else {
        for (int i = 0; i < ProcNum; i++) {
            edges[(2 + (i * 2)) % (ProcNum * 2)] = i;
            edges[(ProcNum * 2 - 1 + i * 2) % (ProcNum * 2)] = i;
        }
    }

    MPI_Comm RingComm;
    MPI_Graph_create(MPI_COMM_WORLD, ProcNum, index, edges, 1, &RingComm);

    return RingComm;
}
