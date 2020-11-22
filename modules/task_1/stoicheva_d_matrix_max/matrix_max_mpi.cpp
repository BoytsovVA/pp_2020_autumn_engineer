// Copyright 2020 Stoicheva Darya
#include "../../../modules/task_1/stoicheva_d_matrix_max/matrix_max_mpi.h"
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include <algorithm>

std::vector<int> create_random_matrix(const size_t rows, const size_t columns) {
    size_t size = rows * columns;
    std::mt19937 gen(uint32_t(time(0)));
    std::uniform_int_distribution<int> uid(-50, +50);
    std::vector<int> matrix(size);

    for (size_t i = 0; i < size; i++) {
        matrix[i] = uid(gen);
    }
    return matrix;
}

std::vector<int> get_max_elements_of_rows(std::vector<int> matrix, const size_t rows, const size_t columns) {
    if (!(rows && columns)) {
        return std::vector<int>();
    }
    std::vector<int> max_elements(rows);
    for (size_t row = 0; row < rows; row++) {
        int max_value = INT_MIN;
        for (size_t column = 0; column < columns; column++) {
            if (max_value < matrix[row * columns + column]) {
                max_value = matrix[row * columns + column];
            }
        }
        max_elements[row] = max_value;
    }
    return max_elements;
}

void print_matrix(std::vector<int> matrix, const size_t rows, const size_t columns, std::string prefix) {
    #ifdef DEBUG_PRINT
    std::cout << "\n" << prefix << "Matrix: {\n";
    for (size_t row = 0; row < rows; row++) {
        std::cout << prefix << "  ";
        for (size_t column = 0; column < columns; column++) {
            std::cout << matrix[row * columns + column] << ", ";
        }
        std::cout << "\n";
    }
    std::cout << prefix << "}\n";
    #endif
}

std::vector<int> get_max_elements_of_rows_sequentional(std::vector<int> matrix, const size_t rows,
    const size_t columns) {
    return get_max_elements_of_rows(matrix, rows, columns);
}

std::vector<int> get_max_elements_of_rows_parallel(const std::vector<int>& matrix, const size_t rows,
    const size_t columns) {

    if (!(rows && columns)) {
        return std::vector<int>();
    }

    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    #ifdef DEBUG_PRINT
        printf("[%d] Process: %d of %d, Rows: %d, Columns: %d\n", ProcRank, ProcRank, ProcNum, rows, columns);
    #endif

    size_t count_processes = size_t(ProcNum);
    size_t count_rows_per_process = std::max(size_t(1), rows / count_processes);
    size_t count_extra_rows = rows < count_processes ? 0 : rows % count_processes;

    #ifdef DEBUG_PRINT
        printf("[%d] Rows per process: %d,  extra rows: %d\n", ProcRank, count_rows_per_process, count_extra_rows);
    #endif

    if (ProcRank == 0) {
        print_matrix(matrix, rows, columns, "[" + std::to_string(ProcRank) + "] initial  ");
    }

    std::vector<int> sent_matrix_rows(count_rows_per_process * columns);

    MPI_Scatter(matrix.data(), count_rows_per_process * columns, MPI_INT,
                sent_matrix_rows.data(), count_rows_per_process * columns, MPI_INT, 0, MPI_COMM_WORLD);

    #ifdef DEBUG_PRINT
        printf("[%d] Rows per process: %d,  extra rows: %d\n", ProcRank, count_rows_per_process, count_extra_rows);
        print_matrix(sent_matrix_rows, count_rows_per_process, columns,
                     "[" + std::to_string(ProcRank) + "] sent_matrix_raws  ");
    #endif

    std::vector<int> receive_elements_max = get_max_elements_of_rows(sent_matrix_rows, count_rows_per_process,
                                                                     columns);
    print_matrix(receive_elements_max, count_rows_per_process, 1,
        "[" + std::to_string(ProcRank) + "] receive_elements_max  ");

    std::vector<int> max_values;
    if (ProcRank == 0) {
        max_values = std::vector<int>(std::max(rows, size_t(ProcNum)));
    }

    MPI_Gather(receive_elements_max.data(), count_rows_per_process, MPI_INT,
               max_values.data(), count_rows_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    if (ProcRank == 0) {
        if (count_extra_rows) {
            int count_base_rows = count_rows_per_process * count_processes;
            std::vector<int> extra_elements_max = get_max_elements_of_rows(
                std::vector<int>(matrix.begin() + count_base_rows * columns, matrix.end()),
                                 count_extra_rows, columns);
            print_matrix(extra_elements_max,
                count_extra_rows, 1, "[" + std::to_string(ProcRank) + "] extra_elements_max  ");
            for (size_t extra_row = 0; extra_row < count_extra_rows; extra_row++) {
                max_values[count_base_rows + extra_row] = extra_elements_max[extra_row];
            }
        }
        print_matrix(max_values, rows, 1, "[" + std::to_string(ProcRank) + "] max_values  ");
        if (rows < count_processes) {
            max_values = std::vector<int>(max_values.begin(), max_values.begin() + rows);
        }
    }

    return max_values;
}
