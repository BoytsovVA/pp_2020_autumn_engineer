// Copyright 2020 Krasilnikov Alexey

#include "../../../modules/task_1/krasilnikov_a_count_sentences/count_sentences.h"
#include <mpi.h>
#include <ctime>
#include <random>
#include <string>


std::string getRandomString(const size_t size) {
  std::mt19937 generator;
  std::string str(size, '_');
  for (auto &symbol : str) {
    if (generator() % 10 == 0) {
      symbol = '.';
    } else {
      symbol = static_cast<char>(generator() % 128);
    }
  }
  return str;
}

uint32_t getCountSentencesSequential(const std::string& str) {
  uint32_t count_sentences = 0;
  for (const auto symbol : str) {
    if (symbol == '.') {
      ++count_sentences;
    }
  }
  return count_sentences;
}

uint32_t getCountSentencesParallel(const std::string& str, const size_t size_str) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  uint32_t total_count = 0;
  std::string local_str;
  const uint32_t delta = size_str / size;
  const uint32_t remain = size_str % size;
  if (rank == 0) {
    for (size_t process = 0; process < remain; ++process) {
      MPI_Send(&str[0] + process * (delta + 1), delta + 1, MPI_CHAR, process + 1, 0, MPI_COMM_WORLD);
    }
    for (size_t process = remain; static_cast<int>(process) < size - 1; ++process) {
      MPI_Send(&str[0] + process * delta + remain, delta, MPI_CHAR, process + 1, 0, MPI_COMM_WORLD);
    }
    local_str = str.substr((size - 1) * delta + remain);
  } else {
    MPI_Status status;
    if (rank <= static_cast<int>(remain)) {
      local_str.resize(delta + 1);
      MPI_Recv(&local_str[0], delta + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    } else {
      local_str.resize(delta);
      MPI_Recv(&local_str[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }
  }
  uint32_t local_count = getCountSentencesSequential(local_str);
  MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return total_count;
}
