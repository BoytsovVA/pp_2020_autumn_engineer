  // Copyright 2020 Ivanov Yaroslav
#include <mpi.h>
#include <random>
#include <algorithm>
#include <ctime>
#include <vector>
#include "../../../modules/task_2/ivanov_y_contrast/contrast.h"

std::vector<int> CreateRandomImg(const int width, const int height) {
    std::mt19937 gen;
    int pixes = height * width;
    gen.seed(static_cast<int>(time(0)));
    std::vector<int> Arr(pixes);
    for (int i = 0; i < width * height; i++) {
        Arr[i] = gen() % 255;
    }
    return Arr;
}

std::vector<int> LocalContrast(std::vector<int> Arr, int height, int width) {
    for (int i = 0; i < height * width; i++) {
        int p = Arr[i];
        if (p < 190) {
            p -= 25;
            if (p < 0) {
                p = 0;
            }
        } else {
            p += 25;
            if (p > 255) {
                p = 255;
            }
        }
        Arr[i] = p;
    }
    return Arr;
}


std::vector<int> ParallelContrast(std::vector<int> Arr, int height, int width) {
    int process_number, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &process_number);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int del = height / process_number;
    int tmp = height % process_number;

    int pixes = height * width;
    int* sendbuf = new int[height];
    int* sendcounts = new int[process_number];
    int* displs = new int[process_number];
    std::vector<int> Ar(LocalContrast(Arr, height, width));
    for (int i = 0; i < height; i++) {
        sendbuf[i] = i;
    }
    for (int i = 0; i < process_number; i++) {
        if (tmp > 0) {
            sendcounts[i] = del + 1;
            tmp = tmp - 1;
        } else {
            sendcounts[i] = del;
        }
    }
    int justFlag = 0;
    for (int i = 0; i < process_number; i++) {
        displs[i] = justFlag;
        justFlag += sendcounts[i];
    }
    std::vector<int>recvbuf(sendcounts[process_rank]);
    MPI_Scatterv(&sendbuf[0], sendcounts, displs, MPI_INT, &recvbuf[0], sendcounts[process_rank],
        MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int>local_result_mas(recvbuf.size() * width);
    local_result_mas = LocalContrast(Arr, recvbuf.size(), width);
    int lon = recvbuf.size();
    std::vector<int>local_result(width * recvbuf.size());
    for (int i = 0; i < width * lon; i++) {
        local_result[i] = local_result_mas[i];
    }
    int* global_result = new int[pixes];
    int* sendcounts_global = new int[process_number];
    int* displs_global = new int[process_number];
    for (int i = 0; i < process_number; i++) {
        sendcounts_global[i] = width * recvbuf.size();
    }
    int justFlag1 = 0;
    for (int i = 0; i < process_number; i++) {
        displs_global[i] = justFlag1 * width;
        justFlag1 += sendcounts[i];
    }
    MPI_Gatherv(&local_result[0], sendcounts_global[process_rank], MPI_INT, &global_result[0],
        sendcounts_global, displs_global, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> global_result_mas(Ar);
    for (int i = 0; i < height * width; i++) {
        global_result[i] = global_result_mas[i];
    }
    return global_result_mas;
}

