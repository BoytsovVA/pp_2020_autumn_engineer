// Copyright 2020 Alekhin Denis
#ifndef MODULES_TASK_1_ALEKHIN_D_INTEGRATION_TRAPEZOIDAL_METHOD_INTEGRATION_H_
#define MODULES_TASK_1_ALEKHIN_D_INTEGRATION_TRAPEZOIDAL_METHOD_INTEGRATION_H_

#include <math.h>
#include <functional>
#include<vector>

double myFunc(double);

extern  std::function<double(double)> f;

// passing function, number of segments and integration limits
double localIntegration(std::function<double(double)>, int, double, double);

// passing function, number of segments and integration limits
double integration(std::function<double(double)>, int, int, int);

#endif  // MODULES_TASK_1_ALEKHIN_D_INTEGRATION_TRAPEZOIDAL_METHOD_INTEGRATION_H_
