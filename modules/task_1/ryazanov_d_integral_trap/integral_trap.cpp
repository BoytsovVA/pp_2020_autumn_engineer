// Copyright 2020 Dmitriy Ryazanov
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "../../../modules/task_1/ryazanov_d_integral_trap/integral_trap.h"

double fun(double x) {
    return 7 / (x + 1);
}

std::function<double(double)> f = fun;

double integral(std::function<double(double)> func, int n, double a, double b) {
    double q_local = 0;
    double step = (b - a) / n;
    if ((n == 0) || (a > b)) {
        throw "Error";
    } else {
      for (int i = 1; i < step; ++i) {
        q_local += f(a + i * step);
      }
      q_local += (f(a) + f(b)) / 2;
      q_local *= step;
    }
    return q_local;
}

double integration(std::function<double(double)> func, int n, int a, int b) {
  int size;
  int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if ((n == 0) || (a > b)) {throw "Error";}
    double q_local = .0;
    double step = (b - a) / size;

  if (n < size) {
      q_local = integral(func, 1, a + rank * step, a + (rank + 1) * step);
  } else {
    if (n % size == 0) {
        q_local = integral(func, n / size, a + rank * step, a + (rank + 1) * step);
    } else {
      if (rank != size - 1)
        q_local = integral(func, n / size, a + rank * step, a + (rank + 1) * step);
      else
        q_local = integral(func, n - (n / size) * (size - 1), a + rank * step, b);
    }
  }
  double q_global = 0;
  MPI_Reduce(&q_local, &q_global, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);
  MPI_Bcast(&q_global, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return q_global;
}
