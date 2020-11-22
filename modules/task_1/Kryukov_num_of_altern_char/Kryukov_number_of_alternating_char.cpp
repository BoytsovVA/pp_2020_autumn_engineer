// Copyright 2020 Kryukov Sergey
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/Kryukov_num_of_altern_char/Kryukov_number_of_alternating_char.h"

std::vector<int> getRandomVector(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getNUMalternCHAR(std::vector<int> vec) {
    const int size = vec.size();
    int number = 0;
    for (int i = 0; i < size - 1; i++) {
        if (vec[i] < 0 && vec[i + 1] >= 0) {
            number += 1;
        } else if (vec[i] >= 0 && vec[i + 1] < 0) {
            number += 1;
        } else {
            continue;
        }
    }
    return number;
}

int getParallelNUMalternCHAR(std::vector<int> general_vec, int size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = size_vector / size;
    if (rank == 0) {
        for (int process = 1; process < size; process++)
            MPI_Send(&general_vec[0] + process * delta, delta,
                MPI_INT, process, 0, MPI_COMM_WORLD);
    }

    std::vector<int> local_vec(delta);
    if (rank == 0) {
         local_vec = std::vector<int>(general_vec.begin(),
            general_vec.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int final_altern = 0;
    int local_altern = getNUMalternCHAR(local_vec);
    MPI_Op op_code;
    op_code = MPI_SUM;
    MPI_Reduce(&local_altern, &final_altern, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return final_altern;
}
