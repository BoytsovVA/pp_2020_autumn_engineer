  // Copyright 2020 Zlobin George
#include <mpi.h>
#include "../../../modules/task_1/zlobin_g_method_of_rectangles/method_rectangles.h"

double getSequentialIntegration(double(*Func)(double), const double& a, const double& b,
                    const int& n, const int& method) {
    const double h = (b - a) / static_cast<double>(n);
    double sum = 0;

    switch (method) {
    case LEFT_RECTANGLES:
        for (int i = 0; i < n; i++)
            sum += Func(a + h * i);
        break;
    case RIGHT_RECTANGLES:
        for (int i = 0; i < n; i++)
            sum += Func(a + h * (i + 1));
        break;
    case MEAN_RECTANGLES:
        for (int i = 0; i < n; i++)
            sum += Func(a + h * (i + 0.5));
        break;
    default:
        break;
    }

    return (sum * h);
}

double getParallelIntegration(double(*Func)(double), const double& a, const double& b,
                    const int& n, const int& method) {
    const double h = (b - a) / static_cast<double>(n);
    double global_sum = 0;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double local_sum = 0;
    switch (method) {
    case LEFT_RECTANGLES:
        for (int i = rank; i < n; i += size)
            local_sum += Func(a + h * i);
        break;
    case RIGHT_RECTANGLES:
        for (int i = rank; i < n; i += size)
            local_sum += Func(a + h * (i + 1));
        break;
    case MEAN_RECTANGLES:
        for (int i = rank; i < n; i += size)
            local_sum += Func(a + h * (i + 0.5));
        break;
    default:
        break;
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum * h;
}
