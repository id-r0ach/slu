#pragma once

double** create_arr(int n, int m);

void print_matrix(double** matrix_a, double* matrix_b, int n);

void clear_matrix(double** p, int n);

void read_matrix(double**& matrix_a, double*& matrix_b, double*& matrix_x, int& n);

void matrix_copy(double** matrix, double**& matrix_copy, int n);

void matrix_copy_1(double* matrix, double*& matrix_copy, int n);

void write_matrix(double** matrix_a, double* matrix_b, double* matrix_x, int n);

void set_problem(double** matrix_a, double* matrix_b, double* matrix_x, int n);

double residual(double** matrix_a, double* matrix_b, double* matrix_x, int size);

double error_rate(double* X, double* X_copy, int n);

void gauss(double** A, double* B, double* X, int n, bool f, double& timer);