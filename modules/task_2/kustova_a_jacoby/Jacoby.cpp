// Copyright 2020 Kustova Anastasiya
#include "../../../modules/task_2/kustova_a_jacoby/Jacoby.h"
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include <ctime>
#define MAX_ITERATIONS 100
double Distance(double *X_Old, double *X_New, int n) {
    int i;
    double max = 0;
    for (i = 0; i < n; i++) {
        if (my_abs(X_Old[i] - X_New[i]) > max)
        max = my_abs(X_Old[i] - X_New[i]);
    }
    return(max);
}
double my_abs(double x) {
    if (x > 0) {
        return x;
    } else {
        return -x;
    }
}
double *Iterations(int n, double *X_Old, double *X_New, double *Bloc_X, double *BRecv,
        double *ARecv, int GlobalRowNo, int amountRowBloc, int rank) {
    int index;
    for (int irow = 0; irow < n; irow++) {
        X_Old[irow] = X_New[irow];
    }

    for (int irow = 0; irow < amountRowBloc; irow++) {
        GlobalRowNo = (rank * amountRowBloc) + irow;
        Bloc_X[irow] = BRecv[irow];
        index = irow * n;

        for (int icol = 0; icol < n; icol++) {
            if (icol != GlobalRowNo) {
                Bloc_X[irow] -= X_Old[icol] * ARecv[index + icol];
            }
        }
        Bloc_X[irow] = Bloc_X[irow] / ARecv[index + GlobalRowNo];
    }
    return Bloc_X;
}

double *Iteration_for_0_rank(int n, double *X_Old, double *Input_B, double *Bloc_XX,
        double *Input_A, int GlobalRowNo, int amountRowBloc, int size) {
    int index;
    for (int irow = amountRowBloc * size; irow < n; irow ++) {
        GlobalRowNo = irow;
        Bloc_XX[irow] = Input_B[irow];
        index = irow * n;
        for (int icol = 0; icol < n; icol++) {
            if (icol != GlobalRowNo) {
                Bloc_XX[irow] -= X_Old[icol] * Input_A[index + icol];
            }
        }
        Bloc_XX[irow] = Bloc_XX[irow] / Input_A[index + GlobalRowNo];
    }
    return Bloc_XX;
}


bool Check_Correct_Matrix(std::vector<double> matrix, int n) {
    double modul_sum;
    for (int i = 0; i < n; i++) {
        modul_sum = 0;
        if (matrix[i * n + i] == 0) {
            return false;
        }
        for (int j = 0; j < n; j++) {
            if (j != i) {
                modul_sum += my_abs(matrix[i * n + j] / matrix[i * n + i]);
            }
        }
        if (modul_sum > 1) {
            return false;
        }
    }
    return true;
}

std::vector<double> Parallel_Jacoby(std::vector<double> A, std::vector<double> B, int n, double eps) {
    int size, rank, amountRowBloc, GlobalRowNo, Iteration = 0;
    double *ARecv, *BRecv, *Bloc_XX, *X_New, *X_Old, *Bloc_X, *Input_A, *Input_B;
    std::vector<double> res(n);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    amountRowBloc = n / size;    // how much to each proc
    Input_A = new double[n * n];
    Input_B = new double[n];
    Bloc_X = new double[n];
    X_New = new double[n];
    X_Old = new double[n];
    for (int i = 0; i < n * n; i++) {
        Input_A[i] = A[i];
        if (i < n) {
            Input_B[i] = B[i];
            X_New[i] = B[i];
        }
    }
    ARecv = new double[amountRowBloc * n];
    BRecv = new double[amountRowBloc];
    // double s = MPI_Wtime();
     MPI_Scatter(Input_A, amountRowBloc * n, MPI_DOUBLE, ARecv, amountRowBloc * n, MPI_DOUBLE, 0,     MPI_COMM_WORLD);
     MPI_Scatter(Input_B, amountRowBloc, MPI_DOUBLE, BRecv, amountRowBloc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // std::cout << MPI_Wtime() - s << "    ";
    for (int irow=0; irow < amountRowBloc; irow ++) {
        Bloc_X[irow] = BRecv[irow];
    }
    Bloc_XX = new double[n];
    do {
        Bloc_X = Iterations(n, X_Old, X_New, Bloc_X, BRecv, ARecv, GlobalRowNo, amountRowBloc, rank);
        MPI_Allgather(Bloc_X, amountRowBloc, MPI_DOUBLE, X_New, amountRowBloc, MPI_DOUBLE, MPI_COMM_WORLD);
        if (rank == 0) {
            Bloc_XX = Iteration_for_0_rank(n, X_Old, Input_B, Bloc_XX, Input_A, GlobalRowNo,  amountRowBloc, size);
        }
        for (int irow = amountRowBloc * size; irow < n; irow ++) {
            MPI_Allgather(&Bloc_XX[irow], 1, MPI_DOUBLE, &X_New[irow], 1, MPI_DOUBLE, MPI_COMM_WORLD);
        }
        Iteration++;
    } while ((Iteration < MAX_ITERATIONS) && (Distance(X_Old, X_New, n) >= eps));
    for (int i = 0; i < n; i++) {
        res[i] = X_New[i];
    }
    return res;
}

std::vector<double> Sequential_Jacoby(std::vector<double> A, std::vector<double> B, int n, double eps) {
    double *Input_A, *Input_B, *X_New, *X_Old;
    Input_A = new double[n * n];
    Input_B = new double[n];
    std::vector<double> res(n);
    X_New = new double[n];
    X_Old = new double[n];
    for (int i = 0; i < n * n; i++) {
        Input_A[i] = A[i];
        if (i < n) {
            Input_B[i] = B[i];
            X_New[i] = B[i];
        }
    }
    int Iteration = 0;
    do {
        for (int k = 0; k < n; k++) {
            X_Old[k] = X_New[k];
        }
        for (int i = 0; i < n; i++) {
            X_New[i] = Input_B[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    X_New[i] -= Input_A[i * n + j] * X_Old[j];
                }
            }
            X_New[i] /= Input_A[i * n + i];
        }
        Iteration++;
    } while ((Iteration < MAX_ITERATIONS) && (Distance(X_Old, X_New, n) >= eps));
    for (int i = 0; i < n; i++) {
        res[i] = X_New[i];
    }
    return res;
}
