// Copyright 2020 Gavrilov Nikita
#ifndef MODULES_TASK_1_GAVRILOV_N_MONTE_CARLO_MONTE_CARLO_H_
#define MODULES_TASK_1_GAVRILOV_N_MONTE_CARLO_MONTE_CARLO_H_

double getIntegral(double _min, double _max, int splitCount, double(*func)(double));
double getIntegralParallel(double _min, double _max, int splitCount, double(*func)(double));

#endif  // MODULES_TASK_1_GAVRILOV_N_MONTE_CARLO_MONTE_CARLO_H_
