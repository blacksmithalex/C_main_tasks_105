#include <stdio.h>
#include <stdlib.h>

// Функции
int Len(FILE *f);
void Scan(FILE *f, int *arr, int len);
void Sort(int *arr, int len);
int Func(int *A, int *B, int a, int b);
void Output(FILE *out, int *arr, int len, int result);

// Вычисление длины массива в файле
int Len(FILE *f) {
    int len = 0, temp;
    while (fscanf(f, "%d", &temp) == 1) {
        len++;
    }
    return len;
}

// Считывание массива из файла
void Scan(FILE *f, int *arr, int len) {
    for (int i = 0; i < len; i++) {
        fscanf(f, "%d", &arr[i]);
    }
}

// Сортировка массива пузырьком
void Sort(int *arr, int len) {
    int i, j, temp;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Проверка подмножества (все элементы B должны быть в A)
int Func(int *A, int *B, int a, int b) {
    int i = 0, j = 0;
    while (i < a && j < b) {
        if (A[i] == B[j]) {
            j++; // Совпадение найдено, проверяем следующий элемент B
        } else if (A[i] < B[j]) {
            i++; // Идем дальше по массиву A
        } else {
            return 0; // Элемент B[j] отсутствует в A
        }
    }
    return j == b; // Если все элементы B найдены, возвращаем 1
}

// Вывод результата в файл
void Output(FILE *out, int *arr, int len, int result) {
    fprintf(out, result ? "YES\n" : "NO\n");
    for (int i = 0; i < len; i++) {
        fprintf(out, "%d ", arr[i]);
    }
    fprintf(out, "\n");
}

// Главная функция
int main(int argc, char *argv[]) {
    FILE *ina, *out;
    int a, b = 0, result;
    int *A, *B;

    // Проверка аргументов командной строки
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return -1;
    }

    // Открытие входного и выходного файлов
    ina = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (ina == NULL || out == NULL) {
        if (ina) fclose(ina);
        if (out) fclose(out);
        return -1;
    }

    // Чтение длины массива из файла
    a = Len(ina);
    if (a == 0) {
        fclose(ina);
        fclose(out);
        return -1;
    }

    // Считывание первого массива из файла
    rewind(ina);
    A = (int *)malloc(a * sizeof(int));
    if (!A) {
        perror("Error allocating memory");
        fclose(ina);
        fclose(out);
        return -1;
    }
    Scan(ina, A, a);
    fclose(ina);

    // Ввод второго массива с клавиатуры
    printf("Enter the elements of the second array (terminate with a non-integer input):\n");
    B = NULL;
    int capacity = 10;
    B = (int *)malloc(capacity * sizeof(int));
    if (!B) {
        perror("Error allocating memory");
        free(A);
        fclose(out);
        return -1;
    }
    while (scanf("%d", &B[b]) == 1) {
        b++;
        if (b >= capacity) {
            capacity *= 2;
            int *new_B = realloc(B, capacity * sizeof(int));
            if (!new_B) {
                perror("Error reallocating memory");
                free(A);
                free(B);
                fclose(out);
                return -1;
            }
            B = new_B;
        }
    }

    // Сортировка массивов
    Sort(A, a);
    Sort(B, b);

    // Проверка подмножества
    result = Func(A, B, a, b);

    // Запись результата в файл
    Output(out, B, b, result);

    // Очистка памяти и закрытие файлов
    free(A);
    free(B);
    fclose(out);

    return 0;
}
