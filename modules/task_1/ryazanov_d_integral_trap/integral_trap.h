// Copyright 2020 Dmitriy Ryazanov
#ifndef MODULES_TASK_1_RYAZANOV_D_INTEGRAL_TRAP_INTEGRAL_TRAP_H_
#define MODULES_TASK_1_RYAZANOV_D_INTEGRAL_TRAP_INTEGRAL_TRAP_H_

#include <vector>
#include <string>
#include <functional>



double fun(double);
extern std::function<double(double)> f;
double integral(std::function<double(double)>, int, double, double);
double integration(std::function<double(double)>, int, int, int);

#endif  // MODULES_TASK_1_RYAZANOV_D_INTEGRAL_TRAP_INTEGRAL_TRAP_H_
