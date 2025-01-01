#include <stdio.h>

#include <stdlib.h>

#include <limits.h>

#define MAX_INPUT_SIZE 50
#define BASE 10

int main()
{
    int prev;
    int curr;
    int next;
    int lastMinIndex = -1;
    int maxDistance = -1;
    int index = 1;
    char input[MAX_INPUT_SIZE];
    int singleElement = 1;

    if (scanf("%49s", input) != 1) {
        return 1;
    }

    char *end;
    long temp = strtol(input, &end, BASE);
    if (*end != '\0' || temp < INT_MIN || temp > INT_MAX) {
        return 1;
    }
    prev = (int)temp;

    if (scanf("%49s", input) != 1) {
        printf("0\n");
        return 0;
    }

    singleElement = 0;
    temp = strtol(input, &end, BASE);
    if (*end != '\0' || temp < INT_MIN || temp > INT_MAX) {
        return 1;
    }
    curr = (int)temp;

    while (scanf("%49s", input) == 1) {
        temp = strtol(input, &end, BASE);
        if (*end != '\0' || temp < INT_MIN || temp > INT_MAX) {
            return 1;
        }
        next = (int)temp;

        if (curr < prev && curr < next) {
            if (lastMinIndex != -1) {
                int distance = index - lastMinIndex - 1;
                if (distance > maxDistance) {
                    maxDistance = distance;
                }
            }
            lastMinIndex = index;
        }

        prev = curr;
        curr = next;
        index++;
    }

    if (maxDistance == -1) {
        if (singleElement) {
            printf("0\n");
        } else {
            printf("%d\n", maxDistance);
        }
    }

    return 0;
}
