// Copyright 2020 Makhov Pavel
#include <mpi.h>
#include <string>
#include <iostream>
#include <ctime>
#include <random>
#include "../../../modules/task_1/makhov_p_sim_v_stroke/sim_v_stroke.h"


std::string RanStr(int string) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::string ret_string;
    ret_string.resize(string);
    for (auto& sim : ret_string) {
        sim = static_cast<char>(gen() % 100);
    }
    return ret_string;
}

int FrequencySec(const std::string& curr_str, char sim, int string) {
    int frequency_count = 0;
    for (int j = 0; j < string; ++j) {
        if (curr_str[j] == sim) ++frequency_count;
    }
    return frequency_count;
}

int FrequencyParallel(const std::string& gen_str, char sim, int string) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (string < size) {
        if (rank == 0)
            return FrequencySec(gen_str, sim, string);
        else
            return 0;
    }
    const int delta = string / size;
    int real_size;
    if (rank == size - 1)
        real_size = string - (size - 1) * delta;
    else
        real_size = delta;

    if (rank == 0 && size > 1) {
        for (int proc_num = 1; proc_num < size - 1; ++proc_num) {
            MPI_Send(&gen_str[proc_num * delta], delta,
                MPI_CHAR, proc_num, 0, MPI_COMM_WORLD);
        }
        MPI_Send(&gen_str[(size - 1) * delta], string - (size - 1) * delta,
            MPI_CHAR, size - 1, 0, MPI_COMM_WORLD);
    }
    int gen_sum = 0;
    int local_sum = 0;
    std::string local_string;
    local_string.resize(delta);
    if (rank == 0) {
        local_sum = FrequencySec(gen_str, sim, delta);
    } else {
        MPI_Status status;
        MPI_Recv(&local_string[0], real_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        local_sum = FrequencySec(local_string, sim, real_size);
    }

    MPI_Reduce(&local_sum, &gen_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return gen_sum;
}
