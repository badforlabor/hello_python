// 更多例子，参考这里： https://pybind11.readthedocs.io/en/stable/classes.html
// 绑定：智能指针
// 对象生命周期，参考这里：https://blog.csdn.net/qq_39784672/article/details/128422661


#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/stl.h>
#include <iostream>
#include <memory>
#include <Windows.h>
namespace py = pybind11;


class FSmartPtrCls
{
public:
	FSmartPtrCls()
		: Value(0)
	{
		py::print("FSmartPtrCls ctor", this, Value);
	}
	~FSmartPtrCls()
	{
		py::print("~FSmartPtrCls dtor", this);
	}
	void ShowMe()
	{
		py::print("FSmartPtrCls showme! Value=", Value);
	}
	int Value;
};
class FUniquePtrCls
{
public:
	FUniquePtrCls()
		: Value(0)
	{
		py::print("FUniquePtrCls ctor", this, Value);
	}
	~FUniquePtrCls()
	{
		py::print("~FUniquePtrCls dtor", this);
	}
	void ShowMe()
	{
		py::print("FUniquePtrCls showme! Value=", Value);
	}
	int Value;
};


// 尝试用Wrap的形式！
template <typename T>
class shared_ptr_with_addressof_operator {
	std::shared_ptr<T> impl;

public:
	shared_ptr_with_addressof_operator() = default;
	explicit shared_ptr_with_addressof_operator(T* p) : impl(p) {}
	T* get() const { return impl.get(); }
	T** operator&() { throw std::logic_error("Call of overloaded operator& is not expected"); }
};
PYBIND11_DECLARE_HOLDER_TYPE(T, shared_ptr_with_addressof_operator<T>)
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>)

// 严重错误！！FSmartPtrCls在python中，只能二选一，不能unique_ptr和shared_ptr同时注册！
std::unique_ptr<FSmartPtrCls> create_example() { return std::unique_ptr<FSmartPtrCls>(new FSmartPtrCls()); }
std::shared_ptr<FSmartPtrCls> create_example2() { return std::shared_ptr<FSmartPtrCls>(new FSmartPtrCls()); }

std::unique_ptr<FUniquePtrCls> create_unique() { return std::move(std::make_unique<FUniquePtrCls>()); }


void ShowMe(std::shared_ptr<FSmartPtrCls> Ptr) { Ptr->ShowMe(); }
void ShowMe(FSmartPtrCls* Ptr) { Ptr->ShowMe(); }
void ShowMe(FSmartPtrCls& Ptr) { Ptr.ShowMe(); }



class MyClass {
public:
	MyClass(const std::string& value) : data(value) {}

	void set_value(const std::string& value) {
		data = value;
	}

	std::string get_value() const {
		return data;
	}

private:
	std::string data;
};

void InitBindSmartPtr2(py::module& m)
{
	py::class_<MyClass, std::shared_ptr<MyClass>>(m, "MyClass")
		.def(py::init<const std::string&>())
		.def("set_value", &MyClass::set_value)
		.def("get_value", &MyClass::get_value);

	//m.def("create_shared", []() {
	//	return std::make_shared<MyClass>("Shared Pointer");
	//	});

	//m.def("create_unique", []() {
	//	return std::unique_ptr<MyClass>(new MyClass("Unique Pointer"));
	//	});
}

class InnerClass {
public:
	void method()
	{
		std::cout << "OK" << std::endl;
	}
};

class OuterClass {
public:
	OuterClass()
		: obj(std::make_unique<InnerClass>())
	{
	}

	std::unique_ptr<InnerClass> obj;
};

static void InitBindSmartPtr4(py::module& m)
{
	py::class_<InnerClass>(m, "InnerClass")
		.def("method", &InnerClass::method);

	py::class_<OuterClass>(m, "OuterClass", py::dynamic_attr())
		.def(py::init<>())
		.def_property_readonly("obj", [](const OuterClass& o) {return o.obj.get(); }, py::return_value_policy::reference_internal); // 特别注意，返回策略！！
}


class Child {

public:
	Child(int num) : _num(num) {}

	std::string helloChild() {
		return "Hi there Child" + std::to_string(_num);
	}

	int _num;
};

class Parent {

public:

	// Problem 1:
	const std::unique_ptr<Child>& get_Child() {
		m_Child = std::make_unique<Child>(6);
		return m_Child;
	};

	// Problem 2:
	std::vector<std::unique_ptr<Child>>&& get_Childs() {
		auto some_child = std::make_unique<Child>(1);
		m_Childs.push_back(std::move(some_child));
		auto other_child = std::make_unique<Child>(2);
		m_Childs.push_back(std::move(other_child));
		return std::move(m_Childs);
	};

private:
	std::unique_ptr<Child> m_Child;
	std::vector<std::unique_ptr<Child>> m_Childs;
};

static void InitBindSmartPtr5(py::module& m)
{
	py::class_<Child>(m, "Child")
		.def(py::init<int>())
		.def("hello", &Child::helloChild)
		.def("__repr__", [](const Child* self) {
		return "Child" + std::to_string(self->_num);
			});

	py::class_<Parent>(m, "Parent")
		.def(py::init<>())
		.def("get_child", [](Parent* parent) {
		return parent->get_Child().get();
			}, py::return_value_policy::reference)  // important policy!
		.def("get_children", [](Parent* parent) {
				return parent->get_Childs();
			}, py::return_value_policy::reference_internal);  // important policy!

	py::class_<std::unique_ptr<Child>>(m, "UniquePtrChild")
		.def(py::init<>());

	//py::bind_vector<std::vector<std::unique_ptr<Child>>>(m, "ChildPtrVector");

}


void InitBindSmartPtr(py::module& m)
{
	if (false)
	{
		extern void WaitForDebugger();
		WaitForDebugger();
	}

	InitBindSmartPtr5(m);

	InitBindSmartPtr4(m);

	InitBindSmartPtr2(m);

	// 必须加上std::shared_ptr<FSmartPtrCls>，表示智能指针管理生命周期！否则使用智能指针时，会出错！
	// 而且，只能定义一次。比如加了下面的定义，就无法这样：py::class_<FSmartPtrCls>(m, "FSmartPtrCls")
	py::class_<FSmartPtrCls, std::shared_ptr<FSmartPtrCls>>(m, "FSmartPtrCls")
		.def(py::init())
		.def("show_me", &FSmartPtrCls::ShowMe)
		//.def_readwrite("value", &FSmartPtrCls::Value)
		.def_static("make", []()
			{
				return std::shared_ptr<FSmartPtrCls>(new FSmartPtrCls());
			}/*, py::return_value_policy::reference*/)
		.def_property("value", 
			[](FSmartPtrCls* Ptr) 
			{
				py::print(Ptr, "getValue", Ptr->Value);
				return Ptr->Value; 
			}, 
			[](FSmartPtrCls* Ptr, int V) 
			{ 
				py::print(Ptr, "setValue", V);
				Ptr->Value = V; 
			});

	m.def("create_example", &create_example2);	// 如果用create_example就会出错。即如果使用shared_ptr内存分配方式，就不能用unique_ptr！！
	m.def("create_example2", &create_example2);

	m.def("show_me1", py::overload_cast<std::shared_ptr<FSmartPtrCls>>(&ShowMe) );
	m.def("show_me2", py::overload_cast<FSmartPtrCls*>(&ShowMe));
	m.def("show_me3", py::overload_cast<FSmartPtrCls&>(&ShowMe));


	py::class_<FUniquePtrCls, std::unique_ptr<FUniquePtrCls/*, py::nodelete*/>>(m, "FUniquePtrCls")
		//.def(py::init())
		.def("show_me", &FUniquePtrCls::ShowMe)
		.def_static("make", &create_unique)
		//.def_readwrite("value", &FSmartPtrCls::Value)
		.def_property("value",
			[](const FUniquePtrCls* Ptr)
			{
				py::print("getValue", Ptr->Value);
				return Ptr->Value;
			},
			[](FUniquePtrCls* Ptr, int V)
			{
				py::print(Ptr, "setValue", V);
				Ptr->Value = V;
			});
	m.def("create_unique", &create_unique/*, py::return_value_policy::reference_internal*/);
}