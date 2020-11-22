// Copyright 2020 Kryukov Sergey
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include "../../../modules/task_2/kryukov_s_topology_star/topology_star.h"

int* getRandomVector(int size) {
    std::mt19937 random_generator;
    random_generator.seed(static_cast<unsigned int>(time(0)));
    int* vec = new int[size];
    for (int i = 0; i < size; i++) {
        vec[i] = random_generator() % 100;
    }
    return vec;
}

std::vector<int> initialVectorNodes(std::vector<int> inputV, int Size) {
    inputV.resize(Size);
    for (int j = 0; j < Size; ++j)
        inputV[j] = j;
    return inputV;
}

std::vector<int> createMasEdges(std::vector<int> inputV, int Size) {
    std::vector<int> edges;
    edges.resize((Size - 1) * 2);
    for (int j = 1; j < Size; ++j) {
        if (j != Size)
            edges[j - 1] = j;
    }
    for (int i = Size;
        i < (Size - 1) * 2; i++) {
        edges[i] = 0;
    }
    return edges;
}

std::vector<int> createMasIndex(std::vector<int> inputV, int Size) {
    std::vector<int> index;
    index.resize(Size);
    index[0] = Size - 1;
    for (int j = 1; j < Size; ++j) {
        index[j] = index[0] + j;
    }
    return index;
}


MPI_Comm createTopologyStar(MPI_Comm comm, std::vector<int> MasNodes) {
    int rank;
    int size;
    int reorder = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> nodes;
    std::vector<int> listEdges;
    std::vector<int> outArc;
    nodes = initialVectorNodes(MasNodes, size);
    listEdges = createMasEdges(nodes, size);
    outArc = createMasIndex(nodes, size);

    int * index = new int[size];
    int * edges = new int[(size - 1) * 2];
    for (int i = 0; i < size; ++i) {
        index[i] = outArc[i];
    }
    for (int i = 0; i < static_cast<int>(listEdges.size()); ++i) {
        edges[i] = listEdges[i];
    }
    MPI_Comm StarComm;
    MPI_Graph_create(comm, size, index, edges, reorder, &StarComm);

    return StarComm;
}

int* SendRecvStar(int* buf, int count, MPI_Datatype datatype,
    int dest, int send_tag, MPI_Comm comm, int source, int rank) {
    MPI_Status status;
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* message = new int[count];

    if (rank == source) {
        for (int i = 0; i < count; i++) {
            message[i] = buf[i];
        }
    }

    if ((dest != 0) && (source != 0)) {
            if (rank == source) {
                if (rank != 0) {
                    MPI_Send(&message[0], count, datatype, 0, 0, comm);
                    rank = 0;
                } else if (rank == 0) {
                    MPI_Send(&message[0], count, datatype, dest, 0, comm);
                    rank = dest;
                }
            } else {
                if (rank == 0) {
                    MPI_Recv(&message[0], count, datatype, source, 0, comm, &status);
                    rank = 0;
                    source = 0;
                } else if (rank == dest) {
                    MPI_Recv(&message[0], count, datatype, 0, 0, comm, &status);
                }
            }
    }
    if (dest == 0 && source != 0) {
            if (rank == source) {
                MPI_Send(&message[0], count, datatype, dest, 0, comm);
            } else if (rank == dest) {
                MPI_Recv(&message[0], count, datatype, source, 0, comm, &status);
            }
    }
    if (dest != 0 && source == 0) {
            if (rank == source) {
                MPI_Send(&message[0], count, datatype, dest, 0, comm);
            } else if (rank == dest) {
                MPI_Recv(&message[0], count, datatype, source, 0, comm, &status);
            }
    }
    return message;
}
