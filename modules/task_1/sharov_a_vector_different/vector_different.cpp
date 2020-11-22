// Copyright 2020 Sharov Alexander
#include <mpi.h>
#include <math.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_1/sharov_a_vector_different/vector_different.h"


std::vector<int> getRandomVector(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

void Different(int* first, int* second, int* lenght, MPI_Datatype* dtype) {
    if (abs(first[0] - first[1]) > abs(second[0] - second[1])) {
        second[0] = first[0];
        second[1] = first[1];
    }
}

void getSequentialOperations(std::vector<int> vector, int* elems) {
    int diff = 0;
    for (int i = 0; i < static_cast<int>(vector.size() - 1); i++) {
        if (abs(vector[i] - vector[i + 1]) > diff) {
            diff = abs(vector[i] - vector[i + 1]);
            elems[0] = vector[i];
            elems[1] = vector[i + 1];
        }
    }
}


void getParallelOperations(std::vector<int> global_vec, int vector_size, int* diff_elem) {
    int ProcNum, ProcRank, delta;
    MPI_Op op;
    int elems[2] = { 0, 0 };

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (vector_size < ProcNum) {
        if (ProcRank == 0) {
            getSequentialOperations(global_vec, diff_elem);
        }
    } else {
        delta = vector_size / ProcNum + 1;
        if (vector_size % ProcNum > ProcNum / 2) {
            delta++;
        }

        std::vector<int> local_vec(delta);
        if (ProcRank == 0) {
            int start = vector_size - (ProcNum - 1) * (delta - 1) - 1;
            for (int proc = 1; proc < ProcNum; proc++) {
                MPI_Send(&global_vec[start + ((delta - 1) * (proc - 1))], delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
            }
            local_vec = std::vector<int>(global_vec.begin(), global_vec.begin() + start + 1);
        } else {
            MPI_Status status;
            MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }

        getSequentialOperations(local_vec, elems);

        MPI_Op_create(reinterpret_cast<MPI_User_function*>(Different), true, &op);
        MPI_Reduce(elems, diff_elem, 2, MPI_INT, op, 0, MPI_COMM_WORLD);
    }
}
