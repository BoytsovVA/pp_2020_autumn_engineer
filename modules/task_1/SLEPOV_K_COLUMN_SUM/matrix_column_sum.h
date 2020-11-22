// Copyright 2020 Slepov Konstantin
#ifndef MODULES_TASK_1_SLEPOV_K_COLUMN_SUM_MATRIX_COLUMN_SUM_H_
#define MODULES_TASK_1_SLEPOV_K_COLUMN_SUM_MATRIX_COLUMN_SUM_H_

#include <vector>
using std::vector;

// column - major order
class Matrix {
 public:
  int* data;
  int columns, rows;

  Matrix(int c, int r, int* data);
  Matrix(int c, int r);
  Matrix(const Matrix& m);
  ~Matrix() { delete[] data; }
};

// returns random matrix in column-major order
Matrix getRandomMatrix(int columns, int rows);
vector<int> SeqColumnSum(const Matrix& matrix);
vector<int> ParColumnSum(const Matrix& matrix);

#endif  // MODULES_TASK_1_SLEPOV_K_COLUMN_SUM_MATRIX_COLUMN_SUM_H_
