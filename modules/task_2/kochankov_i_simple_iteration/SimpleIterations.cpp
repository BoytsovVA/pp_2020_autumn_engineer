// Copyright 2020 Kochankov Ilya

#include "../../../modules/task_2/kochankov_i_simple_iteration/SimpleIterations.h"
#include <mpi.h>
#include <ctime>
#include <random>
#include <vector>
#include "stdexcept"
#include "algorithm"

using std::cin;
using std::cout;
using std::endl;

Matrix get_input_matrix() {
    cout << "Input matrix size: ";
    int matrix_size;
    cin >> matrix_size;

    Matrix matrix = Matrix(matrix_size, matrix_size + 1);

    cout << endl
        << "Input matrix: " << endl;

    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size + 1; j++) {
            if (j == matrix_size) {
                cout << "B[" << i << "] = ";
            } else {
                cout << "A[" << i << ", " << j << "] = ";
            }
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

double get_input_accuracy() {
    cout << endl
        << "Input accuracy: ";
    double eps;
    cin >> eps;
    return eps;
}

void print_result(const vector<double>& result) {
    cout << "Results: " << endl;
    for (int i = 0; i < static_cast<int>(result.size()); i++) {
        cout << "X(" << i << ")=" << result[i] << endl;
    }
}

Matrix get_rand_matrix(int x, int y) {
    std::mt19937 gen;

    gen.seed(static_cast<unsigned int>(time(0)));
    Matrix vec(x, y);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            vec[i][j] = (static_cast<double>(gen()) / 100000) + 1;
        }
    }
    return vec;
}

int is_correct_matrix(const Matrix& matrix) {
    for (int i = 0; i < matrix.get_rows(); i++) {
        if (matrix[i][i] == 0) {
            return 0;
        }
    }
    return 1;
}

vector<double> linear_simple_iteration(const Matrix& matrix, double accuracy) {
    if (is_correct_matrix(matrix) == 0) {
        throw std::runtime_error("There is null element on main diagonal!");
    }
    int matrix_size = matrix.get_rows();
    vector<double> prev_vars_values(matrix_size, 0.0);

    int iterations = 0;
    while (true) {
        iterations++;

        if (iterations == 1000) {
            break;
        }

        vector<double> curent_vars_values(matrix_size, 0.0);

        for (int i = 0; i < matrix_size; i++) {
            curent_vars_values[i] = matrix[i][matrix_size] / matrix[i][i];

            for (int j = 0; j < matrix_size; j++) {
                if (i != j) {
                    curent_vars_values[i] -= matrix[i][j] / matrix[i][i] * prev_vars_values[j];
                }
            }
        }

        double error = 0.0;

        for (int i = 0; i < matrix_size; i++) {
            error += std::abs(curent_vars_values[i] - prev_vars_values[i]);
        }

        if (error < accuracy) {
            break;
        }

        prev_vars_values = curent_vars_values;
    }

    return prev_vars_values;
}

vector<double> parallel_simple_iteration(const Matrix& matrix, double accuracy) {
    int size, rank, len = matrix.get_rows();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int flag;
    if (rank == 0) {
        flag = is_correct_matrix(matrix);
    }
    MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // cout << "Process " << rank << " got flag " << flag << std::flush << endl;
    if (flag == 0) {
        throw std::runtime_error("There is null element on main diagonal!");
    }

    if ( (len < size) || (size == 1) ) {
        if (rank == 0) {
            return linear_simple_iteration(matrix, accuracy);
        } else {
            return vector<double>();
        }
    }
    // cout << "Process " << rank << " enter parallel zone" << std::flush << endl;

    vector<double> prev_vars_values(len, 0.0);

    int delta = len / size;
    int start_index;
    Matrix local_matrix;

    // cout << "Process " << rank << " continued parallel zone" << endl;
    int count = delta;
    if (rank == 0) {
        local_matrix = Matrix(delta, matrix.get_columns());
        std::copy(matrix[0], matrix[0] + delta * matrix.get_columns(), local_matrix[0]);

        for (int process_num = 1; process_num < size; process_num++) {
            start_index = delta * process_num;
            if (process_num == size - 1) {
                count = len - start_index;
            }
            MPI_Send(matrix[start_index], matrix.get_columns() * count, MPI_DOUBLE, process_num, 0, MPI_COMM_WORLD);
        }
        count = delta;
    } else {
        MPI_Status status;
        if (rank == size - 1) {
            count = len - delta * (size - 1);
        }
        // cout << "Process " << rank << " got " << count << " rows" << std::flush << endl;

        local_matrix = Matrix(count, matrix.get_columns());
        MPI_Recv(local_matrix[0], matrix.get_columns() * count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    vector<int> resive_counts(size);
    vector<int> dips(size);
    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            resive_counts[i] = len - delta * (size - 1);
        } else {
            resive_counts[i] = delta;
        }
        if (i == 0) {
            dips[i] = 0;
        } else {
            dips[i] = dips[i - 1] + delta;
        }
    }

    // cout << "Process " << rank << " get subMat: Matrix(" << local_matrix.get_rows()
    //<< "," << local_matrix.get_columns() << ")" << endl;
    // cout << "[";
    // for(int i =0; i < local_matrix.get_rows(); i++){
    //     cout << "[";
    //     for (int j = 0; j < local_matrix.get_columns(); j++){
    //         cout << local_matrix[i][j] << ", ";
    //     }
    //     if (i == local_matrix.get_rows()-1){
    //         cout << "]]" << endl;
    //     }else{
    //         cout << "]," << endl;
    //     }
    // }

    int iterations = 0;

    while (true) {
        iterations++;

        if (iterations == 1000) {
            break;
        }

        vector<double> curent_vars_values(local_matrix.get_rows(), 0.0);

        for (int i = 0; i < local_matrix.get_rows(); i++) {
            curent_vars_values[i] = local_matrix[i][local_matrix.get_columns() - 1] /
                                    local_matrix[i][i + rank * delta];

            for (int j = 0; j < local_matrix.get_columns() - 1; j++) {
                if (i + rank * delta != j) {
                    // cout << "Process " << rank << " was : "
                    // << curent_vars_values[i] << std::flush << endl;
                    curent_vars_values[i] -= (local_matrix[i][j] /
                                        local_matrix[i][i + rank * delta]) * prev_vars_values[j];
                }
            }
        }

        // cout << "Process " << rank << " has curent_vars_values: "<< endl;
        // cout << "(";
        // for (auto i : curent_vars_values){
        //     cout << i << ", ";
        // }
        // cout << ")" << endl;

        double error = 0.0;

        for (int i = 0; i < local_matrix.get_rows(); i++) {
            error += std::abs(curent_vars_values[i] - prev_vars_values[i + rank * delta]);
        }

        MPI_Allreduce(MPI_IN_PLACE, &error, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // cout << "Process " << rank << " has error=" << error << endl;

        if (error < accuracy) {
            break;
        }

        // cout << "Process " << rank << " send " << count << " rows" << std::flush << endl;
        MPI_Allgatherv(&curent_vars_values[0], count, MPI_DOUBLE, &prev_vars_values[0],
                       &resive_counts[0], &dips[0], MPI_DOUBLE, MPI_COMM_WORLD);

        // cout << "Process " << rank << " has prev_vars_values: "<< endl;
        // cout << "(";
        // for (auto i : prev_vars_values){
        //     cout << i << ", ";
        // }
        // cout << ")" << std::flush << endl;
    }

    return prev_vars_values;
}
