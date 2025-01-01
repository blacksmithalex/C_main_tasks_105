#include <stdio.h>
#include <limits.h>

int findMaxDistanceBetweenLocalMinima(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (!file) {
        printf("Ошибка открытия файла.\n");
        return -1;  
    }

    int prev, curr, next;        
    int lastMinIndex = -1;       
    int maxDistance = -1;        
    int index = 1;               

    if (fscanf(file, "%d", &prev) != 1 || fscanf(file, "%d", &curr) != 1) {
        fclose(file);
        return -1; 
    }

    while (fscanf(file, "%d", &next) == 1) {
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

    fclose(file);

    return maxDistance;
}

int main() {
    const char *inputFile = "input.txt";
    const char *outputFile = "output.txt";
    FILE *out = fopen(outputFile, "w");

    if (!out) {
        printf("Ошибка открытия файла для вывода.\n");
        return 1;
    }

    int result = findMaxDistanceBetweenLocalMinima(inputFile);

    if (result == -1) {
        fprintf(out, "Недостаточно данных или отсутствуют локальные минимумы.\n");
    } else {
        fprintf(out, "%d\n", result);
    }

    fclose(out);
    return 0;
}
