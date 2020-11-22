// Copyright 2020 Kartashev Vladislav
#include <mpi.h>
#include <random>
#include <algorithm>
#include <ctime>
#include "iostream"
#include "../../../modules/task_1/kartashev_v_lexical_order/lexical_order.h"


int get_sequential_operations(const char* str_1, const char* str_2, int length_1, int length_2) {
    int length = std::min(length_1, length_2);

    for (int i = 0; i < length; i++) {
        if (str_1[i] > str_2[i]) {
            return 0;
        }
        if (str_1[i] < str_2[i]) {
            return 2;
        }
    }

    if (length_1 < length_2) {
        return 2;
    }

    if (length_1 == length_2) {
        return 1;
    }

    return 0;
}

int get_parallel_operations(const char* str_1, const char* str_2, int length_1, int length_2) {
    int size, rank, length = std::min(length_1, length_2);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // std::cout << std::endl << "rank: " << rank << " lenght: " << length << std::flush << std::endl;

    if (length < size) {
        // std::cout << rank << "in if" << std::flush;
        if (rank == 0) {
            return get_sequential_operations(str_1, str_2, length_1, length_2);
        } else {
            return 0;
        }
    }

    int delta = length / size;
    int start_index;
    char* local_str_1;
    char* local_str_2;
    int local_size;

    if (rank == 0) {
        // std::cout << "length: " << length << std::flush << std::endl;
        // std::cout << "delta: " << delta << std::flush << std::endl;

        local_str_1 = new char[delta];
        local_str_2 = new char[delta];

        std::copy(str_1, str_1 + delta, local_str_1);
        std::copy(str_2, str_2 + delta, local_str_2);

        local_size = delta;
        for (int proc = 1; proc < size; proc++) {
            start_index = delta * proc;
            if (proc == size - 1) {
                local_size = length - start_index;
            }
            // std::cout << "local_size: " << local_size << std::flush << std::endl;
            MPI_Send(str_1 + start_index, local_size, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            MPI_Send(str_2 + start_index, local_size, MPI_CHAR, proc, 0, MPI_COMM_WORLD);
        }
        local_size = delta;
    } else {
        MPI_Status status;
        local_size = delta;
        if (rank == size - 1) {
            local_size = length - delta * (size - 1);
        }
        // std::cout << "local_size recive: " << local_size << std::flush << std::endl;

        local_str_1 = new char[local_size];
        local_str_2 = new char[local_size];
        MPI_Recv(local_str_1, local_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(local_str_2, local_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }
    auto local_result = static_cast<int>(get_sequential_operations(local_str_1, local_str_2, local_size, local_size));

    // std::cout << "process rank=" << rank << " get local_result: " << local_result << std::flush << std::endl;

    int batch[2] = {rank, local_result};
    int * globalResult = new int[size*2];

    // std::cout << "process rank=" << rank << " get batch: " << batch[0] << ',' << batch[1] << std::flush << std::endl;

    MPI_Gather(batch, 2, MPI_INT, globalResult + rank * 2, 2, MPI_INT, 0, MPI_COMM_WORLD);

    delete [] local_str_1;
    delete [] local_str_2;

    if (rank == 0) {
        int * sortedGlobResult = new int[size];

        for (int i = 0; i < size; i++) {
            sortedGlobResult[globalResult[i * 2]] = globalResult[i * 2 + 1];
        }

        delete [] globalResult;

        for (int i = 0; i < size; i++) {
            if (sortedGlobResult[i] == 2) {
                // std::cout << "result: " << 2 << std::flush << std::endl;
                delete [] sortedGlobResult;
                return 2;
            }
            if (sortedGlobResult[i] == 0) {
                // std::cout << "result: " << 0 << std::flush << std::endl;
                delete [] sortedGlobResult;
                return 0;
            }
        }

        delete [] sortedGlobResult;

        if (length_1 < length_2) {
            // std::cout << "result: " << 2 << std::flush << std::endl;
            return 2;
        }

        if (length_1 == length_2) {
            // std::cout << "result: " << 1 << std::flush << std::endl;
            return 1;
        }

        // std::cout << "result: " << 0 << std::flush << std::endl;
        return 0;
    }

    return 0;
}

char* get_rand_string(int lenght) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(clock()));

    char *str = new char[lenght+1];
    for (int i = 0; i < lenght; i++) {
        str[i] = static_cast<char>(gen() % 26 + 97);
    }
    str[lenght] = '\0';
    return str;
}
