// Copyright 2020 Gorbunova Valeria
#include <mpi.h>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "./../../modules/task_1/gorbunova_v_lexicograph/lexicograph.h"


std::string generateString(const int length) {
    std::string s;
    for (int i = 0; i < length; i++) {
        s += std::rand() % 10 + 97;
    }
    return s;
}

bool getSecuentialOperation(const std::string part) {
    int length = (part.length());
    for (int i = 1; i < length; i++)
        if (part[i] < part[i - 1]) {
            if ((part[i - 1] != '.') && (part[i] != '.'))
                return false;
        }
    return true;
}

bool getParallelOperation(const std::string s1, const std::string s2) {
    std::string s1_s2 = s1 + '.' + s2 + '.';
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int div = (s1_s2.length() - 1) / size;
    int mod = (s1_s2.length() - 1) % size;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int startval, endval;
            startval = i * div + mod;
            endval = startval + div;
            MPI_Send(&startval, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&endval, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
        }
    }
    std::string part;
    int startval, endval;
    bool result, res;
    if (rank == 0) {
        part = s1_s2.substr(0, div + mod + 1);
    } else {
        MPI_Status status;
        MPI_Recv(&startval, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&endval, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (int i = startval; i <= endval; i++)
            part += s1_s2[i];
    }
    result = getSecuentialOperation(part);
    MPI_Allreduce(&result, &res, 1, MPI_C_BOOL, MPI_LAND, MPI_COMM_WORLD);
    return res;
}
