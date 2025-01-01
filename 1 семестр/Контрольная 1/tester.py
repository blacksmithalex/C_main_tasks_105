from libtester import prepare, SR
import math
import random
import sys

def only_increasing(elements):
    if not elements:
        return
    current = elements[0]
    increasing = False
    for e in elements[1:]:
        if e > current or increasing:
            yield current
        increasing = e > current
        current = e
    if increasing:
        yield current

def main():
    e = prepare()
    if not e:
        return

    def run_test(pre_values):
        values = list(str(element) for element in pre_values)
        solution = sum(x for x in only_increasing(pre_values) if x % 2 == 0)
        #print(pre_values, list(only_increasing(pre_values)), solution)

        result = e.expect_success(" ".join(values)[:32] + "...", input=" ".join(values))
        if not result:
            return False

        try:
            output = int(result.stdout)
            if solution != output:
                print(f"ОШИБКА {SR}\nОтвет не совпадает. Ожидаемый ответ:")
                print(solution)
                return False
            else:
                return True
        except Exception:
            print(f"ОШИБКА {SR}\nТестеру не удалось считать выходные данные")
            return False


    errors = 0
    errors += not e.expect_failure("Буквы", input="1 2 a")
    errors += not e.expect_failure("Буквы", input="a -2 1")
    errors += not e.expect_failure("Числа с плавающей точкой", input="0.1234 5.678 9.101112 -123.533e31")

    errors += not run_test([])
    errors += not run_test([0])
    errors += not run_test([0, 0])
    errors += not run_test([0, 0, 0])
    errors += not run_test([1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1])
    errors += not run_test([1, 1, 5, 2, 8, 3, 10, 4, 12, 5, 14, 6, 16, 7, 18, 8, 20, 9])
    errors += not run_test([1, 2, 1, 2, 1, 2, 1, 2, 1, 2])
    errors += not run_test([1, -1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, 7, -7, 8, -8, 9, -9])
    errors += not run_test([1, -1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, 7, -7, 8, 8, -8, 9, -9])
    errors += not run_test([1, -1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, 7, -7, 8, 7, -8, 9, -9])
    errors += not run_test([1, -1, 2, -2, 3, -3, 4, -4, 5, -5, 6, -6, 7, -7, 8, 8, -8, 9, -9, -9, -10])
    errors += not run_test([1, 2, 3, 4, 5, 6, 7, 8, 9])
    errors += not run_test([1, 1, 2, 3, 3])
    errors += not run_test([1, 1, 3, 3, 3])
    errors += not run_test([-9, -8, 7, -6, -5, -4, -3, -2, -1])

    for _ in range(10):
        errors += not run_test([random.randint(-2, 2) for _ in range(random.randint(20, 200))])
        errors += not run_test([random.randint(-2, 2) - index for index in range(random.randint(20, 200))])
        errors += not run_test([random.randint(-100, 100) for _ in range(random.randint(10000, 100000))])
    errors += not run_test([random.randint(-10000000, 10000000) for _ in range(random.randint(10, 100))])

    print(f"Тестирование завершено, количество ошибок: {errors}")

    sys.exit(errors)

main()
