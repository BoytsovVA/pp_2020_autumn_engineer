  // Copyright 2020 Zhafyarov Oleg
#include <mpi.h>
#include <random>
#include <algorithm>
#include <ctime>
#include "../../../modules/task_2/zhafyarov_o_smoothing_image/smoothing_image.h"

void DeleteDynamicArray(int** image, int height) {
  for (int i = 0; i < height; i++) {
    delete[] image[i];
  }
  delete[] image;
}

void MedianFilter(int* buffer, int height, int width) {
  int* pix = new int[9];
  int* temp_buffer = new int[height * width];

  for (int i = 1; i < height - 1; i++) {
    for (int j = 1; j < width - 1; j++) {
      pix[0] = buffer[j - 1 + (i - 1) * width];
      pix[1] = buffer[j - 1 + (i)*width];
      pix[2] = buffer[j - 1 + (i + 1) * width];
      pix[3] = buffer[j + (i - 1) * width];
      pix[4] = buffer[j + i * width];
      pix[5] = buffer[j + (i + 1) * width];
      pix[6] = buffer[j + 1 + (i - 1) * width];
      pix[7] = buffer[j + 1 + i * width];
      pix[8] = buffer[j + 1 + (i + 1) * width];

      std::sort(pix, pix + 9, [](int v1, int v2) -> bool {
          return v1 > v2;
        });
      temp_buffer[j + i * width] = pix[4];
    }
  }
  for (int i = 1; i < height - 1; i++) {
    for (int j = 1; j < width - 1; j++) {
      buffer[j + i * width] = temp_buffer[j + i * width];
    }
  }
  delete[] temp_buffer;
  delete[] pix;
}

void MedianFilter(int** image, int height, int width) {
  if (height < 3 && width < 3) {
    return;
  }
  int** temp_image = new int* [height];
  for (int i = 0; i < height; i++) {
    temp_image[i] = new int[width];
  }
  int* pix = new int[9];

  for (int i = 1; i < height - 1; ++i) {
    for (int j = 1; j < width - 1; ++j) {
      pix[0] = image[i - 1][j - 1];
      pix[1] = image[i][j - 1];
      pix[2] = image[i + 1][j - 1];
      pix[3] = image[i - 1][j];
      pix[4] = image[i][j];
      pix[5] = image[i + 1][j];
      pix[6] = image[i - 1][j + 1];
      pix[7] = image[i][j + 1];
      pix[8] = image[i + 1][j + 1];

      std::sort(pix, pix + 9, [](int v1, int v2) -> bool {
          return v1 > v2;
        });
      temp_image[i][j] = pix[4];
    }
  }
  for (int i = 1; i < height - 1; ++i) {
    for (int j = 1; j < width - 1; ++j) {
      image[i][j] = temp_image[i][j];
    }
  }
  DeleteDynamicArray(temp_image, height);
  delete[] pix;
}

void SmoothingImageParallel(int** image, int height, int width) {
  int process_number, process_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &process_number);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  MPI_Status status;

  if (process_rank == 0) {
    if (height < process_number || process_number == 1) {
      for (int i = 1; i < process_number; i++) {
        MPI_Send(image, 2, MPI_INT, i, 1, MPI_COMM_WORLD);
      }
      MedianFilter(image, height, width);
    } else {
      int Size_For_Process = height / process_number;
      int Div_For_Process = height % process_number;
      int Size_buffer = (Size_For_Process + 2) * width;

      for (int i = 1; i < process_number; i++) {
        if (i == process_number - 1) {
          Size_buffer = (Size_For_Process + 1) * width;
        }
        int* buffer = new int[Size_buffer];
        for (int j = 0; j < Size_buffer; j++) {
          buffer[j] = image[(j / width) + Div_For_Process + (Size_For_Process * i) - 1][j % width];
        }
        height = Size_buffer / width;
        int Height_Width[2] = {height, width};

        MPI_Send(Height_Width, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(buffer, Size_buffer, MPI_INT, i, 0, MPI_COMM_WORLD);

        delete[] buffer;
      }

      int Size_Zero = (Size_For_Process + Div_For_Process + 1) * width;
      int* buffer = new int[Size_Zero];

      height = Size_For_Process + Div_For_Process + 1;

      for (int i = 0; i < Size_Zero; i++) {
        buffer[i] = image[i / width][i % width];
      }

      MedianFilter(buffer, height, width);

      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          image[i][j] = buffer[i * width + j];
        }
      }

      for (int k = 1; k < process_number; k++) {
        Size_buffer = (Size_For_Process + 2) * width;
        height = Size_For_Process;

        if (k == process_number - 1) {
          Size_buffer = (Size_For_Process + 1) * width;
          height = Size_For_Process;
        }

        int* buffer_recv = new int[Size_buffer];

        MPI_Recv(buffer_recv, Size_buffer, MPI_INT, k, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < height; i++) {
          for (int j = 0; j < width; j++) {
            image[i + Div_For_Process + k * Size_For_Process][j] = buffer_recv[i * width + j + width];
          }
        }
        delete[] buffer_recv;
      }
      delete[] buffer;
    }
  } else {
      int* Height_Width = new int[2];
      MPI_Recv(Height_Width, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if (status.MPI_TAG == 1) {
        return;
      }
      int height = Height_Width[0];
      int width = Height_Width[1];

      int Size_buffer = height * width;

      int* buffer = new int[Size_buffer];
      MPI_Recv(buffer, Size_buffer, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

      MedianFilter(buffer, height, width);

      MPI_Send(buffer, Size_buffer, MPI_INT, 0, 0, MPI_COMM_WORLD);

      delete[] Height_Width;
      delete[] buffer;
  }
}

int Comparator(int number) {
  if (number < 0) {
    return 0;
  }
  if (number > 255) {
    return 255;
  }
  return number;
}
