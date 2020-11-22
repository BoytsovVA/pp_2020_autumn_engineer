// Copyright 2020 Tronin Dmitry
#include "../../modules/task_1/tronin_d_min_row_element/min_row_element.h"
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <iostream>


std::vector<int32_t> getRandomVector(size_t size) {
  std::mt19937 rand_gen(54612);
  std::uniform_int_distribution<int> dist(-1000, 1000);
  std::vector<int32_t> vector(size*size);

  for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
          vector[i*size + j] = dist(rand_gen);
      }
  }

  return vector;
}

std::vector<int32_t> getMinRowElementSequential(
        const std::vector<int32_t>& vector,
        size_t size) {
    std::vector<int32_t> min_row_element_vector(size);
    for (size_t row = 0; row < size; ++row) {
        min_row_element_vector[row] =
                *std::min_element(vector.begin() + row*size,
                                  vector.begin() + row*size + size);
    }

    return min_row_element_vector;
}

std::vector<int32_t> getMinRowElementParallel(
        const std::vector<int32_t>& vector,
        size_t size) {
    if (size == 0) {
        return std::vector<int32_t>();
    }

    int comm_size, comm_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    int rec_size  = size / comm_size;
    if (size % comm_size) {
        ++rec_size;
    }

    std::vector<int32_t> rec_vector(size * rec_size);

    std::vector<int> send_size(comm_size,
                               size * (size / comm_size)), send_pos(comm_size);

    for (size_t i = 0; i < size % comm_size; ++i) {
        send_size[i] += size;
    }

    int pos = 0;
    for (size_t i = 0; i < static_cast<size_t>(comm_size); ++i) {
            send_pos[i] = pos;
            pos += send_size[i];
    }

    MPI_Scatterv(vector.data(), &send_size[0],  send_pos.data(), MPI_INT32_T,
                 rec_vector.data(), static_cast<int>(send_size[comm_rank] * size),
                 MPI_INT32_T, 0, MPI_COMM_WORLD);

    std::vector<int32_t> min_element_row(send_size[comm_rank] / size);

    for (size_t i = 0;
         i < static_cast<size_t>(send_size[comm_rank] / size); ++i) {
        auto min_element_iter = std::min_element(
                rec_vector.begin() + i * size,
                rec_vector.begin() + (i + 1) * size);
        min_element_row[i] = *min_element_iter;
    }

    std::vector<int32_t> min_element_together(size);

    std::vector<int> rec_size_v(comm_size, size / comm_size),
            rec_pos(comm_size);

    for (size_t i = 0; i < size % comm_size; ++i) {
        rec_size_v[i] += 1;
    }

    pos = 0;
    for (size_t i = 0; i < static_cast<size_t>(comm_size); ++i) {
            rec_pos[i] = pos;
            pos += rec_size_v[i];
    }

    MPI_Gatherv(min_element_row.data(),  rec_size_v[comm_rank], MPI_INT32_T,
                min_element_together.data(), rec_size_v.data(), rec_pos.data(),
                MPI_INT32_T, 0, MPI_COMM_WORLD);

    return min_element_together;
}
