// Copyright 2020 Kokh Vladislav
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include <algorithm>


#include "../../../modules/task_1/kokh_v_min_val_matrix/min_val_matrix.h"


int* randomMatrix(int* m, int r, int c) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            m[i * c + j] = gen() % 100;
        }
    }

    return m;
}

int getSequentialOperations(int* m, int r, int c) {
    int minElement = 100;

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (m[i * c + j] < minElement) {
                minElement = m[i * c + j];
            }
        }
    }

    return minElement;
}

int getParallelOperations(int* m, int r, int c) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int part = r / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&m[proc * part * c], part * c, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    int* mini_matrix = new int[part * c];

    if (rank == 0) {
        mini_matrix = m;
    } else {
        MPI_Status status;
        MPI_Recv(mini_matrix, part * c, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int min = 100;
    int local_min = getSequentialOperations(mini_matrix, part, c);

    if (rank == 0 && r > size * part) {
        int local_min_ostatok;
        mini_matrix = &m[size * part * c];
        local_min_ostatok = getSequentialOperations(mini_matrix, r - (size * part), c);
        local_min = (local_min < local_min_ostatok) ? local_min : local_min_ostatok;
    }

    MPI_Reduce(&local_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return min;
}
