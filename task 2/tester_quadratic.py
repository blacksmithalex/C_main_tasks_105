from libtester import prepare, SR
import math
import random
import sys

def main():
    e = prepare()
    if not e:
        return
    
    def testcase(name, input):
        result = e.expect_success(name, input=" ".join(str(i) for i in input))
        a, b, c = input
        if a > 0:
            d = b * b - 4 * a * c
            roots = set([(-b + math.sqrt(d))/2, (-b - math.sqrt(d))/2])
        else:
            roots = set([-c/b])
        if not result:
            return result
        print(roots)
        try:
            values = [float(f) for f in result.stdout.split()]
            if len(values) == 2 and abs(values[0] - values[1]) < 1e-5:
                print("Двойной корень нужно выводить только один раз")
                return False
            
            for root in roots:
                if not any(abs(x - root) < 1e-5 for x in values):
                    print("Корень не найден: ", root)
                    return False
            
            if len(roots) != len(values):
                print("Неверное количество корней. Ожидается: ", len(roots))
                return False
            
            return True
            
        except Exception:
            print("Не удалось разобрать вывод программы")
            return False

    
    errors = 0
    errors += not e.expect_failure("Пустой ввод", input="")
    errors += not e.expect_failure("Одно число", input="1")
    errors += not e.expect_failure("Два числа", input="1 2")
    errors += not e.expect_failure("Буквы", input="1 2 a")
    errors += not e.expect_failure("Буквы", input="a -2 1")

    errors += not e.expect_failure("Нет корней", input="1 2 3", code=1)

    errors += not testcase("Два корня", [1, 2, 0])
    errors += not testcase("Один корень", [1, 2, 1])
    errors += not testcase("Два корня", [1, 2, 0.999])
    errors += not testcase("Всё ещё два корня", [1, 2, 0.999999999])
    errors += not testcase("...", [10324325322435, 0, 0])
    errors += not testcase("Линейное уравнение", [0, 2, -32432])
    
    print(f"Тестирование завершено, количество ошибок: {errors}")

    sys.exit(errors)

main()
