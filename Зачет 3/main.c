#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Функция сравнения для сортировки
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Бинарный поиск элемента в массиве
bool binary_search(const int *arr, int size, int target) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return true;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    // Открываем входной файл
    FILE *input_file = fopen(input_filename, "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    // Считываем первый массив из файла
    int *x = NULL;
    int x_size = 0;
    int x_capacity = 10;
    x = malloc(x_capacity * sizeof(int));
    if (!x) {
        perror("Error allocating memory");
        fclose(input_file);
        return 1;
    }

    while (fscanf(input_file, "%d", &x[x_size]) == 1) {
        x_size++;
        if (x_size >= x_capacity) {
            x_capacity *= 2;
            int *new_x = realloc(x, x_capacity * sizeof(int));
            if (!new_x) {
                perror("Error reallocating memory");
                free(x);
                fclose(input_file);
                return 1;
            }
            x = new_x;
        }
    }

    fclose(input_file);

    // Считываем второй массив с клавиатуры
    int *y = NULL;
    int y_size = 0;
    int y_capacity = 10;
    y = malloc(y_capacity * sizeof(int));
    if (!y) {
        perror("Error allocating memory");
        free(x);
        return 1;
    }

    int temp;
    while (scanf("%d", &temp) == 1) {
        y[y_size++] = temp;
        if (y_size >= y_capacity) {
            y_capacity *= 2;
            int *new_y = realloc(y, y_capacity * sizeof(int));
            if (!new_y) {
                perror("Error reallocating memory");
                free(x);
                free(y);
                return 1;
            }
            y = new_y;
        }
    }

    // Сортируем оба массива
    qsort(x, x_size, sizeof(int), compare);
    qsort(y, y_size, sizeof(int), compare);

    // Проверяем, является ли второй массив подмножеством первого
    bool is_subset = true;
    for (int i = 0; i < y_size; i++) {
        if (!binary_search(x, x_size, y[i])) {
            is_subset = false;
            break;
        }
    }

    // Открываем выходной файл
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Error opening output file");
        free(x);
        free(y);
        return 1;
    }

    // Записываем результат
    fprintf(output_file, "%s\n", is_subset ? "YES" : "NO");
    for (int i = 0; i < y_size; i++) {
        fprintf(output_file, "%d ", y[i]);
    }
    fprintf(output_file, "\n");

    fclose(output_file);
    free(x);
    free(y);
    return 0;
}
