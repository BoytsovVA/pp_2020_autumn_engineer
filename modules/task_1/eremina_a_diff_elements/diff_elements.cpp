// Copyright 2020 Eremina Alena
#include <mpi.h>
#include <math.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_1/eremina_a_diff_elements/diff_elements.h"

std::vector<int> getRandomVector(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

void maxDiffElems(int* in, int* out, int* len, MPI_Datatype *dtype) {
    if (abs(in[0] - in[1]) > abs(out[0] - out[1])) {
        out[0] = in[0];
        out[1] = in[1];
    }
}

void getSequentialOperations(std::vector<int> vec, int* elems) {
    int diff = 0;
    for (int i = 0; i < static_cast<int>(vec.size() - 1); i++) {
        if (abs(vec[i] - vec[i + 1]) > diff) {
            diff = abs(vec[i] - vec[i + 1]);
            elems[0] = vec[i];
            elems[1] = vec[i + 1];
        }
    }
}

void getParallelOperations(std::vector<int> global_vec, int count_size_vector, int* max_diff_elems) {
    int ProcNum, ProcRank, delta;
    MPI_Op op;
    int elems[2] = { 0, 0 };

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (count_size_vector < ProcNum) {
        if (ProcRank == 0) {
            getSequentialOperations(global_vec, max_diff_elems);
        }
    } else {
        delta = count_size_vector / ProcNum + 1;
        if (count_size_vector % ProcNum > ProcNum / 2) {
            delta++;
        }

        std::vector<int> local_vec(delta);
        if (ProcRank == 0) {
            int start = count_size_vector - (ProcNum - 1) * (delta - 1) - 1;
            for (int proc = 1; proc < ProcNum; proc++) {
                MPI_Send(&global_vec[start + ((delta - 1) * (proc - 1))], delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
            local_vec = std::vector<int>(global_vec.begin(), global_vec.begin() + start + 1);
        } else {
            MPI_Status status;
            MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }

        getSequentialOperations(local_vec, elems);

        MPI_Op_create(reinterpret_cast<MPI_User_function *>(maxDiffElems), true, &op);
        MPI_Reduce(elems, max_diff_elems, 2, MPI_INT, op, 0, MPI_COMM_WORLD);
    }
}
