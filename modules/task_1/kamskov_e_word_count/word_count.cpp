// Copyright 2020 Kamskov Eugene
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/kamskov_e_word_count/word_count.h"

std::string GenRandString(int wordCount) {
    std::srand((unsigned)std::time(0));
    auto rword = []() -> std::string {
        std::vector<std::string> words = {"aaa", "bbb", "ccc", "ddd"};

        return words[ std::rand() % (words.size() - 1)];
    };
    std::string str("");
    for (int i = 0; i < wordCount; i++) {
        str.append(rword());
        str.append(" ");
    }
    return str;
}
int ParCount(const std::string ref) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (ref.empty() && rank == 0) {
        throw EMPTY_STRING_ERROR;
    }
    int del, ost;
    del = ref.size() / size;
    ost = ref.size() % size;
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int start, end;
            start = i * del + ost;
            end = start + del;
            MPI_Send(&start, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
        }
    }
    std::string delta = "";
    int start, end;
    if (rank == 0) {
        delta = ref.substr(0, del + ost);
    } else {
        MPI_Status status;
        MPI_Recv(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        for (int i = start; i < end; i++)
            delta += ref[i];
    }
    int stringCount = 0;
    int deltaCount;
    if (rank == 0) {
        deltaCount = SimpleCount(delta);
    } else {
        deltaCount = SimpleCountDelta(delta);
    }
    if ((rank != size - 1) && (delta[delta.size()-1] == ' ')) {
        deltaCount++;
    }
    MPI_Reduce(&deltaCount, &stringCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return stringCount + 1;
}
int SimpleCountDelta(const std::string str) {
    if (str.empty()) {
        throw EMPTY_STRING_ERROR;
    }
    int s = str.size();
    int i;
    int count = 0;
    for (i = 0; i < s; i++) {
        if ((str[i] == ' ') && (str[i + 1] != ' ')) count++;
        if ((i == s - 1) && str[i] == ' ') count--;
    }
    return count;
}
int SimpleCount(const std::string str) {
    if (str.empty()) {
        throw EMPTY_STRING_ERROR;
    }
    int s = str.size();
    int i;
    int count = 0;
    for (i = 0; i < s; i++) {
        if ((str[i] == ' ') && (str[i + 1] != ' ')) count++;
        if ((i == 0) && str[i] == ' ') count--;
        if ((i == s - 1) && str[i] == ' ') count--;
    }
    return count;
}



