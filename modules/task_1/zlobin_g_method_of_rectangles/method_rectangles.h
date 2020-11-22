// Copyright 2020 Zlobin George
#ifndef MODULES_TASK_1_ZLOBIN_G_METHOD_OF_RECTANGLES_METHOD_RECTANGLES_H_
#define MODULES_TASK_1_ZLOBIN_G_METHOD_OF_RECTANGLES_METHOD_RECTANGLES_H_

enum Methods {LEFT_RECTANGLES = 1, RIGHT_RECTANGLES, MEAN_RECTANGLES};

double getSequentialIntegration(double(*Func)(double), const double& a, const double& b,
                    const int& n, const int& method = LEFT_RECTANGLES);
double getParallelIntegration(double(*Func)(double), const double& a, const double& b,
                    const int& n, const int& method = LEFT_RECTANGLES);

#endif  // MODULES_TASK_1_ZLOBIN_G_METHOD_OF_RECTANGLES_METHOD_RECTANGLES_H_
