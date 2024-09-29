import fcntl
import os.path
import os
from pathlib import Path
from signal import Signals # pylint: disable=no-name-in-module
import subprocess
import sys
from queue import Queue, Empty
from threading import Thread
from tempfile import NamedTemporaryFile

HR = "-" * 40
SR = "*" * 120

def run(arguments, timeout=120, verb="Выполняем", **kwargs):
    print(f"{verb}: {' '.join(arguments)}")
    try:
        result = subprocess.run(arguments, timeout=timeout, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding="utf8", **kwargs)
        print(HR)
    except subprocess.TimeoutExpired as e:
        print("Время выполнения истекло")
        print(f"Вывод программы:\n{HR}\n{e.stdout}\n{HR}")
        if e.stderr:
            printf(f"{e.stderr}\n{HR}")
        raise
    print(HR)
    if isinstance(result.stdout, bytes):
        result.stdout = result.stdout.decode()
    sys.stdout.write(result.stdout)
    print(HR)
    if result.returncode != 0:
        print(f"Выполнение завершилось с кодом: {result.returncode}")
    return result

class Executable:
    def __init__(self, *args):
        self.args = list(args)

    def run_test(self, name, arguments=[], inputs=[], output=False, input=None):
        return run_test(name, executable=self.args, arguments=arguments, inputs=inputs, output=output, input=input)
    def expect_failure(self, name, arguments=[], inputs=[], output=False, code=None, input=None):
        return expect_failure(name, executable=self.args, arguments=arguments, inputs=inputs, output=output, code=code, input=input)
    def expect_success(self, name, arguments=[], inputs=[], output=False, input=None):
        return expect_success(name, executable=self.args, arguments=arguments, inputs=inputs, output=output, input=input)

def run_test(name, executable, arguments=[], inputs=[], output=False, input=None):
    tempfiles = []
    run_arguments = list(executable)
    for i in inputs:
        tf = NamedTemporaryFile("wb")
        tf.write(i.encode())
        tf.flush()
        tempfiles.append(tf)
        run_arguments.append(tf.name)
    if output:
        tf = NamedTemporaryFile("r")
        tempfiles.append(tf)
        run_arguments.append(tf.name)
    
    print_arguments = executable + [f"входной-файл-{i}" for i, _ in enumerate(inputs)]
    if output:
        print_arguments.append("выходной-файл")
    print_arguments += [str(arg) for arg in arguments]
    run_arguments += [str(arg) for arg in arguments]

    print(f"""{HR}
Запуск теста: {name}.
Командная строка: |{' '.join(print_arguments)}|
""")
    for n, i in enumerate(inputs):
        print(f"""входной-файл-{n}:
{HR}
{i}
{HR}
""")
    if input:
        print(f"""Стандартный ввод:
{HR}
{input}
{HR}
""")
    result = run(run_arguments, input=input)
    if output:
        try:
            tf = tempfiles[-1]
            tf.seek(0)
            result.output_file = tf.read(64*1024*1024)
        except Empty:
            result.output_file = None
        print(f"""В выходной файл записано:
{HR}
{result.output_file}
{HR}
""")
    return result


def prepare():
    if Path("./a.out").exists():
        return Executable("./a.out")
    
    print("Компиляция завершилась с ошибкой, файл ./a.out не найден")
    return


def expect_failure(name, arguments=[], inputs=[], output=False, code=None, executable=[], input=None):
    result = run_test(name, executable=executable, arguments=arguments, inputs=inputs, output=output, input=input)
    if result.returncode == 0:
        print(f"{SR}\nПрограмма завершилась успешно (с кодом 0), но должна была вернуть ненулевой код ошибки")
        return False
    elif result.returncode < 0:
        print(f"{SR}\nПрограмма упала с исключением {Signals(-result.returncode).name}")
        return False
    elif code and result.returncode != code:
        print(f"{SR}\nПрограмма завершилась с кодом ошибки {result.returncode}, а требовался код {code}")
    else:
        print(f"Программа завершилась с ненулевым кодом ошибки {result.returncode}, что и требовалось по условию теста.")
    return result

def expect_success(name, arguments=[], inputs=[], output=False, executable=[], input=None):
    result = run_test(name, executable=executable, arguments=arguments, inputs=inputs, output=output, input=input)
    if result.returncode > 0:
        print(f"{SR}\nПрограмма завершилась с кодом ошибки {result.returncode} ({result.returncode - 256}), но должна была завершиться с нулевым кодом")
        return False
    if result.returncode < 0:
        print(f"{SR}\nПрограмма упала с исключением {Signals(-result.returncode).name}")
        return False
    print("Программа завершилась успешно")
    return result
