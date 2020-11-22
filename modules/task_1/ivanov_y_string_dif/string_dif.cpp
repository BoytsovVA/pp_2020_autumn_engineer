// Copyright 2020 Ivanov Yaroslav
#include <mpi.h>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/ivanov_y_string_dif/string_dif.h"


std::string generateString(int n) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned char>(time(0)));
    std::string str;
    for (int i = 0; i < n; i++) {
        str.push_back(gen() % 254 + 1);
    }
    return str;
}

int getLocalDif(const std::string str1, const std::string str2, const int size) {
    int count_dif = 0;
    if (str1.empty()) {
        return count_dif;
    }
    if (str2.empty()) {
        return count_dif;
    }
    for (int i = 0; i < size; i++) {
        if (str1[i] != str2[i]) {
            count_dif++;
        }
    }
    return count_dif;
}

int getParallelDif(const std::string str1, const std::string str2, const int size_str) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = size_str / size;
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&str1[0] + i * delta, delta, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Send(&str2[0] + i * delta, delta, MPI_CHAR, i, 1, MPI_COMM_WORLD);
        }
    }

    std::string local_str1, local_str2;
    if (rank == 0) {
        local_str1 = std::string(str1.begin(), str1.begin() + delta);
        local_str2 = std::string(str2.begin(), str2.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_str1[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_str2[0], delta, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
    }
    int global_dif = 0;
    int local_dif = getLocalDif(local_str1, local_str2, size_str);
    MPI_Op  op_code;
    op_code = MPI_SUM;
    MPI_Reduce(&local_dif, &global_dif, 1, MPI_CHAR, op_code, 0, MPI_COMM_WORLD);
    return global_dif;
}
