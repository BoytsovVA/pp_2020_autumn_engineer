  // Copyright 2020 Zlobin George
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./gather.h"

TEST(Parallel_Operations_MPI, Get_MPI_Gather_Result) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank * 2;

    MPI_Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Result: ";
        for (int i = 0; i < size; i++) {
            std::cout << res[i] << " ";
        }
        std::cout << "\n";
    }
}

TEST(Parallel_Operations_MPI, Get_Gather_Result) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank * 2;

    Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Result: ";
        for (int i = 0; i < size; i++) {
            std::cout << res[i] << " ";
        }
        std::cout << "\n";
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_No_Throw) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank;

    ASSERT_NO_THROW(Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, 0, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Test_Gather_MPI_SUCCESS) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank;

    ASSERT_EQ(MPI_SUCCESS, Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, 0, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Test_Gather_MPI_ERR_COUNT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank;

    ASSERT_EQ(MPI_ERR_COUNT, Gather(&local_res, 1, MPI_INT, &res[0], 2, MPI_INT, 0, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Test_Gather_MPI_ERR_TYPE) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank;

    ASSERT_EQ(MPI_ERR_TYPE, Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Test_Gather_Same_Result_As_MPI_Gather) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res_Gather(size);
    std::vector<int> res_MPI_Gather(size);
    int local_res;

    local_res = rank * 2;

    Gather(&local_res, 1, MPI_INT, &res_Gather[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_res, 1, MPI_INT, &res_MPI_Gather[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(res_MPI_Gather, res_Gather);
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_INT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank * 2;

    Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(i * 2, res[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_FLOAT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<float> res(size);
    float local_res;

    local_res = rank / 10.f * 2;

    Gather(&local_res, 1, MPI_FLOAT, &res[0], 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double error = pow(10.0, -8);
        for (int i = 0; i < size; i++) {
            ASSERT_NEAR(i / 10.f * 2, res[i], error);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_DOUBLE) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> res(size);
    double local_res;

    local_res = rank / 10.0 * 2;

    Gather(&local_res, 1, MPI_DOUBLE, &res[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double error = pow(10.0, -8);
        for (int i = 0; i < size; i++) {
            ASSERT_NEAR(i / 10.0 * 2, res[i], error);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_to_mid) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank * 2;

    Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, size / 2, MPI_COMM_WORLD);

    if (rank == size / 2) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(i * 2, res[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_to_last) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> res(size);
    int local_res;

    local_res = rank * 2;

    Gather(&local_res, 1, MPI_INT, &res[0], 1, MPI_INT, size - 1, MPI_COMM_WORLD);

    if (rank == size - 1) {
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(i * 2, res[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_Gather_With_Vectors) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count = 10;
    int* res = new int[size * count];
    int* local_res = new int[count];

    if (rank == 0) {
        for (int i = 0; i < size * count; i++) {
            res[i] = i;
        }
    }

    MPI_Scatter(&res[0], count, MPI_INT, &local_res[0], count, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        local_res[i] = local_res[i] * 2;
    }

    Gather(&local_res[0], count, MPI_INT, &res[0], count, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size * count; i++) {
            ASSERT_EQ(i * 2, res[i]);
        }
    }

    delete[] res;
    delete[] local_res;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
