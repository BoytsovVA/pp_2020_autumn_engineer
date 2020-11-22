// Copyright 2020 Kochankov Ilya

#ifndef MODULES_TASK_2_KOCHANKOV_I_SIMPLE_ITERATION_MATRIX_H_
#define MODULES_TASK_2_KOCHANKOV_I_SIMPLE_ITERATION_MATRIX_H_

class Matrix {
 public:
    Matrix();
    Matrix(int rows, int columns);
    Matrix(const Matrix &copy);

    int get_rows() const { return _rows; }
    int get_columns() const { return _columns; }

    Matrix &operator=(const Matrix &matrix);
    double *operator[](int index);
    const double *operator[](int index) const;

    ~Matrix();

 private:
    int _rows;
    int _columns;
    double *_array;

    void _init();
};

#endif  // MODULES_TASK_2_KOCHANKOV_I_SIMPLE_ITERATION_MATRIX_H_
