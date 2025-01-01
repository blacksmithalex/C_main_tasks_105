#include <stdio.h>
#include <limits.h>

// Функция для нахождения максимального расстояния между локальными минимумами
int findMaxDistanceBetweenLocalMinima(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (!file) {
        printf("Ошибка открытия файла.\n");
        return -1;  // Ошибка при открытии файла
    }

    int prev, curr, next;        // Три последовательных элемента
    int lastMinIndex = -1;       // Индекс последнего найденного локального минимума
    int maxDistance = -1;        // Максимальное расстояние между локальными минимумами
    int index = 1;               // Текущий индекс элемента (индексация с 1 для удобства)

    // Считываем первые два числа
    if (fscanf(file, "%d", &prev) != 1 || fscanf(file, "%d", &curr) != 1) {
        fclose(file);
        return -1; // Недостаточно данных для вычислений (например, пустой файл)
    }

    // Чтение оставшихся элементов до конца файла
    while (fscanf(file, "%d", &next) == 1) {
        // Проверяем, является ли текущий элемент локальным минимумом
        if (curr < prev && curr < next) {
            if (lastMinIndex != -1) {
                int distance = index - lastMinIndex - 1;
                if (distance > maxDistance) {
                    maxDistance = distance;
                }
            }
            // Запоминаем текущий индекс как индекс последнего локального минимума
            lastMinIndex = index;
        }

        // Продвигаем элементы: prev <- curr <- next
        prev = curr;
        curr = next;
        index++;
    }

    fclose(file);

    return maxDistance; // Если нет минимумов или недостаточно, maxDistance останется -1
}

int main() {
    // Открываем файлы input.txt и output.txt
    const char *inputFile = "input.txt";
    const char *outputFile = "output.txt";
    FILE *out = fopen(outputFile, "w");

    if (!out) {
        printf("Ошибка открытия файла для вывода.\n");
        return 1;
    }

    // Вызываем функцию для нахождения максимального расстояния между локальными минимумами
    int result = findMaxDistanceBetweenLocalMinima(inputFile);

    // Пишем результат в файл output.txt
    if (result == -1) {
        fprintf(out, "Недостаточно данных или отсутствуют локальные минимумы.\n");
    } else {
        fprintf(out, "%d\n", result);
    }

    fclose(out);
    return 0;
}
