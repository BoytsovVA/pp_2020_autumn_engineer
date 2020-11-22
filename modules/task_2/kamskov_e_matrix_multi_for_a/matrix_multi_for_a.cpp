// Copyright 2020 Kamskov Eugene
#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_2/kamskov_e_matrix_multi_for_a/matrix_multi_for_a.h"


std::vector<int> getRandomMatrix(int rows, int columns) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(columns * rows);
    for (int  i = 0; i < columns ; i++) {
        for (int j = 0; j < rows; j++) {
         vec[i * rows + j] = gen() % 10 + 1; }
        }
    return vec;
}

std::vector<int> getRes(std::vector<int> vec, std::vector<int> sec_vec, int size_row) {
    const int  sz = vec.size();

    std::vector<int> result(sz);
    int sum = 0;
    for (unsigned int k = 0;  k < vec.size()/size_row; k++) {
        for (int j = 0; j < size_row; j++) {
            sum = 0;
            for (int  i = 0; i < size_row; i++) {
               sum += vec[k * size_row + i] * sec_vec[j + i * size_row];
            }
            result[k * size_row + j] = sum;
        }
    }
    return result;
}

std::vector<int> getSequentialOperations(std::vector<int> vec, std::vector<int> sec_vec, int size_row) {
    const int  sz = vec.size();
    std::vector<int> result(sz);
    int sum = 0;
    for (unsigned int k = 0;  k < vec.size()/size_row; k++) {
        for (int j = 0; j < size_row; j++) {
            sum = 0;
            for (int  i = 0; i < size_row; i++) {
               sum += vec[k * size_row +i] * sec_vec[j + i * size_row];
            }
            result[k * size_row + j] = sum;
        }
    }
    return result;
}

std::vector<int> getParallelOperations(std::vector<int> global_vec, std::vector<int> sec_global_vec,
                          int rows) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = rows / size;
  if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(&global_vec[0] + proc * delta * rows, delta* rows,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&sec_global_vec[0] , rows*rows,
                        MPI_INT, proc, 1, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta * rows);
    std::vector<int> second_local_vec(rows * rows);

    if (rank == 0) {
        local_vec = std::vector<int>(global_vec.begin(),
                                     global_vec.begin() + delta * rows);
        second_local_vec = std::vector<int>(sec_global_vec.begin(),
                                     sec_global_vec.begin() + rows * rows);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], delta*rows, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&second_local_vec[0], delta*rows*delta*rows, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    std::vector<int> res_matrix(rows * rows);
    std::vector<int> res_vector = getSequentialOperations(local_vec, second_local_vec, rows);
    MPI_Allgather(res_vector.data(), delta * rows, MPI_INT, res_matrix.data(),
        delta * rows, MPI_INT, MPI_COMM_WORLD);
    if (rank == 0) {
            local_vec = std::vector<int>(global_vec.begin()+ size * delta * rows,
                                     global_vec.end());
    std::vector<int> remaining_vector = getSequentialOperations(local_vec, second_local_vec, rows);
        for (unsigned int i = 0; i < remaining_vector.size(); i++) {
            res_matrix[size * delta * rows + i] = remaining_vector[i];
        }
    }

    return res_matrix;
}
