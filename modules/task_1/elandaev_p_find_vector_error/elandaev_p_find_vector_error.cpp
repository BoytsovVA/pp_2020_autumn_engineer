// Copyright 2020 Elandaev Pavel

#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/elandaev_p_find_vector_error/elandaev_p_find_vector_error.h"


std::vector<int> getRandomVector(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int getSequentialOperations(std::vector<int> vec, int sz) {
    if (sz == 0 || sz == 1) return 0;

    int count_error = 0;
    for (int i = 1; i < sz; i++) {
        if (vec[i] <= vec[i-1]) {
            count_error++;
        }
    }
    return count_error;
}

int getParallelOperations(std::vector<int> global_vec,
                          int count_size_vector) {
    if (count_size_vector == 0 || count_size_vector == 1)
        return 0;
    int size, rank;
    int Global_error = 0;
    int Local_error = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = count_size_vector / size;
    int remainded = count_size_vector % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&global_vec[0] + proc * delta + remainded - 1, delta + 1,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        std::vector<int> local_vec(delta + remainded);
        local_vec = std::vector<int>(global_vec.begin(),
                                     global_vec.begin() + delta + remainded);
        Local_error = getSequentialOperations(local_vec, static_cast<int>(local_vec.size()));
    } else {
        std::vector<int> local_vec(delta + 1);
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        Local_error = getSequentialOperations(local_vec, static_cast<int>(local_vec.size()));
    }

    MPI_Reduce(&Local_error, &Global_error, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return Global_error;
}
