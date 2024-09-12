import sys
from os.path import abspath, join, dirname

# 设置dll所在位置，否则import的时候，会失败！！！
sys.path.insert(0, join(abspath(dirname(__file__)), '..', 'bin64', 'Debug'))
sys.path.insert(0, join(abspath(dirname(__file__)), '..', 'bin64', 'Release'))


import cmake_example as m
import ctypes
import gc

# from cmake_example import example123
# import example123

def test_main():
    assert m.__version__ == "0.0.1"
    assert m.add(1, 2) == 3
    assert m.subtract(1, 2) == -1
    assert m.mul(1, 2) == 2
    print("test_main done")


test_main()

def test_pet():
    p = m.Pet("Molly")
    print(p)
    print(p.getName())
    p.setName("Charly")
    print(p.getName())
    print(p.name)
    p.name = "Charly1"
    print(p.name)
    print(p.name_p)
    p.name_p = "Charly2"
    print(p.name_p)
    print(type(p))

    p2 = p
    p.set("name1")
    p.set(20)
    print("p=", p.name, p.age)
    print("p2=", p2.name, p2.age) 
    print("p==p2?", p==p2) #p2和p是一模一样的，p是一个指针！跟C#和java，是一样的！


    print("test_pet done")
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

smart_ptr_ref = 0
def test_smartptr():
    d = m.create_example()
    d.value = 1
    d.show_me()
    # del d

    print("test_smartptr 2222222222222")
    d2 = m.FSmartPtrCls.make() #m.create_example2()
    gc.collect()
    #print(d2, id(d2))
    d2.value = 10
    print(d2.value)
    d2.show_me()
    # m.show_me1(d2)
    # m.show_me2(d2)
    # m.show_me3(d2)

    #global smart_ptr_ref
    #smart_ptr_ref = d2

    print("test_smartptr 3333333333")
    d3 = m.FSmartPtrCls()
    #pointer = ctypes.cast(id(d3), ctypes.POINTER(ctypes.c_int))
    print(d3)
    d3.show_me()
    m.show_me1(d3)
    m.show_me2(d3)
    m.show_me3(d3)

    print("test_smartptr done")

test_smartptr()    
#smart_ptr_ref.show_me()
print("test_smartptr after")

# def test_smartptr_wrap():
#     d = m.create_example4()
#     d.get1().show_me()
#     print("test_smartptr_wrap done")

# test_smartptr_wrap()


def test_2():
    shared_obj = m.create_shared()
    print(shared_obj.get_value())  # 输出: Shared Pointer

    # 修改值
    shared_obj.set_value("New Shared Value")
    print(shared_obj.get_value())  # 输出: New Shared Value
    print(shared_obj.get_value())
    gc.collect()
    print(shared_obj.get_value())

    # 使用 unique_ptr 创建对象
    unique_obj = m.create_unique()
    print(unique_obj)  # 输出: Unique Pointer
    print(unique_obj.get_value())

    # 修改值
    unique_obj.set_value("New Unique Value")
    print(unique_obj.get_value())  # 输出: New Unique Value

# test_2()
