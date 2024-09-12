// 更多例子，参考这里： https://pybind11.readthedocs.io/en/stable/classes.html
// 绑定类

#include <pybind11/pybind11.h>
namespace py = pybind11;


class Animal {
public:
    virtual std::string go(int n_times) = 0;
    virtual std::string name() { return "unknown"; }
};

class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i = 0; i < n_times; ++i)
            result += bark() + " ";
        return result;
    }
    virtual std::string bark() { return "woof!"; }
};

std::string call_go(Animal* animal) {
    return animal->go(3);
}


struct Pet {
    Pet(const std::string& name) : name(name) { }
    void setName(const std::string& name_) { name = name_; }
    const std::string& getName() const { return name; }

    void set(int age_) { age = age_; }
    void set(const std::string& name_) { name = name_; }

    std::string name;
    int age;
};


static void init3(py::module& m)
{
    py::class_<Pet>(m, "Pet") // 绑定一个类
        .def(py::init<const std::string&>()) // 绑定构造函数!!! 不这样做，在python中，无法这样调用：p = Pet("123")
        .def("setName", &Pet::setName) // 绑定一个函数
        .def("getName", &Pet::getName)
        .def_readwrite("name", &Pet::name)  // 绑定一个变量
        .def_readonly("age", &Pet::age)
        .def_property("name_p", &Pet::getName, &Pet::setName) // 绑定一个property
        .def("show_me", [](const Pet& a) { // 绑定一个lambda表达式
            return "<example.Pet named '" + a.name + "'>";
            })
        .def("set", py::overload_cast<int>(&Pet::set), "Set the pet's age")
        .def("set", py::overload_cast<const std::string&>(&Pet::set), "Set the pet's name") // 函数重载
        .def("__repr__", // 定义python的print函数。
            [](const Pet& a) {
                return "<example.Pet named '" + a.name + "'>";
            });
}

void InitBindClass(py::module& m2)
{
    init3(m2);

    py::class_<Animal>(m2, "Animal")
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m2, "Dog")
        .def(py::init<>());

    m2.def("call_go", &call_go); // 绑定一个全局函数！
}
