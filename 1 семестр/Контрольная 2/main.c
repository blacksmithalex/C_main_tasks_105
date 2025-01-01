#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <string.h>

#define ERR_FILE_OPEN 2
#define ERR_NO_NUMBERS 3
#define START_CAPACITY 10

int compare_abs(const void *a, const void *b)
{
    int abs_a = abs(*(int *)a);
    int abs_b = abs(*(int *)b);
    return abs_a - abs_b;
}

int is_arithmetic_progression(const int *array, int size)
{
    if (size <= 1) {
        return 1;
    }
    int diff = array[1] - array[0];
    for (int i = 2; i < size; i++) {
        if (array[i] - array[i - 1] != diff) {
            return 0;
        }
    }
    return 1;
}

int *read_array_from_file(const char *filename, int *size)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(ERR_FILE_OPEN);
    }

    int capacity = START_CAPACITY;
    int *array = malloc(capacity * sizeof(int));
    if (!array) {
        perror("Error allocating memory");
        fclose(file);
        exit(ERR_FILE_OPEN);
    }

    *size = 0;
    while (1) {
        int value;
        if (fscanf(file, "%d", &value) == 1) {
            if (*size >= capacity) {
                capacity *= 2;
                int *new_array = realloc(array, capacity * sizeof(int));
                if (!new_array) {
                    perror("Error reallocating memory");
                    free(array);
                    fclose(file);
                    exit(ERR_FILE_OPEN);
                }
                array = new_array;
            }
            array[(*size)++] = value;
        } else if (feof(file)) {
            break;
        } else {
            fprintf(stderr, "Error: Invalid number in file\n");
            free(array);
            fclose(file);
            exit(ERR_NO_NUMBERS);
        }
    }

    fclose(file);

    if (*size == 0) {
        fprintf(stderr, "Error: No numbers in file\n");
        free(array);
        exit(ERR_NO_NUMBERS);
    }

    return array;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    int size_x;
    int size_y;
    int *array_x = read_array_from_file(argv[1], &size_x);
    int *array_y = read_array_from_file(argv[2], &size_y);

    qsort(array_x, size_x, sizeof(int), compare_abs);
    qsort(array_y, size_y, sizeof(int), compare_abs);

    int is_x_arithmetic = is_arithmetic_progression(array_x, size_x);
    int is_y_arithmetic = is_arithmetic_progression(array_y, size_y);

    if (is_x_arithmetic && is_y_arithmetic) {
        printf("YEs\n");
    } else {
        printf("No\n");
    }

    for (int i = 0; i < size_x; i++) {
        printf("%d ", array_x[i]);
    }
    printf("\n");

    free(array_x);
    free(array_y);

    return 0;
}
