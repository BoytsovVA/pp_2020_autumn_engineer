// Copyright 2020 Sozinov Alex

#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>

#include "../../modules/task_1/sozinov_a_max_row_matrix/max_row_matrix.h"

std::vector<int> GenerateMatrix(int sizeRow, int countRow, int min, int max) {
    std::vector<int> vec(sizeRow * countRow);
    std::random_device dev;
    std::mt19937 ger(dev());
    std::uniform_int_distribution<> dist(min, max);

    for (int j = 0; j < sizeRow * countRow; ++j) {
        vec[j] = dist(ger);
    }
    return vec;
}

std::vector<int> GetSequentialMax(const std::vector<int> &vect, int sizeRow, int countRow) {
    std::vector<int> vectMax(countRow);
    for (int i = 0; i < countRow; ++i) {
        vectMax[i] = *(std::max_element(vect.begin() + i * sizeRow, vect.begin() + (i + 1) * sizeRow));
    }
    return vectMax;
}

std::vector<int> GetParalMax(const std::vector<int> &vect, int sizeRow, int countRow) {
    if (!sizeRow || !countRow)
        return std::vector<int>();

    int ProcCount;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
    std::vector<int> vectMax(countRow);
    int offset = countRow / ProcCount;
    std::vector<int> sendSize(ProcCount, offset * sizeRow);
    for (int i = 0; i < countRow % ProcCount; ++i)
        sendSize[i] += sizeRow;

    if (countRow % ProcCount)
        ++offset;
    std::vector<int> recVect(offset * sizeRow);
    std::vector<int> sendOffset(ProcCount);
    for (int pos = 0, i = 0; i < ProcCount; ++i) {
        sendOffset[i] = pos;
        pos += sendSize[i];
    }

    MPI_Scatterv(&vect[0], &sendSize[0], &sendOffset[0], MPI_INT,
        &recVect[0], sendSize[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> recSize(ProcCount, countRow / ProcCount);
    std::vector<int> recOffset(ProcCount);

    for (int i = 0; i < countRow % ProcCount; ++i)
        ++recSize[i];
    for (int pos = 0, i = 0; i < ProcCount; ++i) {
        recOffset[i] = pos;
        pos += recSize[i];
    }
    std::vector<int> locMax(recSize[ProcRank]);
    for (int i = 0; i < recSize[ProcRank]; ++i) {
        locMax[i] = *(std::max_element(recVect.begin() + i * sizeRow, recVect.begin() + (i + 1) * sizeRow));
    }

    MPI_Gatherv(&locMax[0], recSize[ProcRank], MPI_INT,
        &vectMax[0], &recSize[0], &recOffset[0], MPI_INT, 0, MPI_COMM_WORLD);

    return vectMax;
}
