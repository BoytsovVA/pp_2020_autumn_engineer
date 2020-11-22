// Copyright 2020 Kucheryavykh Yan
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_1/kucheryavykh_y_matrix_column_sum/matrix_columns_sum.h"

std::vector<int> createRandomMatrix(const int rows, const int cols) {
    if (rows < 1 || cols < 1)
        throw(1);
    std::vector<int> Matrix(rows * cols);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < rows * cols; i++) {
        Matrix[i] = gen() % 100;
    }
    return Matrix;
}

std::vector<int> getSequintialColumnsSum(const std::vector<int> Matrix, int rows, int cols) {
    std::vector<int> result(cols);
    int sum;
    for (int i = 0; i < cols; i++) {
        sum = 0;
        for (int j = 0; j < rows; j++) {
            sum += Matrix[rows * i + j];
        }
        result[i] = sum;
    }
    return result;
}

std::vector<int> getParallelColumnsSum(const std::vector<int> Matrix_1, int rows, int cols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int delta = cols / size;
    const int reminder = cols % size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ((Matrix_1.size() != (size_t)rows * (size_t)cols) || (rows <= 0) || (cols <= 0)) {
        throw(1);
    }

    if (rows == 1)
        return Matrix_1;

    std::vector <int> Matrix_2(delta * rows, 0);

    if (delta > 0) {
        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                MPI_Send(&Matrix_1[rows * (reminder + i * delta)], rows * delta, MPI_INT, i, 1, MPI_COMM_WORLD);
            }
        } else {
            MPI_Status status;
            MPI_Recv(&Matrix_2[0], delta * rows, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
    }
    std::vector<int> result;
    if (rank == 0) {
        MPI_Status status;
        int sum = 0;
        std::vector<int> elem(rows);
        Matrix_2 = std::vector<int>(Matrix_1.begin(), Matrix_1.begin() + rows * (reminder + delta));

        for (int i = 0; i < delta + reminder; i++) {
            for (int j = 0; j < rows; j++) {
                sum += Matrix_2[i * rows + j];
            }
            result.push_back(sum);
            sum = 0;
        }

        if (delta > 0) {
            for (int i = 1; i < size; i++) {
                MPI_Recv(&elem[0], delta, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
                result.insert(result.end(), elem.begin(), elem.begin() + delta);
            }
        }
    } else if (delta > 0) {
        int sum = 0;
        for (int i = 0; i < delta; i++) {
            for (int j = 0; j < rows; j++) {
                sum += Matrix_2[i * rows + j];
            }
            result.push_back(sum);
            sum = 0;
        }
        MPI_Send(&result[0], delta, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    return result;
}
