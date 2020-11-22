  // Copyright 2020 Zhafyarov Oleg
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./smoothing_image.h"

TEST(Parallel_Operations_MPI, FirstTest_Image1x1) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 1;
  const int width = 1;

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
  }
  image[0][0] = 0;

  SmoothingImageParallel(image, height, width);
  int result[height][width] = {};
  result[0][0] = 0;
  bool result_is_correct = true;

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, SecondTest_Image2x2) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 2;
  const int width = 2;

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(i);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      result[i][j] = Comparator(i);
    }
  }
  bool result_is_correct = true;

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, ThirdTest_Image5x6) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 5;
  const int width = 6;

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(i);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      result[i][j] = Comparator(i);
    }
  }
  bool result_is_correct = true;

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, FourthTest_Image25x30) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 25;
  const int width = 30;

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(i);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      result[i][j] = Comparator(i);
    }
  }
  bool result_is_correct = true;

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, FifthTest_Image75x45) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 75;
  const int width = 45;

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(i);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      result[i][j] = Comparator(i);
    }
  }
  bool result_is_correct = true;

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, SixthTest_Image260x45) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 75;
  const int width = 45;

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(i);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      result[i][j] = Comparator(i);
    }
  }
  bool result_is_correct = true;

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, SeventhTest_Image3x3) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  const int height = 3;
  const int width = 3;
  bool result_is_correct = true;

  int temp[3][3] = { {6,   28, 11},
                     {204, 39, 7 },
                     {0,   52, 89} };

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(temp[i][j]);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width] = { {6,   28, 11},
                                {204, 28, 7 },
                                {0,   52, 89} };

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }
  DeleteDynamicArray(image, height);
  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

TEST(Parallel_Operations_MPI, EighthTest_Image5x6) {
  int process_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  bool result_is_correct = true;
  const int height = 5;
  const int width = 6;

  int temp[5][6] = { {6,   28, 11, 59, 1,   8},
                     {204, 18, 7,  23, 32,  1},
                     {0,   52, 89, 11, 18,  79},
                     {158, 21, 3,  8,  14,  82},
                     {11,  29, 28, 1,  208, 53} };

  int** image = new int* [height];
  for (int i = 0; i < height; i++) {
    image[i] = new int[width];
    for (int j = 0; j < width; j++) {
      image[i][j] = Comparator(temp[i][j]);
    }
  }

  SmoothingImageParallel(image, height, width);

  int result[height][width] = { {6,   28, 11, 59, 1,   8},
                                {204, 18, 23, 18, 18,  1},
                                {0,   21, 18, 14, 18,  79},
                                {158, 28, 21, 14, 18,  82},
                                {11,  29, 28, 1,  208, 53} };

  if (process_rank == 0) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (image[i][j] != result[i][j]) {
          result_is_correct = false;
          break;
        }
      }
    }
  }

  DeleteDynamicArray(image, height);

  if (process_rank == 0) {
    ASSERT_EQ(result_is_correct, true);
  }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}


