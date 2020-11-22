// Copyright 2020 Kokh Vladislav
#include <mpi.h>
#include <queue>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/kokh_v_producers_and_consumers/producers_and_consumers.h"

double sleep_time = 0.01;

void sleepFunction(double seconds) {
    double t1 = MPI_Wtime();
    while (MPI_Wtime() - t1 < seconds) {}
}

int mainpar(int it) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    MPI_Request request;
    int result;
    int last_consumer = -1;

    std::queue<int> queue_of_elements;
    while (it != 0) {
        if (last_consumer == -1) {
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        } else {
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
        }
        --it;
        if (status.MPI_TAG == 3) {
            queue_of_elements.push(result);
            if (last_consumer != -1) {
                int el = queue_of_elements.front();
                queue_of_elements.pop();
                MPI_Isend(&el, 1, MPI_INT, last_consumer, 4, MPI_COMM_WORLD, &request);
                MPI_Request_free(&request);
                last_consumer = -1;
            }
        } else {
            if (!queue_of_elements.empty()) {
                int el = queue_of_elements.front();
                queue_of_elements.pop();
                MPI_Isend(&el, 1, MPI_INT, status.MPI_SOURCE, 4, MPI_COMM_WORLD, &request);
                MPI_Request_free(&request);
            } else {
                last_consumer = status.MPI_SOURCE;
            }
        }
    }
    return queue_of_elements.size();
}

void produce() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sleepFunction(sleep_time * rank);
    MPI_Send(&rank, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
}

void consume() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    sleepFunction(sleep_time * rank);
    MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

    int el;
    MPI_Status status;
    MPI_Recv(&el, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
}
