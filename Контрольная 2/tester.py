from libtester import prepare, SR
import logging
import math
import os
import random
import sys

def main():
    e = prepare()
    if not e:
        return

    os.environ["ASAN_OPTIONS"] = "exitcode=154"

    errors = 0

    def test_valid(x, y, name="Корректные данные"):
        x_str = " ".join(str(el) for el in x)
        y_str = " ".join(str(el) for el in y)

        result = e.expect_success(name, inputs=[x_str, y_str])
        if not result:
            return result

        try:
            output_str = result.stdout.strip().split()
            answer = output_str[0]
            output = [int(el.strip()) for el in output_str[1:]]
        except Exception:
            print(f"ОШИБКА{SR}\nНе удалось разобрать вывод (в выводе есть что-то кроме YeS/nO и последовательности целых чисел, разделённых пробелами)")
            logging.exception("")
            return False

        if answer not in {'YEs', 'No'}:
            print(f'ОШИБКА{SR}\nНекорректный формат ответа: ', answer, '(ожидается YEs или No)')
            return False

        not_in_order = list((o1, o2) for o1, o2 in zip(output, output[1:]) if abs(o1) > abs(o2))
        if len(output) != len(x) or not_in_order:
            print(f'ОШИБКА{SR}\nВ ответе найдены не все элементы массива X, или они не упорядочены по возрастанию модулей элементов: {str(not_in_order[0][0])} {str(not_in_order[0][1])}')
            return False

        x = sorted(x, reverse=True)
        y = sorted(y, reverse=True)
        d_x = set(x2 - x1 for x1, x2 in zip(x, x[1:]))
        d_y = set(y2 - y1 for y1, y2 in zip(y, y[1:]))

        d = d_x.union(d_y)
        if len(d) > 1 and answer != "No":
            print(f'ОШИБКА{SR}\nОжидается ответ No, т.к. у последовательностей есть разности: {", ".join(d)}')
            return False
        if len(d) <= 1 and answer != "YEs":
            print(f'ОШИБКА{SR}\nОжидается ответ YEs, т.к. у последовательностей определено не более одной разности: {", ".join(d)}')
            return False
        return True

    errors += not e.expect_failure("Нет аргументов", arguments=[])
    errors += not e.expect_failure("Нет первого файла с входными данными", arguments=["/a/b"])
    errors += not e.expect_failure("Нет второго файла с входными данными", arguments=["/dev/null", "/a/b"])
    errors += not e.expect_failure("Некорректный элемент первого массива", inputs=["3 a 1 1", ""])
    errors += not e.expect_failure("Некорректный элемент первого массива", inputs=["3 1 a 1", ""])
    errors += not e.expect_failure("Некорректный элемент первого массива", inputs=["3 1 1 a", ""])
    errors += not e.expect_failure("Некорректный элемент первого массива", inputs=["2 1 1 a", ""])
    errors += not e.expect_failure("Некорректный элемент второго массива", inputs=["", "3 a 1 1"])
    errors += not e.expect_failure("Некорректный элемент второго массива", inputs=["", "3 1 a 1"])
    errors += not e.expect_failure("Некорректный элемент второго массива", inputs=["", "3 1 1 a"])
    errors += not e.expect_failure("Некорректный элемент второго массива", inputs=["", "2 1 1 a"])

    errors += not e.expect_failure("Пустой ввод", inputs=[" ", ""])
    errors += not e.expect_failure("Пустой X", inputs=[" ", "1"])
    errors += not e.expect_failure("Пустой X", inputs=["a", "1"])
    errors += not e.expect_failure("Пустой Y", inputs=["1", " "])
    errors += not e.expect_failure("Пустой Y", inputs=["1", "a"])

    errors += not test_valid([1, -1, 2, 3], [1, 2, 3, -1], "Идентичные массивы")
    errors += not test_valid([1, 1, 1, 1], [2, 2], "Равные элементы")

    errors += not test_valid([0], [0], "Равные массивы из одного элемента")
    errors += not test_valid([1], [0], "Неравные массивы из одного элемента")
    errors += not test_valid([1], [-1], "Неравные массивы из одного элемента")

    errors += not test_valid([1, 1, 1, 1], [2], "Неопределённая разность Y")
    errors += not test_valid([1], [2, 2], "Неопределённая разность X")

    errors += not test_valid([1, 2, 4, 5], [1, 2, 3, 4])
    errors += not test_valid([1, 2, 3], [1, 2, 4, 5])
    errors += not test_valid([1, 2, 3, 4, 6], [-10, 100, 190])

    errors += not test_valid([-1, 2], [2, 5])
    errors += not test_valid([2, -1], [5, 2])
    errors += not test_valid([-1, 0, -3, -2], [-5, -6, -7])
    errors += not test_valid([-1, 0, -3, -2], [-5, -6, -8])

    sequence = range(0, 50)
    x = list(sequence[::2])
    y = list(sequence[1::2])
    for i in range(10):
        random.shuffle(x)
        random.shuffle(y)
        errors += not test_valid(x, y, "Перестановки последовательности 0..50")

    for i in range(20):
        errors += not test_valid([random.randint(-10, 10) for _ in range(random.randint(1, 4))], [random.randint(-10, 10) for _ in range(random.randint(1, 4))], "Случайная последовательность")

    for i in range(4):
        errors += not test_valid([random.randint(-1000, 10000) for _ in range(random.randint(10, 2000))], [random.randint(-10, 1000) for _ in range(random.randint(10, 2000))], "Большая случайная последовательность")

    print(f"Тестирование завершено, количество ошибок: {errors}")

    sys.exit(errors)

main()
