# 测试函数
"""
测试各种函数相关的
1. 基础语法
2. 装饰器
3. 参数装饰器
4. 闭包中的变量

"""

from loguru import logger


def test1():
    logger.info("func test1")


def avg(first, *rest):
    return (first + sum(rest)) / (1 + len(rest))


def add(x: int, y: int) -> int:
    # 函数的元信息。但，运行时，不会检查！！
    return x + y


def spam(a, b=42):
    # 函数默认值
    print(a, b)


_no_value = object()


def spam2(a, b=_no_value):
    if b is _no_value:
        print('No b value supplied')


x = 42


def spam3(a, b=x):
    print(a, b)


def test_all():
    logger.info(avg(1, 2))
    logger.info(avg(1, 2, 3, 4, 5))
    logger.info(add(3, 4))
    logger.info(add(3.0, 4.0))  # 依然正常！
    logger.info(add.__annotations__)  # 打印函数的一些信息
    logger.info(spam(3))
    logger.info(spam(3, 4))
    logger.info(spam2(3))
    logger.info(spam2(3, 4))

    x = 43
    spam3(1)  # 打印的b，依然是43，因为b=x只执行一次！

    pass
