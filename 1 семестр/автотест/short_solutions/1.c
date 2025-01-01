#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Len(FILE *f);
void Scan(FILE *f, double *arr, int len);
void Sort(double *arr, int len);
int Func(double *A, double *B, int a, int b);

int Len(FILE *f) {
    int len = 0;
    double c;
    while (fscanf(f, "%lf", &c) == 1) 
        len++;
    return len;
}

void Scan(FILE *f, double *arr, int len) {
    int i;
    for (i = 0; i < len; i++) 
        fscanf(f, "%lf", &arr[i]);
}

void Sort(double *arr, int len) {
    int i, j, t;
    for (i = 0; i < len - 1; i++)
        for (j = 0; j < len - 1; j++)
            if (arr[j] > arr[j + 1]) {
                t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
}

int Func(double *A, double *B, int a, int b) {
    int max_len = (a > b) ? a : b;
    double mA = A[a - 1];
    double mB = B[b - 1];
    int i;
    for (i = 0; i < max_len; i++) {
        double val_A = (i < a) ? A[i] : mA;
        double val_B;
        if (i < b)
            val_B = B[i];
        else 
            val_B = mB;
        if (val_A >= val_B)
            return 0;
    }
    return 1;
}

int main(void) {
    FILE *ina, *inb, *out;
    int a, b, check;
    double *A, *B;
    ina = fopen("ina.txt", "r");
    inb = fopen("inb.txt", "r");
    out = fopen("output.txt", "w");
    if (ina == NULL) 
        return -1;
    if (inb == NULL) {
        fclose(ina);
        return -1;
    }
    if (out == NULL) {
        fclose(ina);
        fclose(inb);
        return -1;
    }
    a = Len(ina);
    b = Len(inb);
    if (a == 0 || b == 0) {
        fclose(ina);
        fclose(inb);
        fclose(out);
        return -1;
    }
    rewind(ina);
    rewind(inb);
    A = (double*)malloc(a * sizeof(double));
    B = (double*)malloc(b * sizeof(double));
    Scan(ina, A, a);
    Scan(inb, B, b);
    Sort(A, a);
    Sort(B, b);
    check = Func(A, B, a, b);
    if (check)
        fprintf(out, "YES");
    else
        fprintf(out, "NO");
    fclose(ina);
    fclose(inb);
    fclose(out);
    free(A);
    free(B);
    return 0;
}

