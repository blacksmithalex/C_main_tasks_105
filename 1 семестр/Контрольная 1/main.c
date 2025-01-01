#include <stdio.h>
#include <stdlib.h>

int main() {
    int prev, curr, sum = 0, segment_sum = 0, first = 1, in_segment = 0;
    char input[50];
    int count = 0;

    while (scanf("%s", input) != EOF) {
        char *end;
        curr = strtol(input, &end, 10);
        count++;

        if (*end != '\0') {
            printf("Ошибка ввода\n");
            return 1;
        }

        if (first) {
            prev = curr;
            first = 0;
            in_segment = 0;
        } else {
            if (curr > prev) {
                if (!in_segment) {
                    in_segment = 1;
                    segment_sum = (prev % 2 == 0) ? prev : 0;
                }
                if (curr % 2 == 0) {
                    segment_sum += curr;
                }
            } else {
                if (in_segment) {
                    sum += segment_sum;
                    in_segment = 0; 
                }
            }
            prev = curr;
        }
    }
    
    if (in_segment) {
        sum += segment_sum;
    }

    printf("%d\n", sum);
    return 0;
}
