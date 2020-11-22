// Copyright 2020 Kochankov Ilya

#ifndef MODULES_TASK_1_KOCHANKOV_I_MATRIX_COLUMNS_SUM_MATRIX_H_
#define MODULES_TASK_1_KOCHANKOV_I_MATRIX_COLUMNS_SUM_MATRIX_H_


class Matrix {
 public:
    Matrix();
    Matrix(int rows, int columns);
    Matrix(const Matrix& copy);

    int get_rows() const {return _rows;}
    int get_columns() const {return _columns;}

    Matrix& operator=(const Matrix& matrix);
    int* operator[] (int index);
    const int* operator[] (int index) const;

    ~Matrix();

 private:
    int _rows;
    int _columns;
    int* _array;

    void _init();
};


#endif  // MODULES_TASK_1_KOCHANKOV_I_MATRIX_COLUMNS_SUM_MATRIX_H_
