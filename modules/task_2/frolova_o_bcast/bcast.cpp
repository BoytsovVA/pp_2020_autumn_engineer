// Copyright 2018 Nesterov Alexander
#include "../../../modules/task_2/frolova_o_bcast/bcast.h"
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

std::vector<int> getRandomArray(int size) {
    std::vector<int> array(size);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < size; i++)
       array[i] = gen() % 100;
    return array;
}

std::vector<double> getRandomDoubleArray(int size) {
    std::vector<double> array(size);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    for (int i = 0; i < size; i++)
       array[i] = static_cast<double>(urd(gen));
    return array;
}

std::vector<float> getRandomFloatArray(int size) {
    std::vector<float> array(size);
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 100);
    for (int i = 0; i < size; i++)
       array[i] = static_cast<float>(urd(gen));
    return array;
}

int checkSum(std::vector<int> array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += array[i];
    return sum;
}

double checkDoubleSum(std::vector<double> array, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
        }
    return sum;
}

float checkFloatSum(std::vector<float> array, int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += array[i];
        }
    return sum;
}

void my_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(comm);
    int index = (rank - root + size) % size + 1;
    MPI_Barrier(comm);
    if (rank != root) {
       MPI_Status status;
       MPI_Recv(buffer, count, datatype, (index / 2 - 1 + root) % size, 0, comm, &status);
    }
    if (2 * index <= size) {
      MPI_Send(buffer, count, datatype, (index * 2 - 1 + root) % size, 0, comm);
    }
    if (2 * index + 1 <= size) {
       MPI_Send(buffer, count, datatype, (index * 2 + root) % size, 0, comm);
    }
}

void getSum(std::vector<int> array, int size, int *sum, int root) {
    *sum = 0;
    int count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = size / count;
    if (size % count != 0)
       delta++;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != root) array.resize(size);
    MPI_Barrier(MPI_COMM_WORLD);
    my_Bcast(array.data(), size, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != root) {
       int sum1 = 0;
       for (int i = delta * rank; i < delta * (rank + 1); i++) {
           if (i < size) {
              sum1 += array[i];
           }
       }
       MPI_Send(&sum1, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
       int sum2 = 0;
       for (int i = delta * root; i < delta * (root + 1); i++)
           *sum = *sum + array[i];
       for (int i = 0; i < count; i++) {
           if (i != root) {
              MPI_Status status;
              MPI_Recv(&sum2, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
              *sum += sum2;
           }
       }
    }
}

void getDoubleSum(std::vector<double> array, int size, double *sum, int root) {
    *sum = 0.0;
    int count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = size / count;
    if (size % count != 0)
       delta++;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != root) array.resize(size);
    MPI_Barrier(MPI_COMM_WORLD);
    my_Bcast(array.data(), size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != root) {
       double sum1 = 0.0;
       for (int i = delta * rank; i < delta * (rank + 1); i++) {
           if (i < size) {
              sum1 += array[i];
           }
       }
       MPI_Send(&sum1, 1, MPI_DOUBLE, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
       double sum2 = 0.0;
       for (int i = delta * root; i < delta * (root + 1); i++)
           *sum = *sum + array[i];
       for (int i = 0; i < count; i++) {
           if (i != root) {
              MPI_Status status;
              MPI_Recv(&sum2, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
              *sum += sum2;
           }
       }
    }
}

void getFloatSum(std::vector<float> array, int size, float *sum, int root) {
    *sum = 0.0f;
    int count, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = size / count;
    if (size % count != 0)
       delta++;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != root) array.resize(size);
    MPI_Barrier(MPI_COMM_WORLD);
    my_Bcast(array.data(), size, MPI_FLOAT, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank != root) {
       float sum1 = 0.0f;
       for (int i = delta * rank; i < delta * (rank + 1); i++) {
           if (i < size) {
              sum1 += array[i];
           }
       }
       MPI_Send(&sum1, 1, MPI_FLOAT, root, 0, MPI_COMM_WORLD);
    }
    if (rank == root) {
       float sum2 = 0.0f;
       for (int i = delta * root; i < delta * (root + 1); i++)
           *sum = *sum + array[i];
       for (int i = 0; i < count; i++) {
           if (i != root) {
              MPI_Status status;
              MPI_Recv(&sum2, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
              *sum += sum2;
           }
       }
    }
}

void difOfTime(int count, int root) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array = getRandomArray(count);
    double timeMyBcast1, timeMyBcast2, timeMPIBcast1, timeMPIBcast2;
    if (rank == root)
       timeMyBcast1 = MPI_Wtime();
    my_Bcast(array.data(), count, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == root)
       timeMyBcast2 = MPI_Wtime();
    if (rank == root)
       timeMPIBcast1 = MPI_Wtime();
    MPI_Bcast(array.data(), count, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == root) {
       timeMPIBcast2 = MPI_Wtime();
       double resMyBcast = timeMyBcast2 - timeMyBcast1;
       double resMPIBcast = timeMPIBcast2 - timeMPIBcast1;
       if ( resMPIBcast < resMyBcast ) {
          std::cout << "MPIBcast faster than myBcast on " << resMyBcast - resMPIBcast << "\n";
       } else {
          std::cout << "myBcast faster than MPIBcast on " << resMPIBcast - resMyBcast << "\n";
       }
    }
}
