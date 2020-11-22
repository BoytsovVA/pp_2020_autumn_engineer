// Copyright 2020 NDAH RONY
#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>
#include <utility>
#include "./vec_er_mpi.h"

std::vector<int> RV(int s) {
    std::mt19937 rv;
    if (s <= 0)
        throw - 1;
    std::vector<int> t(s);
    rv.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < static_cast<int>(t.size()); i++)
        t[i] = rv() % 100;
    return t;
}

std::vector<int> vecNor(std::vector<int> a) {
    for (int i = 0; i < static_cast<int>(a.size()) - 1; i++) {
        for (int j = 0; j < static_cast<int>(a.size()) - i - 1; j++) {
            if (a[j] > a[j+1]) {
                std::swap(a[j], a[j+1]);
            }
        }
    }
    return a;
}

int parallelVec(const std::vector<int>& a, int n) {
    int r, s;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    int res = n / s;
    int rem = n - res * s;
    if (r == 0) {
        for (int i = 1; i < s; i++) {
            if (i < s)
                MPI_Send(&a[0] + i*res + rem - 1, res + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<int> t;
    if (r == 0) {
        t.resize(res+rem);
        t = { a.begin(), a.begin() + res + rem };
    } else {
        t.resize(res+1);
        MPI_Status status;
        MPI_Recv(&t[0], res + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int count_s = countErr(t);
    int result = 0;
    MPI_Reduce(&count_s, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return result;
}

int countErr(const std::vector<int>& a) {
    int count = 0;
    for (int i = 0; i < static_cast<int>(a.size()) - 1; i++) {
        if (a[i] > a[i+1])
            count++;
    }
    return count;
}


