// Copyright 2020 Gavrilov Nikita
#include <mpi.h>
#include <utility>
#include <algorithm>
#include "../../modules/task_1/gavrilov_n_monte_carlo/monte_carlo.h"

double getIntegral(double _min, double _max, int splitCount, double(*func)(double)) {
    if (splitCount <= 0)
        throw "splitCount should be more than 0";

    bool minMoreThanMax = _min > _max;
    if (minMoreThanMax)
        std::swap(_min, _max);

    double result = 0;
    double delta = (_max - _min) / splitCount;
    for (double i = _min; i < _max;) {
        double next = std::min(i + delta, _max);
        double localDelta = (next - i);
        result += localDelta * func(i + localDelta / 2);
        i = next;
    }
    if (minMoreThanMax)
        result = -result;

    return result;
}

double getIntegralParallel(double _min, double _max, int splitCount, double(*func)(double)) {
    if (splitCount <= 0)
        throw "splitCount should be more than 0";

    bool minMoreThanMax = _min > _max;
    if (minMoreThanMax)
        std::swap(_min, _max);


    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double delta = _max - _min;
    int splitCountMin = splitCount / size;
    int splitCountAddative = splitCount % size;

    int prevSplitCountSum = splitCountMin * rank + std::min(rank, splitCountAddative);
    int currSplitCount = splitCountMin;
    if (splitCountAddative > rank)
        currSplitCount++;

    double splitDelta = delta / splitCount;

    double locmin = splitDelta * prevSplitCountSum;
    double locmax = locmin + splitDelta * currSplitCount;

    double localInteral = 0;
    if (currSplitCount != 0)
        localInteral = getIntegral(locmin, locmax, currSplitCount, func);


    double totalInteral = 0;
    MPI_Op op_code;
    op_code = MPI_SUM;
    MPI_Reduce(&localInteral, &totalInteral, 1, MPI_DOUBLE, op_code, 0, MPI_COMM_WORLD);

    if (minMoreThanMax)
        totalInteral = -totalInteral;

    return totalInteral;
}
