// Copyright 2020 Chesnokov Artyom
#include <time.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <random>
#include "./scatter.h"

TEST(Task_2, Test_Scatter_Naive_Works) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int test_data[1000];
    if (rank == 0) {
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < 1000; i++) {
            test_data[i] = gen() % 128;
        }
    }
    int delta = 1000 / size;
    int *recv1, *recv2;
    recv1 = new int[delta];
    recv2 = new int[delta];

    MPI_Scatter(test_data, delta, MPI_INT, recv1, delta, MPI_INT, 0, MPI_COMM_WORLD);
    EXPECT_NO_THROW(My_Scatter_Naive(test_data, delta, MPI_INT,
        recv2, delta, MPI_INT, 0, MPI_COMM_WORLD));

    for (int i = 0; i < delta; i++) {
        EXPECT_EQ(recv1[i], recv2[i]);
    }

    delete[] recv1;
    delete[] recv2;
}

TEST(Task_2, Test_Scatter_By_Tree_Works_With_Root_Eq_0) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int test_data[1000];
    if (rank == 0) {
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < 1000; i++) {
            test_data[i] = gen() % 128;
        }
    }
    int delta = 1000 / size;
    int *recv1, *recv2;
    recv1 = new int[delta];
    recv2 = new int[delta];

    MPI_Scatter(test_data, delta, MPI_INT, recv1, delta, MPI_INT, 0, MPI_COMM_WORLD);
    EXPECT_NO_THROW(My_Scatter_By_Tree(test_data, delta, MPI_INT,
        recv2, delta, MPI_INT, 0, MPI_COMM_WORLD));

    for (int i = 0; i < delta; i++) {
        EXPECT_EQ(recv1[i], recv2[i]);
    }

    delete[] recv1;
    delete[] recv2;
}

TEST(Task_2, Test_Scatter_By_Tree_Works_With_Root_Neq_0) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int test_data[1000];
    if (rank == size/2) {
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < 1000; i++) {
            test_data[i] = gen() % 128;
        }
    }
    int delta = 1000 / size;
    int *recv1, *recv2;
    recv1 = new int[delta];
    recv2 = new int[delta];

    MPI_Scatter(test_data, delta, MPI_INT, recv1, delta, MPI_INT, size / 2, MPI_COMM_WORLD);
    EXPECT_NO_THROW(My_Scatter_By_Tree(test_data, delta, MPI_INT,
        recv2, delta, MPI_INT, size / 2, MPI_COMM_WORLD));

    for (int i = 0; i < delta; i++) {
        EXPECT_EQ(recv1[i], recv2[i]);
    }

    delete[] recv1;
    delete[] recv2;
}

TEST(Task_2, Test_Scatter_By_Tree_Works_With_Double) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double test_data[1000];
    if (rank == 0) {
        for (int i = 0; i < 1000; i++) {
            test_data[i] = 2.3 + i;
        }
    }
    int delta = 1000 / size;
    double *recv1, *recv2;
    recv1 = new double[delta];
    recv2 = new double[delta];

    MPI_Scatter(test_data, delta, MPI_DOUBLE, recv1, delta, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    EXPECT_NO_THROW(My_Scatter_By_Tree(test_data, delta, MPI_DOUBLE,
        recv2, delta, MPI_DOUBLE, 0, MPI_COMM_WORLD));

    for (int i = 0; i < delta; i++) {
        EXPECT_EQ(recv1[i], recv2[i]);
    }

    delete[] recv1;
    delete[] recv2;
}

TEST(Task_2, Test_Scatter_By_Tree_Works_With_Float) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    float test_data[1000];
    if (rank == 0) {
        for (int i = 0; i < 1000; i++) {
            test_data[i] = 2.1f + i;
        }
    }
    int delta = 1000 / size;
    float *recv1, *recv2;
    recv1 = new float[delta];
    recv2 = new float[delta];

    MPI_Scatter(test_data, delta, MPI_FLOAT, recv1, delta, MPI_FLOAT, 0, MPI_COMM_WORLD);
    EXPECT_NO_THROW(My_Scatter_By_Tree(test_data, delta, MPI_FLOAT,
        recv2, delta, MPI_FLOAT, 0, MPI_COMM_WORLD));

    for (int i = 0; i < delta; i++) {
        EXPECT_EQ(recv1[i], recv2[i]);
    }

    delete[] recv1;
    delete[] recv2;
}

TEST(Task_2, Time_Comparison_On_Vector_Sum) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int test_data[100000];
    if (rank == 0) {
        std::mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < 100000; i++) {
            test_data[i] = gen() % 128;
        }
    }

    int delta = 100000 / size;
    int *recv;
    recv = new int[delta];


    int res1, subres;
    double t1, t2, t3, t4;

    // MPI
    t1 = MPI_Wtime();
    MPI_Scatter(test_data, delta, MPI_INT, recv, delta, MPI_INT, 0, MPI_COMM_WORLD);
    subres = 0;
    for (int i = 0; i < delta; i++) {
        subres += recv[i];
    }
    MPI_Reduce(&subres, &res1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "MPI realisation: (sum = " << res1 << ")";
        t2 = MPI_Wtime();
        std::cout << " time = " << t2 - t1 << std::endl;
    }
    // My with tree
    t2 = MPI_Wtime();
    My_Scatter_By_Tree(test_data, delta, MPI_INT, recv, delta, MPI_INT, 0, MPI_COMM_WORLD);
    subres = 0;
    for (int i = 0; i < delta; i++) {
        subres += recv[i];
    }
    MPI_Reduce(&subres, &res1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "My realisation with tree: (sum = " << res1 << ")";
        t3 = MPI_Wtime();
        std::cout << " time = " << t3 - t2 << std::endl;
    }
    // My naive
    t3 = MPI_Wtime();
    My_Scatter_Naive(test_data, delta, MPI_INT, recv, delta, MPI_INT, 0, MPI_COMM_WORLD);
    subres = 0;
    for (int i = 0; i < delta; i++) {
        subres += recv[i];
    }
    MPI_Reduce(&subres, &res1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        std::cout << "My naive realisation: (sum = " << res1 << ")";
        t4 = MPI_Wtime();
        std::cout << " time = " << t4 - t3 << std::endl;
    }
    delete[] recv;
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
