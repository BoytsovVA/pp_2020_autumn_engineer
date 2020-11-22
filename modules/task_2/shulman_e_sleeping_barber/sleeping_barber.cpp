// Copyright 2020 Shulman Egor
#include <mpi.h>
#include <iostream>
#include <queue>
#include "../../../modules/task_2/shulman_e_sleeping_barber/sleeping_barber.h"

void barbershop(const int chairs) {
    MPI_Status mpi_status;
    int ProcSize, ProcRank, NumProcess;
    std::queue<int> visitors;
    int barber = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int counter = ProcSize - 2;

    while (counter) {
        MPI_Recv(&NumProcess, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_status);
        if (mpi_status.MPI_TAG == 0) {
//            std::cout << "The Barber cut process " << NumProcess << std::endl;
            barber = 0;
            --counter;
            continue;
        }
        if (mpi_status.MPI_TAG == 1) {
            if (visitors.size() > 0) {
                barber = 1;
                int current_client = visitors.front();
                MPI_Send(&current_client, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
                visitors.pop();
            }
            continue;
        }
        if (mpi_status.MPI_TAG == 2) {
            if (visitors.empty()) {
                if (barber == 0) {
                    barber = 1;
//                    std::cout << "The queue is empty and the Barber is free! "
//                                      << NumProcess << " went inside!" << std::endl;
                MPI_Send(&NumProcess, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
                continue;
                }
            }
            if ( visitors.size() < (unsigned int)chairs ) {
                visitors.push(NumProcess);
//                std::cout << NumProcess << " joined the queue. Queue size: " << visitors.size() << std::endl;
                continue;
            }
            if (visitors.size() >= (unsigned int)chairs) {
//                std::cout << "All seats are occupied. " << NumProcess
//                    << " has left. Queue size: " << visitors.size() << std::endl;
                --counter;
                continue;
            }
        }
    }
    MPI_Send(&ProcRank, 1, MPI_INT, 1, 3, MPI_COMM_WORLD);
}

void barber() {
    int ProcRank, visitor;
    MPI_Status mpi_status;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    while (true) {
        MPI_Send(&ProcRank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Recv(&visitor, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_status);
        if (mpi_status.MPI_TAG == 2) {
            MPI_Send(&visitor, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            continue;
        } else {
            return;
        }
    }
}

void client() {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Send(&ProcRank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
}

int Sleeping_Barber(const int chairs) {
    if (chairs < 0)
        throw "Error! The number of chairs is negative.";

    int ProcSize, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcSize < 3)
        return  0;

    if (ProcRank == 0) {
        barbershop(chairs);
    }
    if (ProcRank == 1) {
        barber();
    }
    if (ProcRank > 1) {
        client();
    }

    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
