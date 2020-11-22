// Copyright 2020 Ognev Denis
#ifndef MODULES_TASK_1_OGNEV_D_INTEGRATION_RECTANGULAR_METHOD_INTEGRATION_H_
#define MODULES_TASK_1_OGNEV_D_INTEGRATION_RECTANGULAR_METHOD_INTEGRATION_H_

#include <functional>

double function(double x);
double integrationSequential(std::function<double(double)> f, double a, double b, int n);
double integrationParallel(std::function<double(double)> f, double a, double b, int n);

#endif  // MODULES_TASK_1_OGNEV_D_INTEGRATION_RECTANGULAR_METHOD_INTEGRATION_H_
