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


def test_all():
    logger.info(avg(1, 2))
    logger.info(avg(1, 2, 3, 4, 5))
    pass
