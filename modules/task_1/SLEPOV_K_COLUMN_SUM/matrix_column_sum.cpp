// Copyright 2020 Slepov Konstatntin
#include "../../../modules/task_1/SLEPOV_K_COLUMN_SUM/matrix_column_sum.h"
#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include<vector>


Matrix getRandomMatrix(int c, int r) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    Matrix matrix(c, r);
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < r; j++) {
            matrix.data[i * r + j] = gen() % 101;
        }
    }
    return matrix;
}

vector<int> SeqColumnSum(const Matrix & matrix) {
    std::vector<int> v(matrix.columns);

    for (int i = 0; i < matrix.columns; i++) {
        v[i] = 0;
        for (int j = 0; j < matrix.rows; j++) {
            v[i] += matrix.data[i*matrix.rows + j];
        }
    }
    return v;
}

vector<int> ParColumnSum(const Matrix & gm) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if num of process less then columns there is no sens to parallel it
    // and delta might be = 0
    if (static_cast<int>(gm.columns) < size) {
        return SeqColumnSum(gm);
    }

    // subdividing data per process
    int delta = gm.columns / size;
    int tail = delta + gm.columns % size;
    Matrix * subMat;
    // as col % size might be != 0, 1st process will be given with more data
    if (rank == 0) {
        // gets data imeadetly without MPI call
      subMat = new Matrix(tail, gm.rows, gm.data);
    } else {
        // data will be got with MPI call
      subMat = new Matrix(delta, gm.rows);
    }

    // distribution of columns by process
    if (rank == 0) {
      int cur_col = tail;
      for (int proc = 1; proc < size; proc++) {
        for (int i = 0; i < delta; cur_col++, i++) {
          MPI_Send(&gm.data[cur_col * gm.rows], gm.rows, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
      }
    } else {
      MPI_Status status;
      for (int i = 0; i < delta; i++) {
        MPI_Recv(&subMat->data[i * gm.rows], gm.rows, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      }
    }

    vector<int> subRes = SeqColumnSum(*subMat);

    std::vector<int> res(gm.columns);
    // gathering sub-vectors
    if (rank == 0) {
      std::copy(subRes.begin(), subRes.end(), res.begin());
      if (size <= 1) return res;
      MPI_Status status;
      for (int i = 0; i < size - 1; i++) {
        MPI_Recv(&res[0] + tail + i * delta, delta, MPI_INT, i + 1, 0, MPI_COMM_WORLD, &status);
      }
    } else {
          MPI_Send(&subRes[0], subRes.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    delete subMat;
    return res;
}
Matrix::Matrix(int c, int r, int* data) : columns(c), rows(r) {
    if (c <= 0 || r <= 0) throw "Negative dimension";
    this->data = new int[c * r];
    for (int i = 0; i < c * r; i++) {
      this->data[i] = data[i];
    }
}

Matrix::Matrix(int c, int r) : columns(c), rows(r) {
  if (c <= 0 || r <= 0) throw "Negative dimension";
  this->data = new int[c * r];
  for (int i = 0; i < c * r; i++) {
    this->data[i] = 0;
  }
}

Matrix::Matrix(const Matrix& m) : columns(m.columns), rows(m.rows) {
    this->data = new int[columns * rows];
    for (int i = 0; i < columns * rows; i++) {
     this->data[i] = data[i];
    }
}
