// Copyright 2020 Avdeev Andrey
#include <mpi.h>
#include "../../../modules/task_1/avdeev_a_rectangle_method/rectangle_method.h"

double getSequentialIntegration(double(*func)(double), const double& a, const double& b, const int& n) {
    if (a > b) throw "border_exception";
    if (n < 1) throw "segments_exception";
    const double h = (b - a) / static_cast<double>(n);
    double sum = 0;

    for (int i = 0; i < n; i++)
        sum += func(a + h * i);

    return (sum * h);
}

double getParallelIntegration(double(*func)(double), const double& a, const double& b, const int& n) {
    if (a > b) throw "border_exception";
    if (n < 1) throw "segments_exception";
    const double h = (b - a) / static_cast<double>(n);
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double global_sum = 0;
    double local_sum = 0;

    for (int i = rank; i < n; i += size)
        local_sum += func(a + h * i);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum * h;
}
