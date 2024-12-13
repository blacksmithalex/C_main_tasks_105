#include <stdio.h>
#include <stdlib.h>
#include <float.h> 
#include <math.h>

#define ERR_FILE -1
#define SUCCESS 0
#define EPS 1e-6

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

double find_max(double *array, int size) {
    double max_val = -DBL_MAX;
    for (int i = 0; i < size; i++) {
        if (array[i] > max_val) {
            max_val = array[i];
        }
    }
    return max_val;
}

void sort_array(double *array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                double temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int compare_arrays(double *a, double *b, int n) {
    for (int k = 1; k < n - 1; k++) {
        int flag = 1;
        float diff1 = a[0] - b[0];
        for (int i = 1; i <= k; i++) {
            float diff = a[i] - b[i];
            if (diff1 < EPS && diff > EPS || diff1 > EPS && diff < EPS)
                flag = 0;
        }
        float diff2 = a[k + 1] - b[k + 1];
        if (diff1 * diff2 > EPS)
            flag = 0;
        for (int i = k + 2; i < n; i++) {
            float diff = a[i] - b[i];
            if (diff2 < EPS && diff > EPS || diff2 > EPS && diff < EPS)
                flag = 0;
        }
        if (flag == 1)
            return k;
    }
    return -1;
}

int write_result_to_file(const char *filename, int k) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        return ERR_FILE;
    }

    fprintf(file, "%d", k);
    fclose(file);
    return SUCCESS;
}

int main() {
    const char *file_a = "ina.txt";
    const char *file_b = "inb.txt";
    const char *output_file = "output.txt";

    int n = count_numbers_in_file(file_b);

    if (n == ERR_FILE) {
        fprintf(stderr, "Error: Unable to process input files\n");
        return ERR_FILE;
    }

    double *a = malloc(n * sizeof(double));
    double *b = malloc(n * sizeof(double));
    if (!a || !b) {
        perror("Error allocating memory");
        free(a);
        free(b);
        return ERR_FILE;
    }

    if (read_array_from_file(file_a, a, n) == ERR_FILE ||
        read_array_from_file(file_b, b, n) == ERR_FILE) {
        fprintf(stderr, "Error: Unable to read arrays from files\n");
        free(a);
        free(b);
        return ERR_FILE;
    }

    sort_array(a, n);
    sort_array(b, n);

    int k = compare_arrays(a, b, n);

    if (write_result_to_file(output_file, k) == ERR_FILE) {
        fprintf(stderr, "Error: Unable to write result to file\n");
        free(a);
        free(b);
        return ERR_FILE;
    }

    free(a);
    free(b);

    return SUCCESS;
}
