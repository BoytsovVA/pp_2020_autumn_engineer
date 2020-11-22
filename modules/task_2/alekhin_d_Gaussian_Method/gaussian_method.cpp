// Copyright 2020 Alekhin Denis
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "../../modules/task_2/alekhin_d_Gaussian_Method/gaussian_method.h"


std::vector<double> getSequantialGauss(Matrix matrix) {
  initMatrix(&matrix);

  for (int i = 0; i < matrix.rows; i++) {
    int leadingLine = chooseLeadingLine(matrix, i);
    swapLines(&matrix, i, leadingLine);
    for (int j = i + 1; j < matrix.rows; j++) {
      double coeff = matrix.matrix[i * matrix.columns + i] / matrix.matrix[j * matrix.columns + i];
      for (int col = 0; col < matrix.columns; col++) {
        matrix.matrix[j * matrix.columns + col] =
          coeff * matrix.matrix[j * matrix.columns + col] - matrix.matrix[i * matrix.columns + col];
      }
    }
  }

  std::vector<double> result(matrix.rows, 0);

  for (int row = matrix.rows - 1; row >= 0; row--) {
    for (int i = row + 1; i < matrix.rows; i++) {
      matrix.matrix[row * matrix.columns + matrix.columns - 1] -= matrix.matrix[row * matrix.columns + i] * result[i];
    }
    result[row] = matrix.matrix[row * matrix.columns + matrix.columns - 1] / matrix.matrix[row * matrix.columns + row];
  }

  return result;
}

void initMatrix(Matrix* matrix) {
  matrix->rows = 5;
  matrix->columns = 6;

  matrix->matrix.push_back(2);
  matrix->matrix.push_back(-5);
  matrix->matrix.push_back(-10);
  matrix->matrix.push_back(-5);
  matrix->matrix.push_back(-1);
  matrix->matrix.push_back(2);

  matrix->matrix.push_back(-4);
  matrix->matrix.push_back(9);
  matrix->matrix.push_back(5);
  matrix->matrix.push_back(-10);
  matrix->matrix.push_back(10);
  matrix->matrix.push_back(-3);

  matrix->matrix.push_back(9);
  matrix->matrix.push_back(7);
  matrix->matrix.push_back(2);
  matrix->matrix.push_back(10);
  matrix->matrix.push_back(9);
  matrix->matrix.push_back(5);

  matrix->matrix.push_back(-6);
  matrix->matrix.push_back(0);
  matrix->matrix.push_back(2);
  matrix->matrix.push_back(4);
  matrix->matrix.push_back(7);
  matrix->matrix.push_back(-4);

  matrix->matrix.push_back(-3);
  matrix->matrix.push_back(-8);
  matrix->matrix.push_back(-4);
  matrix->matrix.push_back(-6);
  matrix->matrix.push_back(7);
  matrix->matrix.push_back(8);
}

int chooseLeadingLine(Matrix matrix, int column) {
  int max = abs(matrix.matrix[column * matrix.columns + column]);
  int maxIndex = column;
  for (int i = column; i < matrix.rows; i++) {
    if (abs(matrix.matrix[i * matrix.columns + column]) > max) {
      max = abs(matrix.matrix[i * matrix.columns + column]);
      maxIndex = i;
    }
  }

  return maxIndex;
}

void swapLines(Matrix* matrix, int line1, int line2) {
  for (int i = 0; i < matrix->columns; i++) {
    double temp = matrix->matrix[i + matrix->columns * line1];
    matrix->matrix[i + matrix->columns * line1] = matrix->matrix[i + matrix->columns * line2];
    matrix->matrix[i + matrix->columns * line2] = temp;
  }
}

std::vector<double> getParallelGauss(Matrix matrix) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int count_of_work_processors = number_of_line_to_store.size();
  for (int i = 0; i < count_of_work_processors; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }

  if (rank < count_of_work_processors) {
    distributeData(matrix, &local_matrix);
    getGaussForward(matrix, &local_matrix);
    // gatherMatrix(&matrix, &local_matrix);
  }
  std::vector<double> result(matrix.rows, 0);
  if (rank == 0) {
    result = getGaussBackward(matrix, &local_matrix);
  }

  MPI_Bcast(
    &result[0],
    result.size(),
    MPI_DOUBLE,
    0,
    MPI_COMM_WORLD);

  return result;
}

void distributeData(Matrix global_matrix, Matrix* local_matrix) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    for (int line = 0; line < global_matrix.rows; line++) {
      if (line % size != 0) {
        MPI_Send(
          &global_matrix.matrix[0] + line * global_matrix.columns,
          global_matrix.columns,
          MPI_DOUBLE,
          line % size,
          0,
          MPI_COMM_WORLD);
      }
    }
  }

  std::vector<int> number_of_lines_to_store = numberOfLinesToStore(global_matrix);

  if (rank == 0) {
    for (int i = 0; i < number_of_lines_to_store[rank]; i++) {
      local_matrix->matrix.insert(
        local_matrix->matrix.begin() + i * global_matrix.columns,
        global_matrix.matrix.begin() + i * size * global_matrix.columns,
        global_matrix.matrix.begin() + i * size * global_matrix.columns + global_matrix.columns);
    }
  } else {
    for (int i = 0; i < number_of_lines_to_store[rank]; i++) {
      std::vector<double> recv(global_matrix.columns);
      MPI_Recv(
        &recv[0],
        global_matrix.columns,
        MPI_DOUBLE,
        0,
        0,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);

      local_matrix->matrix.insert(
        local_matrix->matrix.begin() + i * global_matrix.columns,
        recv.begin(),
        recv.begin() + global_matrix.columns);
    }
  }
}

std::vector<int> numberOfLinesToStore(Matrix matrix) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int vector_size = std::min(size, matrix.rows);
  std::vector<int> result(vector_size, 0);

  for (int i = 0; i < matrix.rows; i++) {
    result[i % size]++;
  }

  return result;
}


void printMatrix(Matrix local_matrix) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  for (int proc = 0; proc < size; proc++) {
    if (rank == proc) {
      for (std::size_t i = 0; i < local_matrix.matrix.size(); i++) {
        if (i % local_matrix.columns == 0)
          std::cout << std::endl << "Rank " << rank << ": ";
        std::cout << local_matrix.matrix[i] << " ";
      }
    }
  }
  std::cout << std::endl;
}

void shareCoeffs(Matrix global_matrix, Matrix local_matrix, int column) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> number_of_lines_to_store = numberOfLinesToStore(global_matrix);
  if (rank != 0) {
    for (int i = 0; i < number_of_lines_to_store[rank]; i++) {
      MPI_Send(
        &local_matrix.matrix[i * global_matrix.columns + column],
        1,
        MPI_DOUBLE,
        0,
        0,
        MPI_COMM_WORLD);
    }
  }
}

int chooseLeadingLineParallel(Matrix global_matrix, Matrix local_matrix, int column) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  shareCoeffs(global_matrix, local_matrix, column);

  std::vector<double> coeff;
  if (rank == 0) {
    int j = 0;
    for (int i = 0; i < global_matrix.rows; i++) {
      if (i % size == 0) {
        coeff.push_back(local_matrix.matrix[j++ * global_matrix.columns + column]);
      }
    }

    for (int i = 0; i < global_matrix.rows; i++) {
      double recv = 0;
      if (i % size != 0) {
        MPI_Recv(
          &recv,
          1,
          MPI_DOUBLE,
          i % size,
          0,
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
        coeff.insert(coeff.begin() + i, recv);
      }
    }
  }

  int max_index = column;
  if (rank == 0) {
    double max = abs(coeff[column]);
    for (std::size_t i = column; i < coeff.size() - 1; i++) {
      for (std::size_t j = column + 1; j < coeff.size(); j++) {
        if (abs(coeff[j]) > max) {
          max = abs(coeff[j]);
          max_index = j;
        }
      }
    }
  }

  MPI_Bcast(&max_index, 1, MPI_INT, 0, MPI_COMM_WORLD);
  return max_index;
}

void getGaussForward(Matrix global_matrix, Matrix* local_matrix) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (int i = 0; i < global_matrix.rows; i++) {
    int leadingLine = chooseLeadingLineParallel(global_matrix, *local_matrix, i);
    swapLinesParallel(local_matrix, leadingLine, i);

    std::vector<double> leading_Line(local_matrix->columns);
    if (i % size == rank) {
      for (int p = 0; p < local_matrix->columns; p++) {
        leading_Line[p] = local_matrix->matrix[(i / size) * local_matrix->columns + p];
      }
    }

    // share leading_Line to all processors
    std::vector<int> number_of_line_to_store = numberOfLinesToStore(global_matrix);
    int count_of_work_processors = number_of_line_to_store.size();
    if (rank == i % size) {
      for (int k = 0; k < count_of_work_processors; k++) {
        if (k != i % size) {
          MPI_Send(
            &leading_Line[0],
            leading_Line.size(),
            MPI_DOUBLE,
            k,
            0,
            MPI_COMM_WORLD);
        }
      }
    } else {
      MPI_Recv(
        &leading_Line[0],
        leading_Line.size(),
        MPI_DOUBLE,
        i % size,
        0,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
    }

    for (int j = i + 1; j < global_matrix.rows; j++) {
      if (rank == (j % size)) {
        double coeff = leading_Line[i] / local_matrix->matrix[(j / size) * local_matrix->columns + i];

        for (int col = 0; col < local_matrix->columns; col++) {
          local_matrix->matrix[(j / size) * local_matrix->columns + col] =
            coeff * local_matrix->matrix[(j / size) * local_matrix->columns + col] - leading_Line[col];
        }
      }
    }
  }
}

void swapLinesParallel(Matrix* local_matrix, int line1, int line2) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (line1 % size == line2 % size && rank == line1 % size) {
    for (int i = 0; i < local_matrix->columns; i++) {
      double temp = local_matrix->matrix[i + local_matrix->columns * (line1 / size)];
      local_matrix->matrix[i + local_matrix->columns * (line1 / size)] =
        local_matrix->matrix[i + local_matrix->columns * (line2 / size)];
      local_matrix->matrix[i + local_matrix->columns * (line2 / size)] = temp;
    }
  } else {
    for (int i = 0; i < local_matrix->columns; i++) {
      if (rank == line1 % size) {
        MPI_Send(&local_matrix->matrix[i + local_matrix->columns * (line1 / size)],
          1,
          MPI_DOUBLE,
          line2 % size,
          0,
          MPI_COMM_WORLD);

        MPI_Recv(
          &local_matrix->matrix[i + local_matrix->columns * (line1 / size)],
          1,
          MPI_DOUBLE,
          line2 % size,
          1,
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
      }

      if (rank == line2 % size) {
        double temp = local_matrix->matrix[i + local_matrix->columns * (line2 / size)];
        MPI_Recv(&local_matrix->matrix[i + local_matrix->columns * (line2 / size)],
          1,
          MPI_DOUBLE,
          line1 % size,
          0,
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);

        MPI_Send(&temp,
          1,
          MPI_DOUBLE,
          line1 % size,
          1,
          MPI_COMM_WORLD);
      }
    }
  }
}

void gatherMatrix(Matrix* global_matrix, Matrix* local_matrix) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> recv(local_matrix->columns);
  global_matrix->matrix.clear();
  if (rank == 0) {
    int j = 0;
    for (int i = 0; i < global_matrix->rows; i++) {
      if (rank != i % size) {
        MPI_Recv(
          &recv[0],
          local_matrix->columns,
          MPI_DOUBLE,
          i % size,
          0,
          MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);

        global_matrix->matrix.insert(
          global_matrix->matrix.begin() + i * global_matrix->columns,
          recv.begin(),
          recv.begin() + global_matrix->columns);
      } else {
        global_matrix->matrix.insert(
          global_matrix->matrix.begin() + i * global_matrix->columns,
          local_matrix->matrix.begin() + j * local_matrix->columns,
          local_matrix->matrix.begin() + j * local_matrix->columns + local_matrix->columns);
        j++;
      }
    }
  } else {
    for (int i = 0; i < local_matrix->rows; i++) {
      MPI_Send(
        &local_matrix->matrix[i * local_matrix->columns],
        local_matrix->columns,
        MPI_DOUBLE,
        0,
        0,
        MPI_COMM_WORLD);
    }
  }
}

std::vector<double> getGaussBackward(Matrix global_matrix, Matrix* local_matrix) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> result(global_matrix.rows, 0);
  for (int row = global_matrix.rows - 1; row >= 0; row--) {
    for (int i = row + 1; i < global_matrix.rows; i++) {
      global_matrix.matrix[row * global_matrix.columns + global_matrix.columns - 1]
        -= global_matrix.matrix[row * global_matrix.columns + i] * result[i];
    }
    result[row] =
      global_matrix.matrix[row * global_matrix.columns + global_matrix.columns - 1] /
      global_matrix.matrix[row * global_matrix.columns + row];
  }

  return result;
}
