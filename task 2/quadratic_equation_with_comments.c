#include <stdio.h>
#include <math.h>

int main() {
    double a, b, c;
    
    // Считываем коэффициенты с клавиатуры
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        return 2; // Ошибка ввода данных
    }

    // Если a равно 0, это не квадратное уравнение
    if (a == 0) {
        return 2; // Ошибка: не квадратное уравнение
    }

    double discriminant = b * b - 4 * a * c;

    // Если дискриминант меньше 0, действительных корней нет
    if (discriminant < 0) {
        return 1; // Нет действительных корней
    }

    // Если дискриминант равен 0, есть один корень
    if (discriminant == 0) {
        double x = -b / (2 * a);
        printf("%.10g\n", x); // Выводим корень
    } else {
        // Дискриминант положителен, есть два действительных корня
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        
        // Выводим корни, сортируя их по возрастанию
        printf("%.10g %.10g\n", x2, x1);
    }

    return 0; // Программа завершена успешно
}