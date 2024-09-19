# coding=utf-8
# 开启asan，这样很容易发现问题： C++ -> 命令行 -> 追加：  /fsanitize=address /Zi
#           然后把clang_rt.asan_dynamic-x86_64.dll拷贝到dll目录

from loguru import logger
import gc
import ctypes
import faulthandler
import sys
from os.path import abspath, join, dirname
import basic.func

# 临时关掉格式化代码，否则，会导致导入失败！！
# autopep8: off
# 先设置dll所在位置，否则import的时候，会失败！！！
sys.path.insert(0, join(abspath(dirname(__file__)), '..', 'bin64', 'Debug'))
sys.path.insert(0, join(abspath(dirname(__file__)), '..', 'bin64', 'Release'))
# 再导入dll
import cmake_example as m
# autopep8: on

faulthandler.enable()


# from cmake_example import example123
# import example123

def test_loguru():
    logger.trace("Executing program")
    logger.debug("Processing data...")
    logger.info("Server started successfully.")
    logger.success("Data processing completed successfully.")
    logger.warning("Invalid configuration detected.")
    logger.error("Failed to connect to the database.")
    logger.critical("Unexpected system error occurred. Shutting down.")
# test_loguru()


basic.func.test1()

# autopep8: off
# 导入，并立即执行！
import basic.import_and_run
# autopep8: on


def test_main():
    assert m.__version__ == "0.0.1"
    assert m.add(1, 2) == 3
    assert m.subtract(1, 2) == -1
    assert m.mul(1, 2) == 2
    logger.info("test_main done")


test_main()


def test_pet():
    p = m.Pet("Molly")
    logger.info(p)
    logger.info(p.getName())
    p.setName("Charly")
    logger.info(p.getName())
    logger.info(p.name)
    p.name = "Charly1"
    logger.info(p.name)
    logger.info(p.name_p)
    p.name_p = "Charly2"
    logger.info(p.name_p)
    logger.info(type(p))

    p2 = p
    p.set("name1")
    p.set(20)
    logger.info("p=", p.name, p.age)
    logger.info("p2=", p2.name, p2.age)
    logger.info("p==p2?", p == p2)  # p2和p是一模一样的，p是一个指针！跟C#和java，是一样的！

    logger.info("test_pet done")
    # p.showme()


test_pet()


def test_anim():
    d = m.Dog()
    m.call_go(d)


class Cat(m.Animal):
    def go(self, n_times):
        return "meow! " * n_times


test_anim()
# c = Cat()
# m.call_go(c)


def test_unique_ptr():
    d = m.OuterClass()
    d.obj.method()
    logger.info("test_unique_ptr done.")


test_unique_ptr()


def test_unique_ptr2():
    p = m.Parent()
    c = p.get_child()
    logger.info(c.hello())
    arr = p.get_children()
    logger.info(arr)
    logger.info(arr[0].hello())
    arr[0] = c
    logger.info(arr)
    logger.info(arr[0].hello())


test_unique_ptr2()

# unique_ptr的例子


def test_unique_ptr3():
    logger.info("test unique")
    a = m.create_unique()
    a.show_me()
    logger.info("123:", m.create_unique().value)
    d = m.create_unique()
    d.value = 1
    logger.info(d.value)
    del d
    # d.value = 1
    # d.value = 2
    # d.show_me()
    # del d


test_unique_ptr3()

smart_ptr_ref = 0


def test_smartptr():

    logger.info("test_smartptr 2222222222222")
    d2 = m.FSmartPtrCls.make()  # m.create_example2()
    gc.collect()
    # logger.info(d2, id(d2))
    d2.value = 10
    logger.info(d2.value)
    d2.show_me()
    # m.show_me1(d2)
    # m.show_me2(d2)
    # m.show_me3(d2)

    # global smart_ptr_ref
    # smart_ptr_ref = d2

    logger.info("test_smartptr 3333333333")
    d3 = m.FSmartPtrCls()
    # pointer = ctypes.cast(id(d3), ctypes.POINTER(ctypes.c_int))
    logger.info(d3)
    d3.show_me()
    m.show_me1(d3)
    m.show_me2(d3)
    m.show_me3(d3)

    logger.info("test_smartptr done")


test_smartptr()
# smart_ptr_ref.show_me()
logger.info("test_smartptr after")

# def test_smartptr_wrap():
#     d = m.create_example4()
#     d.get1().show_me()
#     logger.info("test_smartptr_wrap done")

# test_smartptr_wrap()


def test_2():
    shared_obj = m.create_shared()
    logger.info(shared_obj.get_value())  # 输出: Shared Pointer

    # 修改值
    shared_obj.set_value("New Shared Value")
    logger.info(shared_obj.get_value())  # 输出: New Shared Value
    logger.info(shared_obj.get_value())
    gc.collect()
    logger.info(shared_obj.get_value())

    # 使用 unique_ptr 创建对象
    unique_obj = m.create_unique()
    logger.info(unique_obj)  # 输出: Unique Pointer
    logger.info(unique_obj.get_value())

    # 修改值
    unique_obj.set_value("New Unique Value")
    logger.info(unique_obj.get_value())  # 输出: New Unique Value

# test_2()
