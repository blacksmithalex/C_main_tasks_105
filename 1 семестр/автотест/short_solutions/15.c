#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Len(FILE *f);
void Scan(FILE *f, int *arr, int len);
void Sort(int *arr, int len);
int Func(int *A, int a);

int Len(FILE *f) {
    int len = 0;
    int c;
    while (fscanf(f, "%d", &c) == 1) 
        len++;
    return len;
}

void Scan(FILE *f, int *arr, int len) {
    int i;
    for (i = 0; i < len; i++) 
        fscanf(f, "%d", &arr[i]);
}

void Sort(int *arr, int len) {
    int i, j, t;
    for (i = 0; i < len - 1; i++)
        for (j = 0; j < len - 1; j++)
            if (arr[j] > arr[j + 1]) {
                t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
}

int Func(int *A, int a) {
    int count, i;
    count = 1;
    for (i = 0; i < a - 1; i++) {
        if (A[i] != A[i + 1])
            count++;
    }
    return count;
}

int main(void) {
    FILE *ina, *inb, *out;
    int a, b, check, cardA, cardB, i;
    int *A, *B;
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
    A = (int*)malloc(a * sizeof(int));
    B = (int*)malloc(b * sizeof(int));
    Scan(ina, A, a);
    Scan(inb, B, b);
    Sort(A, a);
    Sort(B, b);

    cardA = Func(A, a);
    cardB = Func(B, b);

    if (cardA >= cardB){
        fprintf(out, "A\n");
        fprintf(out, "%d ", A[0]);
        for (i = 1; i < a; i++) {
            if (A[i] != A[i - 1]) fprintf(out, "%d ", A[i]);
        }
    } else {
        fprintf(out, "B\n");
        fprintf(out, "%d ", B[0]);
        for (i = 1; i < b; i++) {
            if (B[i] != B[i - 1]) fprintf(out, "%d ", B[i]);
        }
    }
    fclose(ina);
    fclose(inb);
    fclose(out);
    free(A);
    free(B);
    return 0;
}

