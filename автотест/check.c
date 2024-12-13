#include <stdio.h>
#include <stdlib.h>

#define ERR_FILE -1
#define SUCCESS 0

int count_numbers_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ERR_FILE;
    }

    int count = 0;
    double temp;
    while (fscanf(file, "%lf", &temp) == 1) {
        count++;
    }

    fclose(file);
    return (count > 0) ? count : ERR_FILE;
}

int read_array_from_file(const char *filename, double *array, int size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ERR_FILE;
    }

    for (int i = 0; i < size; i++) {
        if (fscanf(file, "%lf", &array[i]) != 1) {
            fclose(file);
            return ERR_FILE;
        }
    }

    fclose(file);
    return SUCCESS;
}

void process_arrays(double *a, int m, double *b, int n, double *result, int *result_size) {
    *result_size = 0;

    // Пример обработки: объединение массивов
    for (int i = 0; i < m; i++) {
        result[(*result_size)++] = a[i];
    }
    for (int i = 0; i < n; i++) {
        result[(*result_size)++] = b[i];
    }

    // Пример: упорядочивание результата
    for (int i = 0; i < *result_size - 1; i++) {
        for (int j = 0; j < *result_size - i - 1; j++) {
            if (result[j] > result[j + 1]) {
                double temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }
}

int write_result_to_file(const char *filename, double *array, int size) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        return ERR_FILE;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%.6f\n", array[i]);
    }

    fclose(file);
    return SUCCESS;
}

int main() {
    const char *file_a = "ina.txt";
    const char *file_b = "inb.txt";
    const char *output_file = "output.txt";

    int m = count_numbers_in_file(file_a);
    int n = count_numbers_in_file(file_b);

    if (m == ERR_FILE || n == ERR_FILE) {
        fprintf(stderr, "Error: Unable to process input files\n");
        return ERR_FILE;
    }

    double *a = malloc(m * sizeof(double));
    double *b = malloc(n * sizeof(double));
    double *result = malloc((m + n) * sizeof(double));
    if (!a || !b || !result) {
        perror("Error allocating memory");
        free(a);
        free(b);
        free(result);
        return ERR_FILE;
    }

    if (read_array_from_file(file_a, a, m) == ERR_FILE ||
        read_array_from_file(file_b, b, n) == ERR_FILE) {
        fprintf(stderr, "Error: Unable to read arrays from files\n");
        free(a);
        free(b);
        free(result);
        return ERR_FILE;
    }

    int result_size = 0;
    process_arrays(a, m, b, n, result, &result_size);

    if (write_result_to_file(output_file, result, result_size) == ERR_FILE) {
        fprintf(stderr, "Error: Unable to write result to file\n");
        free(a);
        free(b);
        free(result);
        return ERR_FILE;
    }

    free(a);
    free(b);
    free(result);

    return SUCCESS;
}
