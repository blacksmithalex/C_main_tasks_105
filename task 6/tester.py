from libtester import prepare, SR
import logging
import math
import random
import sys

def main():
    e = prepare()
    if not e:
        return
    
    errors = 0
    
    errors += not e.expect_failure("Нет файла с входными данными", arguments=["output.txt", "/a/b"])
    errors += not e.expect_failure("Нельзя создать файл с выходными данными", arguments=["/proc/1234"])
    errors += not e.expect_failure("Пустой ввод", inputs=[""], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["0 1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["2 1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["2 1 1 1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["-1 1 1 1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["3.23 1 1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["1e7 1 1"], output=True)
    errors += not e.expect_failure("Некорректная длина", inputs=["a 1 1 1"], output=True)
    errors += not e.expect_failure("Некорректный элемент", inputs=["3 a 1 1"], output=True)
    errors += not e.expect_failure("Некорректный элемент", inputs=["3 1 a 1"], output=True)
    errors += not e.expect_failure("Некорректный элемент", inputs=["3 1 1 a"], output=True)
    errors += not e.expect_failure("Некорректный элемент", inputs=["2 1 1 a"], output=True)
    
    errors += not e.expect_success("Одно число (без проверки вывода)", inputs=["1 2"], output=True)
    errors += not e.expect_success("Два числа (без проверки вывода)", inputs=["2 1 2"], output=True)
    errors += not e.expect_success("Два числа (без проверки вывода)", inputs=["  2    1    0   "], output=True)

    
    def test_valid(name, array):
        elements = " ".join(str(element) for element in array)
        result = e.expect_success(name, inputs=[f"{str(len(array))}\n{elements}"], output=True)
        if not result:
            return False
        if not result.output_file:
            print(f"ОШИБКА{SR}\nПрограмма не записала ничего в выходной файл")
            return False

        try:
            values = result.output_file.split()
            length = int(values[0])
            # len(values) == length + 1 — только новый массив
            # len(values) == length + 2 — новый массив и в конце YES/NO
            if len(values) < length + 1:
                print(f"ОШИБКА{SR}\nВ выводе недостаточно элементов")
                return False
            if len(values) > length + 3:
                print(f"ОШИБКА{SR}\nВ выводе слишком много элементов")
                return False
            if len(values) == length + 2 and values[-1] not in {"YES", "NO"}:
                print(f"ОШИБКА{SR}\nЕсли после элементов массива выводится ещё что-то, это должно быть или YES, или NO, а не: {values[-1]}")
                return False
            # проверяем, что элементы массива можно разобрать
            new_elements = [float(element) for element in values[1:length+1]]
        except Exception:
            print(f"ОШИБКА{SR}\nНе удалось разобрать вывод:")
            logging.exception("Исключение при разборе:")
            return False
        
        return True

    errors += not test_valid("Пустой массив", [])    
    errors += not test_valid("Одно число", [1, 2])
    errors += not test_valid("Два числа", [2, 1, 2])
    errors += not test_valid("Возрастающая", [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
    errors += not test_valid("Возрастающая", [0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 2000000000])
    errors += not test_valid("Возрастающая", [-1, 0, 1])
    errors += not test_valid("Возрастающая", [-2000000000, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 2000000000])

    errors += not test_valid("Убывающая", [10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10])
    errors += not test_valid("Убывающая", [-1, -10, -100, -1000, -10000, -100000, -1000000, -10000000, -100000000, -1000000000, -2000000000])

    errors += not test_valid("Постоянная", [0] * 11)
    errors += not test_valid("Постоянная", [11] * 11)
    errors += not test_valid("Постоянная", [-333] * 11)
    errors += not test_valid("Неубывающая", [1, 1, 2, 2, 3, 3, 3, 4, 5, 6, 7, 8])

    errors += not test_valid("Невозрастающая", [0, -1, -1, -2, -2, -3, -3, -4, -5, -6, -7, -7, -8])

    errors += not test_valid("Один максимум", [1, 2, 1])
    errors += not test_valid("Один максимум", [1, 2, 3, 4, 3, 2, 1])
    errors += not test_valid("Один нестрогий максимум", [1, 2, 3, 4, 4, 3, 2, 1])
    errors += not test_valid("Один максимум", [1, 2, 1000, -10])
    errors += not test_valid("Один минимум", [10, 1, 10])
    errors += not test_valid("Один минимум", [10, 5, 1, 5, 10])
    errors += not test_valid("Один нестрогий минимум", [10, 5, 1, 1, 5, 10])
    errors += not test_valid("Один минимум", [10, 1, 1000000000])

    errors += not test_valid("Локальные экстремумы", [1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2, 1])
    errors += not test_valid("Локальные экстремумы", [1, -2, 3, 2, -1, 2, 3, -2, 1, 2, -3, 2, 1, -2, 3, 2, -1, 2, 3, 2, -1, 2, 3, 2, 1])

    for _ in range(50):
        values = [random.randint(-1, 1) for _ in range(random.randint(1, 100))]
        errors += not test_valid("Случайная последовательность из -1, 0, 1", values)

    for _ in range(5):
        errors += not test_valid("Случайная последовательность из -100..100",
            [random.randint(-100, 100) for _ in range(random.randint(1000, 10000))])
    
    print(f"Тестирование завершено, количество ошибок: {errors}")

    sys.exit(errors)

main()
