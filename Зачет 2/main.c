#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int Len(FILE *f);
void Scan(FILE *f, int *arr, int len);
void Sort(int *arr, int len);
int Func(int *A, int *B, int *C, int a, int b, int c);

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

int Func(int *A, int *B, int *C, int a, int b, int c) {
    int flag1 = 1;
    int flag2 = 1;
    int i, j, flag3, flag4;
    for (i = 0; i < a; i++) {
        flag3 = 0;
        flag4 = 0;
        for (j = 0; j < b; j++) {
            if (A[i] == B[j])
                flag3 = 1;
        }
        for (j = 0; j < c; j++) {
            if (A[i] == C[j])
                flag4 = 1;
        }
        if (flag3 == 0 || flag4 == 0)
            flag1 = 0;
        if (flag3 == 0 && flag4 == 0)
            flag2 = 0;
    }
    if (flag1 == 1) {
        return 1;
    } 
    if (flag2 == 1) {
        for (i = 0; i < b; i++) {
            flag3 = 0;
            for (j = 0; j < a; j++)
                if (B[i] == A[j])
                    flag3 = 1;
            if (flag3 == 0)
                flag2 = 0;
        }
        for (i = 0; i < c; i++) {
            flag3 = 0;
            for (j = 0; j < a; j++)
                if (C[i] == A[j])
                    flag3 = 1;
            if (flag3 == 0)
                flag2 = 0;
        }
    }
    
    if (flag2 == 1) {
        return 2;
    } else {
        return 3;
    }
}

int main(void) {
    FILE *ina, *inb, *inc, *out;
    int a, b, c, check;
    int *A, *B, *C;
    ina = fopen("A.txt", "r");
    inb = fopen("B.txt", "r");
    inc = fopen("C.txt", "r");
    out = fopen("output.txt", "w");
    if (ina == NULL) 
        return -1;
    if (inb == NULL) {
        fclose(ina);
        return -1;
    }
    if (inc == NULL) {
        fclose(inc);
        return -1;
    }
    if (out == NULL) {
        fclose(ina);
        fclose(inb);
        fclose(inc);
        return -1;
    }
    a = Len(ina);
    b = Len(inb);
    c = Len(inc);

    if (a == 0 || b == 0 || c == 0) {
        fclose(ina);
        fclose(inb);
        fclose(inc);
        fclose(out);
        return -1;
    }
    rewind(ina);
    rewind(inb);
    rewind(inc);
    A = (int*)malloc(a * sizeof(int));
    B = (int*)malloc(b * sizeof(int));
    C = (int*)malloc(c * sizeof(int));
    Scan(ina, A, a);
    Scan(inb, B, b);
    Scan(inc, C, c);
    Sort(A, a);
    Sort(B, b);
    Sort(C, c);

    check = Func(A, B, C, a, b, c);
    if (check == 1)
        fprintf(out, "A = B * C");
    else if (check == 2)
        fprintf(out, "A = B + C");
    else
        fprintf(out, "NO MATCH");

    fclose(ina);
    fclose(inb);
    fclose(inc);
    fclose(out);
    free(A);
    free(B);
    free(C);
    return 0;
}

