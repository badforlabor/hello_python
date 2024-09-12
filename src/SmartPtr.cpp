// 更多例子，参考这里： https://pybind11.readthedocs.io/en/stable/classes.html
// 绑定：智能指针


#include <pybind11/pybind11.h>
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

std::unique_ptr<FSmartPtrCls> create_example() { return std::unique_ptr<FSmartPtrCls>(new FSmartPtrCls()); }
std::shared_ptr<FSmartPtrCls> create_example2() { return std::shared_ptr<FSmartPtrCls>(new FSmartPtrCls()); }


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

	m.def("create_shared", []() {
		return std::make_shared<MyClass>("Shared Pointer");
		});

	m.def("create_unique", []() {
		return std::unique_ptr<MyClass>(new MyClass("Unique Pointer"));
		});
}


void InitBindSmartPtr(py::module& m)
{
	InitBindSmartPtr2(m);

	// 必须加上std::shared_ptr<FSmartPtrCls>，表示智能指针管理生命周期！否则使用智能指针时，会出错！
	py::class_<FSmartPtrCls, std::shared_ptr<FSmartPtrCls>>(m, "FSmartPtrCls")
		.def(py::init())
		.def("show_me", &FSmartPtrCls::ShowMe)
		//.def_readwrite("value", &FSmartPtrCls::Value)
		.def_static("make", []()
			{
				return std::shared_ptr<FSmartPtrCls>(new FSmartPtrCls());
			}, py::return_value_policy::reference)
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

	m.def("create_example", &create_example);
	m.def("create_example2", &create_example2);

	m.def("show_me1", py::overload_cast<std::shared_ptr<FSmartPtrCls>>(&ShowMe) );
	m.def("show_me2", py::overload_cast<FSmartPtrCls*>(&ShowMe));
	m.def("show_me3", py::overload_cast<FSmartPtrCls&>(&ShowMe));
}