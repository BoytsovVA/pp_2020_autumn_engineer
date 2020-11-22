// Copyright 2020 Alekhin Denis
#include <mpi.h>
#include<iostream>
#include <vector>
#include "../../modules/task_1/alekhin_d_integration_trapezoidal_method/integration.h"

double myFunc(double x) { return x / 2; }

std::function<double(double)> f = myFunc;

double localIntegration(std::function<double(double)> func, int n, double a, double b) {
  if (a > b) throw "Error";
  if (n < 1) throw "Error";
  double local_integral = 0;
  double width = (b - a) / n;
  for (int i = 0; i < n; i++) {
    local_integral += (f(a + i * width) + f(a + (i + 1) * width)) * width / 2;
  }
  return local_integral;
}

double integration(std::function<double(double)> func, int n, int a, int b) {
  if (n < 1) throw "Error: n < 1";
  if (a > b) throw "Error: a > b";

  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double local_integral = 0;

  double step = (b - a) / static_cast<double>(size);

  if (n < size) {  // if number of segments less than number of prosesses
    local_integral = localIntegration(func, 1, a + rank * step, a + (rank + 1) * step);
  } else {  // if number of segments not less than number of prosesses
    if (n % size == 0) {
      local_integral = localIntegration(func, n / size, a + rank * step, a + (rank + 1) * step);
    } else {
      if (rank != size - 1)
        local_integral = localIntegration(func, n / size, a + rank * step, a + (rank + 1) * step);
      else
        local_integral = localIntegration(func, n - (n / size) * (size - 1), a + rank * step, b);
    }
  }
  double global_integral = 0;
  MPI_Reduce(&local_integral, &global_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Bcast(&global_integral, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return global_integral;
}
