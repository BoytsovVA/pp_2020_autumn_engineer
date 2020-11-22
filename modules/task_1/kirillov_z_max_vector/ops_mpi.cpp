// Copyright 2020 Kirillov Zahar
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

std::vector<int> makeVector(int x) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(x);
    for (int i = 0; i < x; i++) { vec[i] = gen() % 100; }
    return vec;
}

int locMax(std::vector<int> vec) {
    const int  length = vec.size();
    int curr = vec[0];
    for (int i = 1; i < length; i++) {
        curr = std::max(curr, vec[i]);
    }
    return curr;
}

int parallelMax(std::vector<int> vec, int len_vector) {
    int size, grade;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &grade);
    const int delta = len_vector / size;
    if (grade == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&vec[0] + i * delta, delta,
                MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local(delta);
    if (grade == 0) {
        local = std::vector<int>(vec.begin(), vec.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int outcome = 0;
    int loc_sum = locMax(local);
    MPI_Op op_code;
    op_code = MPI_MAX;
    MPI_Reduce(&loc_sum, &outcome, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return outcome;
}
