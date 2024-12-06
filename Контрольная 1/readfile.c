#include <stdbool.h>
#include <stdio.h>

int sumEven(const char *filename) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    printf("Ошибка открытия файла.\n");
    return -1;
  }

  int prev, curr;
  int sum = 0;
  bool isIncreasing = false;
  bool firstElement = true;

  while (fscanf(file, "%d", &curr) == 1) {
    if (firstElement) {
      prev = curr;
      firstElement = false;
      continue;
    }

    if (curr > prev) {
      isIncreasing = true;
      if (prev % 2 == 0) {
        sum += prev;
      }
    } else {
      if (isIncreasing && prev % 2 == 0) {
        sum += prev;
      }
      isIncreasing = false;
    }

    prev = curr;
  }

  if (isIncreasing && prev % 2 == 0) {
    sum += prev;
  }

  fclose(file);
  return sum;
}

int main() {
  const char *inputFile = "input.txt";
  const char *outputFile = "output.txt";

  FILE *out = fopen(outputFile, "w");
  if (!out) {
    printf("Ошибка открытия файла для вывода.\n");
    return 1;
  }

  int result = sumEven(inputFile);

  if (result == -1) {
    fprintf(out, "Ошибка обработки файла.\n");
  } else {
    fprintf(out, "%d\n", result);
  }

  fclose(out);
  return 0;
}