// Copyright 2020 Kochankov Ilya

#include "../../../modules/task_2/kochankov_i_simple_iteration/Matrix.h"
#include "stdexcept"

Matrix::Matrix() : _rows(1), _columns(1) {
    this->_init();
}

void Matrix::_init() {
    this->_array = new double[this->_rows * this->_columns];
}

Matrix::~Matrix() {
    delete [] this->_array;
}

Matrix::Matrix(int rows, int columns) : _rows(rows), _columns(columns) {
    if (rows < 1) {
        throw std::runtime_error("number of rows can't be less than zero!");
    }
    if (columns < 1) {
        throw std::runtime_error("number of columns can't be less than zero!");
    }
    this->_init();
}

double *Matrix::operator[](int index) {
    return this->_array + index * this->_columns;
}

const double *Matrix::operator[](int index) const {
    return this->_array + index * this->_columns;;
}

Matrix::Matrix(const Matrix &copy) : _rows(copy._rows), _columns(copy._columns) {
    this->_init();
    for ( int i = 0; i < _rows * _columns; i++ ) {
        this->_array[i] = copy._array[i];
    }
}

Matrix &Matrix::operator=(const Matrix& matrix) {
    this->_rows = matrix._rows;
    this->_columns = matrix._columns;
    this->_init();
    for ( int i = 0; i < _rows * _columns; i++ ) {
        this->_array[i] = matrix._array[i];
    }
    return *this;
}
