# 装饰器
"""
更多资料，参考： https://blog.csdn.net/qq_39784672/article/details/128289169
"""


import time


def clock(func):
    # 装饰器，计算函数执行的时间
    def clocked(*args):
        t0 = time.perf_counter()
        result = func(*args)
        elapsed = time.perf_counter() - t0
        name = func.__name__
        arg_str = ', '.join(repr(arg) for arg in args)
        print('[%0.8fs] %s(%s) -> %r' % (elapsed, name, arg_str, result))
        return result
    return clocked


def just_run(func):
    def run(*args):
        result = func(*args)
        return result
    return run


@clock
def test1(seconds):
    time.sleep(seconds)
    pass


@just_run
@clock
def test2(seconds):
    # 可以放多个装饰器
    time.sleep(seconds)
    pass


DEFAULT_FMT = '[{elapsed:0.8f}s] {name}({args}) -> {result}'


def clock2(fmt=DEFAULT_FMT): 	# clock是参数化装饰器工厂函数
    def decorate(func): 		# decorate 是真正的装饰器
        def clocked(*_args): 	# clocked 包装被装饰的函数
            t0 = time.time()
            _result = func(*_args)  # _result 是被装饰的函数返回的真正结果
            elapsed = time.time() - t0
            name = func.__name__
            args = ', '.join(repr(arg) for arg in _args)
            result = repr(_result)
            print(fmt.format(**locals()))  # **locals()是为了在fmt中引用clocked的局部变量
            return _result
        return clocked
    return decorate


@clock2('[{elapsed:0.2f}s] {name}({args}) -> {result}')
def test3(seconds):
    time.sleep(seconds)
    pass


@clock2
def test4(seconds):
    # 不会有效执行！！！ 必须写成"clock2()"才可以，对比看test5！！
    time.sleep(seconds)
    pass


@clock2()
def test5(seconds):
    time.sleep(seconds)
    pass


def test_all():
    test1(0.1)
    test2(0.3)
    test3(0.5)
    test4(0.4)
    test5(0.6)


if __name__ == '__main__':
    # 直接运行了此文件
    test_all()
else:
    # 以导入的形式，使用此模块
    pass
