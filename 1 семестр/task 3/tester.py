from libtester import prepare, SR
import math
import random
import sys

def main():
    e = prepare()
    if not e:
        return
    
    errors = 0
    errors += not e.expect_failure("Пустой ввод", input="")
    errors += not e.expect_success("Одно число", input="1")
    errors += not e.expect_success("Два числа", input="1 2")
    errors += not e.expect_failure("Буквы", input="1 2 a")
    errors += not e.expect_failure("Буквы", input="a -2 1")
    errors += not e.expect_failure("Числа с плавающей точкой", input="0.1234 5.678 9.101112 -123.533e31")

    errors += not e.expect_success("Возрастающая", input="0 1 2 3 4 5 6 7 8 9 10")
    errors += not e.expect_success("Возрастающая", input="0 1 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000 2000000000")
    errors += not e.expect_success("Возрастающая", input="-1 0 1")
    errors += not e.expect_success("Возрастающая", input="-200000000 0 1 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000 2000000000")

    errors += not e.expect_success("Убывающая", input="10 9 8 7 6 5 4 3 2 1 0 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10")
    errors += not e.expect_success("Убывающая", input="-1 -10 -100 -1000 -10000 -100000 -1000000 -10000000 -100000000 -1000000000 -2000000000")

    errors += not e.expect_success("Неубывающая", input="1 1 2 2 3 3 3 4 5 6 7 8")
    errors += not e.expect_success("Невозрастающая", input="0 -1 -1 -2 -2 -3 -3 -4 -5 -6 -7 -7 -8")

    errors += not e.expect_success("Один максимум", input="1 2 1")
    errors += not e.expect_success("Один максимум", input="1 2 3 4 3 2 1")
    errors += not e.expect_success("Один максимум", input="1 2 1000 -10")
    errors += not e.expect_success("Один минимум", input="10 1 10")
    errors += not e.expect_success("Один минимум", input="10 5 1 5 10")
    errors += not e.expect_success("Один минимум", input="10 1 1000000000")

    errors += not e.expect_success("Локальные экстремумы", input="1 2 3 2 1 2 3 2 1 2 3 2 1 2 3 2 1 2 3 2 1 2 3 2 1 ")
    errors += not e.expect_success("Локальные экстремумы", input="1 -2 3 2 -1 2 3 -2 1 2 -3 2 1 -2 3 2 -1 2 3 2 -1 2 3 2 1 ")

    for _ in range(10):
        errors += not e.expect_success("Случайная последовательность", input=" ".join(str(random.randint(-100, 100)) for _ in range(random.randint(10, 20))))

    errors += not e.expect_success("Случайная последовательность", input=" ".join(str(random.randint(-100, 100)) for _ in range(random.randint(1000, 10000))))
    errors += not e.expect_success("Случайная последовательность", input=" ".join(str(random.randint(-1000000000, 1000000000)) for _ in range(random.randint(1000, 10000))))
    
    print(f"Тестирование завершено, количество ошибок: {errors}")

    sys.exit(errors)

main()
