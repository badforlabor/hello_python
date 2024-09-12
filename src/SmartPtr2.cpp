// 更多例子，参考这里： https://pybind11.readthedocs.io/en/stable/classes.html
// 绑定：智能指针
// https://blog.csdn.net/qq_39784672/article/details/128422661


#include <pybind11/pybind11.h>
namespace py = pybind11;



// 普通类，但是给智能指针使用的！
class FNormalCls
{
public:
	FNormalCls()
		: Value("none")
	{
		py::print("FNormalCls ctor", this);
	}
	~FNormalCls()
	{
		py::print("~FNormalCls dtor", this);
	}
	void ShowMe()
	{
		py::print("FNormalCls showme! Value=", Value);
	}
	std::string Value;
};
template<class T>
struct FSmartPtrWrap
{
	std::shared_ptr<T> impl;
	explicit FSmartPtrWrap(T* p) : impl(p) {}
	T* get() const { return impl.get(); }
};


using FNormalClsPtr = FSmartPtrWrap<FNormalCls>;
FNormalClsPtr create_example4() { return FNormalClsPtr(new FNormalCls()); }



void InitBindSmartPtr3(py::module& m)
{
	py::class_<FNormalCls>(m, "FNormalCls")
		.def(py::init<>())
		.def("show_me", &FNormalCls::ShowMe)
		.def_readwrite("value", &FNormalCls::Value);

	// 这种方式，尽管可以跑通，但是非常危险！！
	// 风险在于，提供的get1方法，返回了一个指针，此指针如果被python持有住，会导致清理指针时的异常情况！
	// 所以，如果是智能指针，一定要用智能指针的形式管理生命周期！！
	py::class_<FNormalClsPtr>(m, "FNormalClsPtr")
		.def_static("make", []() {return create_example4(); })
		.def("get1", [](const FNormalClsPtr& self) {return self.get(); }, py::return_value_policy::reference);	// 必须加上py::return_value_policy::reference，否则对象会被复制，导致会产生额外的析构！
	m.def("create_example4", &create_example4);
}