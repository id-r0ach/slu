#include <iostream>
#include <clocale>
#include "func_header.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "rus"); // Установка русской локали для корректного вывода текста
	srand(time(0)); // Инициализация генератора случайных чисел

	int n_size; // Размерность системы (количество уравнений)

	// Основные матрицы и векторы
	double** matrix_a = nullptr;       // Матрица коэффициентов A
	double** matrix_a_copy = nullptr;  // Копия матрицы A (для вычисления погрешности)
	double* matrix_b = nullptr;        // Вектор правых частей b
	double* matrix_b_copy = nullptr;   // Копия вектора b
	double* matrix_x = nullptr;        // Вектор решений x
	double* matrix_x_copy = nullptr;   // Копия начального x (для оценки ошибки)

	int ch; // Переменная для выбора пункта меню
	bool exit = false; // Флаг выхода из программы
	double timer; // Переменная для хранения времени выполнения

	// Основной бесконечный цикл меню
	while (1) {
		system("cls"); // Очистка консоли

		// Вывод меню
		cout << "1. Ввод данных" << endl;
		cout << "2. Вывод данных" << endl;
		cout << "3. Решение" << endl;
		cout << "4. Результат" << endl;
		cout << "5. Погрешность" << endl;
		cout << "6. Данные из файла" << endl;
		cout << "7. Сохранить данные" << endl;
		cout << "0. Выход" << endl;
		cout << "Выбор: ";

		cin >> ch; // Ввод выбора пользователя

		switch (ch)
		{
		case 1:
			system("cls");

			// Очистка ранее выделенной памяти (если была)
			if (matrix_a != nullptr) clear_matrix(matrix_a, n_size);
			if (matrix_b != nullptr) delete[] matrix_b;
			if (matrix_x != nullptr) delete[] matrix_x;
			if (matrix_a_copy != nullptr) clear_matrix(matrix_a_copy, n_size);
			if (matrix_b_copy != nullptr) delete[] matrix_b_copy;
			if (matrix_x_copy != nullptr) delete[] matrix_x_copy;
			cout << "Введи размерность системы линейных уравнений: ";
			cin >> n_size;

			// Выделение памяти под матрицы и векторы
			matrix_a = create_arr(n_size, n_size);
			matrix_a_copy = create_arr(n_size, n_size);
			matrix_b = new double[n_size];
			matrix_b_copy = new double[n_size];
			matrix_x = new double[n_size];
			matrix_x_copy = new double[n_size];

			// Заполнение системы (ввод или генерация)
			set_problem(matrix_a, matrix_b, matrix_x, n_size);

			// Создание копий исходных данных
			matrix_copy(matrix_a, matrix_a_copy, n_size);
			matrix_copy_1(matrix_b, matrix_b_copy, n_size);
			matrix_copy_1(matrix_x, matrix_x_copy, n_size);

			// Если размер небольшой — выводим матрицу
			if (n_size <= 7) {
				print_matrix(matrix_a, matrix_b, n_size);
			}

			system("pause");
			break;

		case 2:
			system("cls");

			// Проверка, что данные заданы
			if (matrix_a == nullptr || matrix_b == nullptr || matrix_x == nullptr) {
				cout << "Сначала нужно задать систему уравнений" << endl;
				system("pause");
				break;
			}

			// Вывод исходных (скопированных) данных
			if (n_size <= 7) {
				print_matrix(matrix_a_copy, matrix_b_copy, n_size);
			}

			system("pause");
			break;

		case 3:
			system("cls");

			// Проверка наличия данных
			if (matrix_a == nullptr || matrix_b == nullptr || matrix_x == nullptr) {
				cout << "Сначала нужно задать систему уравнений" << endl;
				system("pause");
				break;
			}


			bool ch1;
			cout << "Улучшенная версия - 1" << endl << "Обычная версия - 0" << endl;
			cin >> ch1;

			// Решение системы методом Гаусса
			gauss(matrix_a, matrix_b, matrix_x, n_size, ch1, timer);

			// Вывод времени выполнения
			cout << "Время: " << timer << endl;

			system("pause");
			break;

		case 4:
			system("cls");

			// Проверка наличия данных
			if (matrix_a == nullptr || matrix_b == nullptr || matrix_x == nullptr) {
				cout << "Сначала нужно задать систему уравнений" << endl;
				system("pause");
				break;
			}

			// Вывод найденного решения
			cout << "Решение: " << endl;
			for (int i = 0; i < n_size; i++) {
				cout << "x" << i+1 << " = " << matrix_x[i] << endl;
			}

			system("pause");
			break;

		case 5:
			system("cls");

			// Проверка наличия данных
			if (matrix_a == nullptr || matrix_b == nullptr || matrix_x == nullptr) {
				cout << "Сначала нужно задать систему уравнений" << endl;
				system("pause");
				break;
			}

			// Вычисление невязки (насколько решение удовлетворяет системе)
			cout << "Погрешность: " << residual(matrix_a_copy, matrix_b_copy, matrix_x, n_size) << endl;

			// Вычисление ошибки относительно исходного решения
			cout << "Погрешность: " << error_rate(matrix_x, matrix_x_copy, n_size) << endl;

			system("pause");
			break;

		case 6:
			system("cls");

			// Очистка старых копий
			if (matrix_a_copy != nullptr) clear_matrix(matrix_a_copy, n_size);
			if (matrix_b_copy != nullptr) delete[] matrix_b_copy;
			if (matrix_x_copy != nullptr) delete[] matrix_x_copy;

			// Чтение данных из файла
			read_matrix(matrix_a, matrix_b, matrix_x, n_size);

			// Проверка успешности загрузки
			if (matrix_a == nullptr) {
				cout << "Загрузка не удалась!" << endl;
				system("pause");
				break;
			}

			// Создание новых копий после загрузки
			matrix_a_copy = create_arr(n_size, n_size);
			matrix_b_copy = new double[n_size];
			matrix_x_copy = new double[n_size];

			matrix_copy(matrix_a, matrix_a_copy, n_size);
			matrix_copy_1(matrix_b, matrix_b_copy, n_size);
			matrix_copy_1(matrix_x, matrix_x_copy, n_size);
			cout << "Данные успешно загружены и скопированы!" << endl;

			system("pause");
			break;

		case 7:
			system("cls");

			// Сохранение данных в файл
			write_matrix(matrix_a_copy, matrix_b_copy, matrix_x_copy, n_size);

			system("pause");
			break;

		case 0:
			exit = true; // Установка флага выхода
			break;
		}

		// Проверка выхода из цикла
		if (exit) {
			cout << "Выход из программы" << endl;
			break;
		}
	}

	// Освобождение всей выделенной памяти перед завершением программы
	if (matrix_a != nullptr) clear_matrix(matrix_a, n_size);
	if (matrix_b != nullptr) delete[] matrix_b;
	if (matrix_x != nullptr) delete[] matrix_x;
	if (matrix_a_copy != nullptr) clear_matrix(matrix_a_copy, n_size);
	if (matrix_b_copy != nullptr) delete[] matrix_b_copy;
	if (matrix_x_copy != nullptr) delete[] matrix_x_copy;
	return 0;
}