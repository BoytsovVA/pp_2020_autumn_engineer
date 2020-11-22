// Copyright 2020 Anisimova Polina
#include <mpi.h>
#include <queue>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <list>
#include "../../../modules/task_2/anisimova_p_philosophers/anisimova_p_philosopher.h"


#define FORK_REQUEST  1    //запрос
#define FORK_RESPONSE 2    //ответ
#define FORK_RELEASE  3    //реализация


void Table(int sz, int comm_sz) {
  int philosopher = 0;
  int OutBuffer[1] = {0}, InBuffer[1] = {0};

  MPI_Status status;
  std::list<int> queue;

  int *fork = new int[comm_sz - 1];

  for (int i = 0; i < comm_sz - 1; ++i) {
    fork[i] = 1;
  }

  for (int i = 0; i < (comm_sz - 1) * sz * 2; ++i) {
    MPI_Recv(InBuffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    philosopher = status.MPI_SOURCE;

    if (status.MPI_TAG == FORK_REQUEST) {
      if ((fork[philosopher % (comm_sz - 1)] == 1) && (fork[philosopher - 1] == 1)) {
        fork[philosopher % (comm_sz - 1)] = 0;
        fork[philosopher - 1] = 0;

        MPI_Send(OutBuffer, 1, MPI_REAL, philosopher, FORK_RESPONSE, MPI_COMM_WORLD);
      } else {
        queue.push_back(philosopher);
      }
    } else if (status.MPI_TAG == FORK_RELEASE) {
      fork[philosopher % (comm_sz - 1)] = 1;
      fork[philosopher - 1] = 1;

      for (size_t i = 0U; i < queue.size(); ++i) {
        philosopher = queue.front();

        if ((fork[philosopher % (comm_sz - 1)] == 1) && (fork[philosopher - 1] == 1)) {
          fork[philosopher % (comm_sz - 1)] = 0;
          fork[philosopher - 1] = 0;

          MPI_Send(OutBuffer, 1, MPI_INT, philosopher, FORK_RESPONSE, MPI_COMM_WORLD);

          queue.remove(philosopher);
        }
      }
    }
  }
  delete [] fork;
}

void Pilosophers(int *result, int *_array, const int &_number, int comm_sz, int mode, int sz) {
  int *array = _array;
  int OutBuffer[1], InBuffer[1];

  MPI_Status status;

  for (int i = 0; i < sz; ++i) {
    MPI_Send(OutBuffer, 1, MPI_INT, 0, FORK_REQUEST, MPI_COMM_WORLD);
    MPI_Recv(InBuffer, 1, MPI_INT, 0, FORK_RESPONSE, MPI_COMM_WORLD, &status);

    if (mode == 1) {
      *result *= array[i];
    } else if (mode == 2) {
      *result += array[i];
    } else {
      *result = array[i];
    }

    MPI_Send(OutBuffer, 1, MPI_INT, 0, FORK_RELEASE, MPI_COMM_WORLD);
  }
}
