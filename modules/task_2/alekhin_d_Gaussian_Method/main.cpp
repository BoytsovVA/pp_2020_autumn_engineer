// Copyright 2020 Alekhin Denis
#include <stdlib.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../modules/task_2/alekhin_d_Gaussian_Method/gaussian_method.h"

TEST(Choose_Leading_Line, Choose_Leading_Works) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    EXPECT_NO_THROW(chooseLeadingLine(matrix, 0));
  }
}

TEST(Choose_Leading_Line, Choose_Leading_Line_First_Column_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    EXPECT_EQ(chooseLeadingLine(matrix, 0), 2);
  }
}

TEST(Choose_Leading_Line, Choose_Leading_Line_Second_Column_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    EXPECT_EQ(chooseLeadingLine(matrix, 1), 1);
  }
}

TEST(Choose_Leading_Line, Choose_Leading_Line_Third_Column_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    EXPECT_EQ(chooseLeadingLine(matrix, 2), 4);
  }
}

TEST(Choose_Leading_Line, Choose_Leading_Line_Fourth_Column_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    EXPECT_EQ(chooseLeadingLine(matrix, 3), 4);
  }
}

TEST(Choose_Leading_Line, Choose_Leading_Line_Fifth_Column_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    EXPECT_EQ(chooseLeadingLine(matrix, 4), 4);
  }
}

TEST(Swap_Lines, Swap_Lines_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    initMatrix(&matrix);
    int line2 = chooseLeadingLine(matrix, 0);
    EXPECT_NO_THROW(swapLines(&matrix, 0, line2));
  }
}

TEST(Sequantial_Gauss, Sequantial_Gauss_Works) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    EXPECT_NO_THROW(getSequantialGauss(matrix));
  }
}

TEST(Sequantial_Gauss, Sequantial_Gauss_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    Matrix matrix;
    std::vector<double> result = getSequantialGauss(matrix);
    std::vector<double> temp = { 1.22, -1.145, 0.755, -0.371, 0.47 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_NEAR(result[i], temp[i], 0.01);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_One_Process) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 5;

  if (size == 1) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 5 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_Two_Processes) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 5;

  if (size == 2) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 3, 2 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_Three_Processes) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 5;

  if (size == 3) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 2, 2, 1 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_Four_Processes) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 5;

  if (size == 4) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 2, 1, 1, 1 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_Five_Processes) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 5;

  if (size == 5) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 1, 1, 1, 1, 1 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_Six_Processes) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 5;

  if (size == 6) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 1, 1, 1, 1, 1 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Number_Of_Lines, Number_Of_Lines_For_One_Line) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Matrix matrix;
  matrix.columns = 6;
  matrix.rows = 1;

  if (size == 2) {
    std::vector<int> result = numberOfLinesToStore(matrix);
    std::vector<int> temp = { 1 };

    ASSERT_EQ(result.size(), temp.size());
    for (std::size_t i = 0; i < result.size(); i++) {
      EXPECT_EQ(result[i], temp[i]);
    }
  }
}

TEST(Distribute_Test, DistributeData_Works) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int number_of_line_to_store_size = number_of_line_to_store.size();
  for (int i = 0; i < number_of_line_to_store_size; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }
  int count_of_work_processors = number_of_line_to_store.size();
  if (rank < count_of_work_processors) {
    EXPECT_NO_THROW(distributeData(matrix, &local_matrix));
  }
}

TEST(Leading_Line_Test, Leading_Line_Test_One) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int number_of_line_to_store_size = number_of_line_to_store.size();
  for (int i = 0; i < number_of_line_to_store_size; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }

  int count_of_work_processors = number_of_line_to_store.size();
  if (rank < count_of_work_processors) {
    distributeData(matrix, &local_matrix);
    EXPECT_EQ(chooseLeadingLineParallel(matrix, local_matrix, 0), 2);
  }
}

TEST(Leading_Line_Test, Leading_Line_Test_Two) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int number_of_line_to_store_size = number_of_line_to_store.size();
  for (int i = 0; i < number_of_line_to_store_size; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }

  int count_of_work_processors = number_of_line_to_store.size();
  if (rank < count_of_work_processors) {
    distributeData(matrix, &local_matrix);
    EXPECT_EQ(chooseLeadingLineParallel(matrix, local_matrix, 1), 1);
  }
}

TEST(Leading_Line_Test, Leading_Line_Test_Three) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int number_of_line_to_store_size = number_of_line_to_store.size();
  for (int i = 0; i < number_of_line_to_store_size; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }

  int count_of_work_processors = number_of_line_to_store.size();
  if (rank < count_of_work_processors) {
    distributeData(matrix, &local_matrix);
    EXPECT_EQ(chooseLeadingLineParallel(matrix, local_matrix, 4), 4);
  }
}

TEST(Leading_Line_Test, Leading_Line_Test_Four) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int number_of_line_to_store_size = number_of_line_to_store.size();
  for (int i = 0; i < number_of_line_to_store_size; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }

  int count_of_work_processors = number_of_line_to_store.size();
  if (rank < count_of_work_processors) {
    distributeData(matrix, &local_matrix);
    EXPECT_EQ(chooseLeadingLineParallel(matrix, local_matrix, 3), 4);
  }
}

TEST(Swap_Lines_Test, Swap_Lines_Works) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);
  Matrix local_matrix;

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);

  int count_of_work_processors = number_of_line_to_store.size();
  for (int i = 0; i < count_of_work_processors; i++) {
    if (i == rank) {
      local_matrix.columns = matrix.columns;
      local_matrix.rows = number_of_line_to_store[i];
    }
  }

  if (rank < count_of_work_processors) {
    distributeData(matrix, &local_matrix);
    EXPECT_NO_THROW(swapLinesParallel(&local_matrix, 0, 1));
  }
}

TEST(Sequantial_Gauss, Sequantial_Gauss_Time_Test) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double startTime = MPI_Wtime();

  if (rank == 0) {
    Matrix matrix;
    std::vector<double> result = getSequantialGauss(matrix);
  }

  double endTime = MPI_Wtime();
  if (rank == 0) {
    std::cout << endTime - startTime << std::endl;
  }
}

TEST(Gauss_Parallel_Test, Gauss_Parallel_Time_Test) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix matrix;
  initMatrix(&matrix);

  std::vector<int> number_of_line_to_store = numberOfLinesToStore(matrix);
  int count_of_work_processors = number_of_line_to_store.size();

  double startTime = MPI_Wtime();

  if (rank < count_of_work_processors) {
    getParallelGauss(matrix);
  }

  double endTime = MPI_Wtime();
  if (rank == 0) {
    std::cout << endTime - startTime << std::endl;
  }
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
