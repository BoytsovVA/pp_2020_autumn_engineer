// Copyright 2020 Ekaterina Burmistrova
#include <mpi.h>

#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "./../../modules/task_1/bourmistrova_e_matrix_elem_sum/mpi_fun.h"


std::vector<int> getRandomMatrix(int sz2, int sz1) {
    std::mt19937 gener;
    gener.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> matr(sz2*sz1);
    for (int i = 0; i < sz2; i++) {
        for (int j = 0; j < sz1; j++)
            matr[i*sz1+j] = gener() % 100;
    }
    return matr;
}

int Sequential_method(std::vector<int> vect, std::string op) {
    int siz = vect.size();
    int sum = 0;
    if (op == "+") {
        for (int i = 0; i < siz; i++)
            sum += vect[i];
    }
    if (op == "min") {
        std::sort(vect.begin(), vect.end());
        sum = vect[0];
    }
    if (op == "max") {
        std::sort(vect.begin(), vect.end());
        sum = vect[siz - 1];
    }
    return sum;
}
int Parallel_method(std::vector<int> matr, int n_lin, int n_col, std::string op) {
    int mynode, totnodes;
    int par_sum, inter;

    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &totnodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    par_sum = 0;  // zero sum for accumulation
    const int delta = (n_lin*n_col) / totnodes;

    if (mynode == 0) {
        for (int proc = 1; proc < totnodes; proc++) {
            MPI_Send(&matr[0] + proc * delta, delta,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<int> local_vec(delta);
    if (mynode == 0) {
        local_vec = std::vector<int>(matr.begin(),
            matr.begin() + delta);
    } else {
        MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    inter = Sequential_method(local_vec, op);
    MPI_Op op_code;
    if (op == "+")
        op_code = MPI_SUM;
    if (op == "min")
        op_code = MPI_MIN;
    if (op == "max")
        op_code = MPI_MAX;
    MPI_Reduce(&inter, &par_sum, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return par_sum;
}
int Parallel_method_choose_num_proc(std::vector<int> matr, int n_lin, int n_col, std::string op, int total) {
    int mynode;
    int par_sum, inter;

    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &mynode);
    par_sum = 0;  // zero sum for accumulation
    const int delta = (n_lin*n_col) / total;

    if (mynode == 0) {
        for (int proc = 1; proc < total; proc++) {
            MPI_Send(&matr[0] + proc * delta, delta,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<int> local_vec(delta);
    if (mynode == 0) {
        local_vec = std::vector<int>(matr.begin(),
             matr.begin() + delta);
    } else {
        MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    inter = Sequential_method(local_vec, op);
    MPI_Op op_code;
    if (op == "+")
        op_code = MPI_SUM;
    if (op == "min")
        op_code = MPI_MIN;
    if (op == "max")
        op_code = MPI_MAX;
    MPI_Reduce(&inter, &par_sum, 1, MPI_INT, op_code, 0, MPI_COMM_WORLD);
    return par_sum;
}
