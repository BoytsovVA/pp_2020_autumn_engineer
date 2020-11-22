// Copyright 2020 Pasukhin Dmitry

#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_2/pasukhin_d_hypercube/hypercube.h"

MPI_Request globreq;

int randPath(int min, int max) {
  if (min < 0)
    min = 0;
  if (max < 0)
    max = 0;
  std::mt19937 generator;
  std::uniform_int_distribution<> distribution{ min, max };
  return distribution(generator);
}
int powerHyperCube(const int _size) {
  int power = 1, count = 0;
  while (power < _size) {
    power <<= 1;
    count++;
  }

  return count;
}

void computeRoude(const int rank, const int power, int* collection) {
  for (int i = 1, j = 0; j < power; i <<= 1, j++) {
    if (rank & i)
      collection[j] = -i;
    else
      collection[j] = i;
  }
}

int reciver_nearby(const int _rank, const int _reciever, const int _power, const int* _dir) {
  int res = -1;
  for (int i = 0; i < _power; ++i) {
    int neigb = _rank + _dir[i];
    if (neigb == _reciever) {
      return i;
    }
  }
  return res;
}

int findRoude(const int rank_my, const int rank_past, const int power) {
  int* coll = new int[power];
  computeRoude(rank_past, power, coll);
  int roude = 0;
  for (int i = 0; i < power; ++i) {
    if (rank_past + coll[i] == rank_my) {
      roude = (i + 1) % power;
      break;
    }
  }
  delete[] coll;
  return roude;
}

bool checkProc(const int size) {
  int pow = 1;
  for (int i = 0; i <= 10; ++i) {
    if (size == pow)
      return true;
    pow *= 2;
  }
  return false;
}

int hyperTopoRecv(const int rank, const int size, double* collect) {
  int mysize = 0, myrank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &mysize);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  bool isCorr = checkProc(mysize);
  if (rank == myrank && isCorr) {
    MPI_Status stat;
    MPI_Recv(&collect[0], size, MPI_DOUBLE, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &stat);
    double fin = 0.;
    for (int i = 0; i < mysize; ++i) {
      if (i == myrank || i == stat.MPI_SOURCE) {
        continue;
      }
      MPI_Send(&fin, 1, MPI_DOUBLE, i, mysize + 1, MPI_COMM_WORLD);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  return 0;
}

bool huperCubeTopology(std::vector<double> _message, const int _receiver, const int _sender) {
  int size = 0, rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  bool isCorr = checkProc(size);
  if (_sender >= size || _receiver >= size || !isCorr) {
    return 0;
  }

  if (_receiver == _sender ||
      size == 1) {
    double* arr = new double[_message.size()];
    for (int i = 0; i < static_cast<int>(_message.size()); ++i)
      arr[i] = _message[i];
    MPI_Isend(arr, _message.size(), MPI_DOUBLE, _receiver, _receiver, MPI_COMM_WORLD, &globreq);
    delete[] arr;
    return 0;
  }

  if (rank == _receiver) {
    return 0;
  }

  int power = powerHyperCube(size);
  int* comms_offsets = new int[power];
  computeRoude(rank, power, comms_offsets);

  int near_path;
  int Go = 1;

  while (Go != -1) {
    if (rank == _sender && Go == 1) {
      Go++;
      near_path = reciver_nearby(rank, _receiver, power, comms_offsets);

      if (near_path != -1) {
        MPI_Send(&_message[0], _message.size(), MPI_DOUBLE, rank + comms_offsets[near_path], _receiver, MPI_COMM_WORLD);
        Go = -1;
      } else {
        MPI_Send(&_message[0], _message.size(), MPI_DOUBLE, rank + comms_offsets[0], _receiver, MPI_COMM_WORLD);
      }
    } else {
      int to = 0;
      MPI_Status stat;
      int count;
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
      MPI_Get_count(&stat, MPI_DOUBLE, &count);
      double* mess = new double[count];
      near_path = reciver_nearby(rank, _receiver, power, comms_offsets);
      MPI_Recv(&mess[0], count, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
      if (stat.MPI_TAG == size + 1) {
        Go = -1;
      } else if (near_path == -1) {
        to = findRoude(rank, stat.MPI_SOURCE, power);
        MPI_Send(&mess[0], count, MPI_DOUBLE, rank + comms_offsets[to], _receiver, MPI_COMM_WORLD);
      } else {
        MPI_Send(&mess[0], count, MPI_DOUBLE, rank + comms_offsets[near_path], _receiver, MPI_COMM_WORLD);
        Go = -1;
      }
      delete[] mess;
    }
  }
  delete[] comms_offsets;

  return 0;
}
