// Copyright 2020 Avdeev Andrey
#ifndef MODULES_TASK_1_AVDEEV_A_RECTANGLE_METHOD_RECTANGLE_METHOD_H_
#define MODULES_TASK_1_AVDEEV_A_RECTANGLE_METHOD_RECTANGLE_METHOD_H_

double getSequentialIntegration(double(*func)(double), const double& a, const double& b,
    const int& n);
double getParallelIntegration(double(*func)(double), const double& a, const double& b,
    const int& n);

#endif  // MODULES_TASK_1_AVDEEV_A_RECTANGLE_METHOD_RECTANGLE_METHOD_H_
