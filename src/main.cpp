#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int add(int i, int j) {
    return i + j;
}
int mul(int i, int j) 
{
    return i * j;
}

namespace py = pybind11;

extern void InitBindClass(py::module& m2);
extern void InitBindSmartPtr(py::module& m);

PYBIND11_MODULE(cmake_example, m) {
    InitBindClass(m);
    InitBindSmartPtr(m);

    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("mul", &mul, R"pbdoc(
        mul two numbers

        Some other explanation about the add function.
    )pbdoc");

#ifdef VERSION_INFO
    //m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
    m.attr("__version__") = "0.0.1";
#else
    m.attr("__version__") = "dev";
#endif
}
