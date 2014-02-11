/*
 * matrix.h
 *
 *  Created on: Oct 1, 2013
 *      Author: quannt
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#define MATRIX_ZERO 0
#define MATRIX_IDENTITY 1

enum MatrixException {INVALID_MATRIX_SIZE, INVALID_MATRIX_TYPE, INVALID_SUBMATRIX_INDEX};

class Matrix
{
    private:
        int rows, cols; // Number of rows, columns
        double **cell; // 2D array for cells

        /* For overloading constructor */
        void init(int rows, int cols, int initMat);

    public:
        Matrix(int rows, int cols);
        /* Create new matrix with size rows*cols and having initial setup initMat */
        Matrix(int rows, int cols, int initMat);
        /* Copy constructor */
        Matrix(const Matrix &mat);
        virtual ~Matrix();

        /* Assign operator */
        Matrix &operator=(Matrix mat);
        /* Multiply with a number */
        Matrix operator*(const double a);
        /* Add two matrices */
        Matrix operator+(const Matrix &mat);
        /* Minus matrix */
        Matrix operator-(const Matrix &mat);
        /* Multiply two matrices */
        Matrix operator*(const Matrix &mat);

        /* Get number of rows */
        int getRows();
        /* Get number of columns */
        int getCols();
        /* Get data in a cell */
        double getCell(int row, int col);
        /* Set data in a cell */
        void setCell(int row, int col, double value);
        /* Get a transposed matrix */
        Matrix transpose();
        /* Get sub-matrix corresponding to a cell */
        Matrix subMatrix(int r, int c);
        /* Get determinant */
        double getDet();
        /* Get inverted matrix */
        Matrix inverse();
        /* Print matrix to standard output */
        void print();

        friend void swap(Matrix &m1, Matrix &m2);
};

void swap(Matrix &m1, Matrix &m2);

#endif /* MATRIX_H_ */
