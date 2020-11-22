// Copyright 2020 Novikov Danil
#ifndef MODULES_TASK_2_NOVIKOV_D_ALLREDUCE_ALLREDUCE_H_
#define MODULES_TASK_2_NOVIKOV_D_ALLREDUCE_ALLREDUCE_H_

#include <vector>
#include <random>
#include <ctime>

template<typename Type>
std::vector<Type> getRandomVector(int sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<Type> vec(sz);
    if (typeid(Type) == typeid(int)) {
        for (int i = 0; i < sz; i++) { vec[i] = gen() % 100 * pow(-1, i);}
    } else {
        for (int i = 0; i < sz; i++) { vec[i] = (Type(gen() % 100) + gen() % 100 * 0.01) * pow(-1, i); }
    }
    return vec;
}

namespace my {
    void MPI_Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
}

#endif  // MODULES_TASK_2_NOVIKOV_D_ALLREDUCE_ALLREDUCE_H_
