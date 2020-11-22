// Copyright 2020 Nasedkin Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./scatter.h"
// mpiexec -n 4 "C:\Labs\MPI2020\pp_2020_autumn_engineer\build\bin\nasedkin_mpi_scatter_mpi.exe"


TEST(Parallel_Operations_MPI, Can_Scattering) {
  int proc_rank, proc_size, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 8, pack_size = src_size / proc_size;

  std::vector<int> src(src_size);
  std::vector<int> local(pack_size);

  EXPECT_NO_THROW(MyScatter(&src[0], 1, MPI_INT, &local[0], 1, MPI_INT, root, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Can_Scattering_Correct_Int) {
  int proc_rank, proc_size, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 8, pack_size = src_size / proc_size;

  std::vector<int> src(src_size);
  std::vector<int> local(pack_size);
  std::vector<int> gathered(src_size);

  if (proc_rank == root) {
    for (int i = 0; i < src_size; i++) {
      src[i] = i;
    }
  }

  // MPI_Scatter(&src[0], pack_size, MPI_INT, &local[0], pack_size, MPI_INT, root, MPI_COMM_WORLD);
  MyScatter(&src[0], pack_size, MPI_INT, &local[0], pack_size, MPI_INT, root, MPI_COMM_WORLD);
  MPI_Gather(&local[0], pack_size, MPI_INT, &gathered[0], pack_size, MPI_INT, root, MPI_COMM_WORLD);

  EXPECT_EQ(src, gathered);
}

TEST(Parallel_Operations_MPI, Can_Scattering_Correct_Double) {
  int proc_rank, proc_size, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 8, pack_size = src_size / proc_size;

  std::vector<double> src(src_size);
  std::vector<double> local(pack_size);
  std::vector<double> gathered(src_size);

  if (proc_rank == root) {
    for (int i = 0; i < src_size; i++) {
      src[i] = i;
    }
  }

  MyScatter(&src[0], pack_size, MPI_DOUBLE, &local[0], pack_size, MPI_DOUBLE, root, MPI_COMM_WORLD);
  MPI_Gather(&local[0], pack_size, MPI_DOUBLE, &gathered[0], pack_size, MPI_DOUBLE, root, MPI_COMM_WORLD);
  EXPECT_EQ(src, gathered);
}

TEST(Parallel_Operations_MPI, Can_Scattering_Correct_Float) {
  int proc_rank, proc_size, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 20, pack_size = src_size / proc_size;

  std::vector<float> src(src_size);
  std::vector<float> local(pack_size);
  std::vector<float> gathered(src_size);

  if (proc_rank == root) {
    for (int i = 0; i < src_size; i++) {
      src[i] = i;
    }
  }

  MyScatter(&src[0], pack_size, MPI_FLOAT, &local[0], pack_size, MPI_FLOAT, root, MPI_COMM_WORLD);
  MPI_Gather(&local[0], pack_size, MPI_FLOAT, &gathered[0], pack_size, MPI_FLOAT, root, MPI_COMM_WORLD);
  EXPECT_EQ(src, gathered);
}

TEST(Parallel_Operations_MPI, Throw_When_Different_Pack_Size) {
  int proc_rank, proc_size, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 8, pack_size = src_size / proc_size;

  std::vector<int> src(src_size);
  std::vector<int> local(pack_size);

  ASSERT_ANY_THROW(MyScatter(&src[0], 1, MPI_INT, &local[0], 2, MPI_INT, root, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Throw_When_Negative_Root_Number) {
  int proc_rank, proc_size, root = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 8, pack_size = src_size / proc_size;

  std::vector<int> src(src_size);
  std::vector<int> local(pack_size);

  ASSERT_ANY_THROW(MyScatter(&src[0], pack_size, MPI_INT, &local[0], pack_size, MPI_INT, root, MPI_COMM_WORLD));
}

TEST(Parallel_Operations_MPI, Scatter_Comparsion) {
  int proc_rank, proc_size, root = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
  int src_size = 999, pack_size = src_size / proc_size;

  std::vector<int> src(src_size);
  std::vector<int> local(pack_size);
  std::vector<int> gathered(src_size);

  if (proc_rank == root) {
    for (int i = 0; i < src_size; i++) {
      src[i] = i;
    }
  }

  double start_time, end_time, my_scatter_time, scatter_time;

  start_time = MPI_Wtime();
  MyScatter(&src[0], pack_size, MPI_INT, &local[0], pack_size, MPI_INT, root, MPI_COMM_WORLD);
  end_time = MPI_Wtime();
  if (proc_rank == 0) {
    my_scatter_time = end_time - start_time;
  }

  start_time = MPI_Wtime();
  MPI_Scatter(&src[0], 1, MPI_INT, &local[0], 1, MPI_INT, root, MPI_COMM_WORLD);
  end_time = MPI_Wtime();
  if (proc_rank == 0) {
    scatter_time = end_time - start_time;
  }

  if (proc_rank == 0) {
    std::cout << "My scatter time: " << my_scatter_time << std::endl
      << "MPI scatter time: " << scatter_time << std::endl;
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);
  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());
  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
