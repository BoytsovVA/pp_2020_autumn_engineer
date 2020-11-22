// Copyright 2020 Tronin Dmitry
#include <mpi.h>
#include <ctime>
#include <queue>
#include <random>
#include "../../../modules/task_2/tronin_d_sleeping_hairdresser/sleeping_hairdresser.h"

#ifdef __linux
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

void CrossPlatformSleep(size_t sleepMs) {
#ifdef __linux
    usleep(sleepMs * 1000);
#endif
#ifdef __WIN32
    Sleep(sleepMs);
#endif
}

void SleepingBarber(size_t queue_capacity) {
    int comm_size, comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if (comm_size < 4) {
        return;
    }
    if (comm_rank == 0) {
        WaitingRoom(queue_capacity);
    } else if (comm_rank == 1) {
        Barber();
    } else {
        Customer(comm_rank);
    }
}

void WaitingRoom(size_t queue_capacity) {
    static const size_t kTimeout = 1;

    size_t free_seats = queue_capacity;
    std::queue<int> wait_list;

    MPI_Request barber_request = MPI_REQUEST_NULL;
    int barber_ready = false;
    MPI_Status barber_status;
    bool terminate = false;
    while (!terminate || !wait_list.empty()) {
        std::time_t current_time = std::time(nullptr);
        int data_received = false;
        int current_customer;
        MPI_Request request;
        MPI_Irecv(&current_customer, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
        MPI_Status status;
        MPI_Test(&request, &data_received, &status);

        while (!data_received) {
            if (std::difftime(std::time(nullptr), current_time) > kTimeout) {
                if (free_seats < queue_capacity) {
                    break;
                } else {
                    terminate = true;
                    break;
                }
            }
            CrossPlatformSleep(10);
            MPI_Test(&request, &data_received, &status);
        }

        if (!data_received) {
            MPI_Cancel(&request);
            MPI_Request_free(&request);
        }

        if (data_received) {
            if (free_seats > 0) {
                wait_list.push(status.MPI_SOURCE);
                MPI_Send(nullptr, 0, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                --free_seats;
            } else {
                MPI_Send(nullptr, 0, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
            }
        }

        if (!wait_list.empty()) {
            if (barber_request == MPI_REQUEST_NULL) {
                MPI_Issend(&wait_list.front(), 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &barber_request);
                MPI_Send(nullptr, 0, MPI_INT, wait_list.front(), 1, MPI_COMM_WORLD);
                ++free_seats;
            }
            MPI_Test(&barber_request, &barber_ready, &barber_status);
            if (barber_ready) {
                wait_list.pop();
            }
        }
    }
    int tmp = 0;
    MPI_Send(&tmp, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
}

void Barber() {
    while (true) {
        int customer_rank;
        MPI_Status status;
        MPI_Recv(&customer_rank, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == 1) {
            int p = 4;
            MPI_Send(&p, 1, MPI_INT, customer_rank, 0, MPI_COMM_WORLD);
            CrossPlatformSleep(200);
        } else {
            break;
        }
    }
}

void Customer(int rank) {
    MPI_Status status;
    std::random_device random_device;
    std::mt19937 random_generator(random_device());
    CrossPlatformSleep(std::uniform_int_distribution<size_t>(0, 1000)(random_generator));
    MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(nullptr, 0, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if (status.MPI_TAG == 1) {
        MPI_Recv(nullptr, 0, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int received;
        MPI_Recv(&received, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    }
}
