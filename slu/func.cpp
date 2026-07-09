#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <ctime>
#include "func_header.h"
using namespace std;

// Создание двумерного массива (матрицы) размером n x m
double** create_arr(int n, int m) {
	double** p = new double* [n]; // Выделяем массив указателей (строки)
	for (int i = 0; i < n; i++) {
		p[i] = new double[m]; // Для каждой строки выделяем массив столбцов
	}
	return p;
}

// Вывод матрицы A и вектора B (расширенная матрица системы)
void print_matrix(double** matrix_a, double* matrix_b, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << matrix_a[i][j] << ' ';
		}
		cout << matrix_b[i] << endl; // В конце строки выводится элемент B
	}
}

// Освобождение памяти матрицы
void clear_matrix(double** p, int n) {
	for (int i = 0; i < n; i++) {
		delete[] p[i]; // Удаляем каждую строку
	}
	delete[] p; // Удаляем массив указателей
}

// Чтение матрицы из файла
void read_matrix(double**& matrix_a, double*& matrix_b, double*& matrix_x, int& n) {
	ifstream file("save_matrix.txt", ios_base::in);

	// Проверка открытия файла
	if (!file.is_open()) {
		cout << "Ошибка: не удалось открыть файл!" << endl;
		return;
	}

	// Очистка старых данных (если были)
	if (matrix_a != nullptr) {
		clear_matrix(matrix_a, n);
	}
	if (matrix_b != nullptr) {
		delete[] matrix_b;
	}
	if (matrix_x != nullptr) {
		delete[] matrix_x;
	}

	file >> n; // Считываем размерность

	// Выделяем память под новые данные
	matrix_a = create_arr(n, n);
	matrix_b = new double[n];
	matrix_x = new double[n];

	// Считываем матрицу A
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			file >> matrix_a[i][j];
		}
	}

	// Считываем вектор B
	for (int i = 0; i < n; i++) {
		file >> matrix_b[i];
	}

	// Считываем вектор X (решение)
	for (int i = 0; i < n; i++) {
		file >> matrix_x[i];
	}

	// Если матрица небольшая — выводим
	if (n <= 7) {
		print_matrix(matrix_a, matrix_b, n);
	}

	file.close(); // Закрываем файл
}

// Копирование матрицы
void matrix_copy(double** matrix, double**& matrix_copy, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix_copy[i][j] = matrix[i][j];
		}
	}
}

// Копирование одномерного массива
void matrix_copy_1(double* matrix, double*& matrix_copy, int n) {
	for (int i = 0; i < n; i++) {
		matrix_copy[i] = matrix[i];
	}
}

// Запись данных в файл	
void write_matrix(double** matrix_a, double* matrix_b, double* matrix_x, int n) {
	ofstream file("save_matrix.txt", ios_base::out | ios_base::trunc);
	file << n << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			file << matrix_a[i][j] << " ";
		}
		file << endl;
	}
	for (int i = 0; i < n; i++) {
		file << matrix_b[i] << " ";
	}
	file.close();
}

void set_problem(double** matrix_a, double* matrix_b, double* matrix_x, int n) {
	if (n > 4) {
		for (int i = 0; i < n; i++) {
			matrix_x[i] = 1;
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				matrix_a[i][j] = 1 + rand() % 100;
			}
		}
		for (int i = 0; i < n; i++) {
			matrix_b[i] = 0;
			for (int j = 0; j < n; j++) {
				matrix_b[i] += matrix_a[i][j] * matrix_x[j];
			}
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				system("cls");
				cout << "Ручной ввод" << endl;
				for (int i1 = 0; i1 < n; i1++) {
					// flag = false; // Убрали
					for (int j1 = 0; j1 < n; j1++) {
						if (0 <= matrix_a[i1][j1] && matrix_a[i1][j1] <= 100) {
							cout << matrix_a[i1][j1] << " "; // Фиксированная ширина 4 символа
						}
						else {
							cout << "*" << " "; // Печатаем звездочку, чтобы столбец не съехал
						}
					}
					cout << endl; // Перенос строки теперь всегда после каждой строки матрицы
				}
				cout << "A[" << i + 1 << "][" << j + 1 << "] = "; // Подсказка, какую ячейку вводим
				cin >> matrix_a[i][j];
			}
		}
		for (int i = 0; i < n; i++) {
			system("cls");
			cout << "Ручной ввод B" << endl;

			// Горизонтальный вывод
			cout << "B = [ ";
			for (int i1 = 0; i1 < n; i1++) {
				if (0 <= matrix_b[i1] && matrix_b[i1] <= 100) {
					cout << matrix_b[i1] << " ";
				}
				else {
					cout << "* ";
				}
			}
			cout << "]" << endl;

			cout << "B[" << i + 1 << "] = ";
			cin >> matrix_b[i];
		}
	}
}

double residual(double** matrix_a, double* matrix_b, double* matrix_x, int size) {
	double* arr = new double[size];
	for (int i = 0; i < size; i++) {
		arr[i] = -matrix_b[i];
		for (int j = 0; j < size; j++) {
			arr[i] += matrix_a[i][j] * matrix_x[j];
		}
	}
	double mx = abs(arr[0]);
	for (int i = 1; i < size; i++) {
		if (abs(arr[i]) > mx) {
			mx = abs(arr[i]);
		}
	}
	delete[] arr;
	return mx;
}

double error_rate(double* X, double* X_copy, int n) {
	if (n <= 4) {
		cout << "Ручная проверка" << endl;
		return 0;
	}
	double mx = abs(X[0] - X_copy[0]);
	for (int i = 1; i < n; i++) {
		if (mx < abs(X[i] - X_copy[i])) {
			mx = abs(X[i] - X_copy[i]);
		}
	}
	return mx;
}

void gauss(double** A, double* B, double* X, int n, bool f, double& timer) {
	clock_t start = clock();
	double a;
	for (int j = 0; j < n - 1; j++) {
		if (f) {
			double mx = abs(A[j][j]);
			int m_id = j;
			for (int i = j + 1; i < n; i++) {
				if (abs(A[i][j]) > mx) {
					mx = abs(A[i][j]);
					m_id = i;
				}
			}
			double* tmp = A[m_id];
			A[m_id] = A[j];
			A[j] = tmp;
			double tmp1 = B[m_id];
			B[m_id] = B[j];
			B[j] = tmp1;
		}
		if (abs(A[j][j]) < 1e-9) {
			cout << "Матрица вырожденная! Деление на ноль невозможно." << endl;
			return;
		}
		for (int i = j + 1; i < n; i++) {
			a = A[i][j] / A[j][j];
			for (int k = j; k < n; k++) {
				A[i][k] -= a * A[j][k];
			}
			B[i] -= a * B[j];
		}
		if (n <= 7) {
			cout << endl << endl;
			print_matrix(A, B, n);
		}
	}
	double s;
	for (int i = n - 1; i >= 0; i--) {
		s = 0;
		for (int j = i + 1; j < n; j++) {
			s += A[i][j] * X[j];
		}
		X[i] = (B[i] - s) / A[i][i];
	}
	clock_t end = clock();
	timer = (double)(end - start) / CLOCKS_PER_SEC;
}