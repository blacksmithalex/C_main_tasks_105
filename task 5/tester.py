from libtester import prepare, SR
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
    errors += not e.expect_failure("Не существует второй файл с входными данными", arguments=["tst", "/dev/stdin", "/proc/1234"], input="1 2 3")

    errors += not e.expect_failure("Пустой ввод", inputs=[""], output=True)
    errors += not e.expect_success("Одно число", inputs=["1"], output=True)
    errors += not e.expect_success("Два числа", inputs=["1 2"], output=True)
    errors += not e.expect_success("Два числа", inputs=["1", "2"], output=True)
    errors += not e.expect_failure("Буквы", inputs=["1 ", "2 a"], output=True)
    errors += not e.expect_failure("Буквы", inputs=["a ", "-2 1"], output=True)
    errors += not e.expect_failure("Числа с плавающей точкой", inputs=["0.1234 5.678 9.101112 -123.533e31"], output=True)

    errors += not e.expect_success("Возрастающая", inputs=["0 1 2 3 4 5 6 7 8 9 10"], output=True)
    errors += not e.expect_success("Возрастающая", inputs=["0 1 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000 2000000000"], output=True)
    errors += not e.expect_success("Возрастающая", inputs=["-1 0 1"], output=True)
    errors += not e.expect_success("Возрастающая", inputs=["-200000000 0 1 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000 2000000000"], output=True)

    errors += not e.expect_success("Убывающая", inputs=["10 9 8 7 6 5 4 3 2 1 0 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10"], output=True)
    errors += not e.expect_success("Убывающая", inputs=["-1 -10 -100 -1000 -10000 -100000 -1000000 -10000000 -100000000 -1000000000 -2000000000"], output=True)

    errors += not e.expect_success("Неубывающая", inputs=["1 1", "2 2", "3" , "3" , "3", "4", "5", "6 7 8"], output=True)
    errors += not e.expect_success("Невозрастающая", inputs=["0 -1", "-1 -2", "-2 -3 -3 -4", "-5 -6 -7 -7 -8"], output=True)

    errors += not e.expect_success("Один максимум", inputs=["1 2 1"], output=True)
    errors += not e.expect_success("Один максимум", inputs=["1 2 3 4 3 2 1"], output=True)
    errors += not e.expect_success("Один максимум", inputs=["1 2 1000 -10"], output=True)
    errors += not e.expect_success("Один минимум", inputs=["10 1 10"], output=True)
    errors += not e.expect_success("Один минимум", inputs=["10 5 1 5 10"], output=True)
    errors += not e.expect_success("Один минимум", inputs=["10 1 1000000000"], output=True)

    errors += not e.expect_success("Локальные экстремумы", inputs=["1 2", "3", "2 1 2 3 2 1 2 3 2 1 2 3 2 1 2 3 2 1 2 3 2 1 "], output=True)
    errors += not e.expect_success("Локальные экстремумы", inputs=["1 -2 3 2 -1 2 3 -2 1 2 -3 2 1 -2 3 2 -1 2 3 2 -1 2 3 2 1 "], output=True)

    for _ in range(20):
        files = list(" ".join(str(random.randint(-1, 1)) for _ in range(random.randint(0, 100))) for _ in range(random.randint(1, 100)))
        separate = e.expect_success("Случайная последовательность — раздельно", inputs=files, output=True)
        concatenated = e.expect_success("Случайная последовательность — в одном файле", output=True, inputs=["\n".join(files)])
        if not separate or not concatenated:
            errors += 1
            continue
        if separate.output_file.split() != concatenated.output_file.split():
            errors += 1
            print("Результаты для одних и тех же данных при вводе из одного и из нескольких файлов отличаются")



    errors += not e.expect_success("Случайная последовательность", inputs=[" ".join(str(random.randint(-100, 100)) for _ in range(random.randint(1000, 10000)))], output=True)
    
    print(f"Тестирование завершено, количество ошибок: {errors}")

    sys.exit(errors)

main()
