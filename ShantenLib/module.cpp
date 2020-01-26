#include <Windows.h>
#include <cmath>
#include "ShantenCalculator.h"
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

const double e = 2.7182818284590452353602874713527;

double sinh_impl(double x) {
    return (1 - pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double cosh_impl(double x) {
    return (1 + pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double tanh_impl(double x) {
    std::vector<int> test;
    test.at(-1);
    return sinh_impl(x) / cosh_impl(x);
}

namespace py = pybind11;

PYBIND11_MODULE(ShantenLib, m) {
    m.def("fast_tanh2", &tanh_impl, R"pbdoc(
        Compute a hyperbolic tangent of a single argument expressed in radians.
    )pbdoc");
    py::class_<ShantenCalculator::ImprovementCount>(m, "ImprovementCount")
        .def(py::init())
        .def_readwrite("Neg", &ShantenCalculator::ImprovementCount::Neg)
        .def_readwrite("Zero", &ShantenCalculator::ImprovementCount::Zero)
        .def_readwrite("One", &ShantenCalculator::ImprovementCount::One)
        .def_readwrite("Two", &ShantenCalculator::ImprovementCount::Two)
        .def("__repr__",
            [](const ShantenCalculator::ImprovementCount& a) {
                std::stringstream fmt;
                fmt << "ImprovementCount(Neg=" << a.Neg << ", Zero=" << a.Zero << ", One=" << a.One << ", Two=" << a.Two << ")";
                return fmt.str();
            });
    py::class_<ShantenCalculator>(m, "ShantenCalculator")
        .def(py::init())
        .def("GetTwoStepCounts", &ShantenCalculator::CalculateTwoStep)
        .def("GetTwoShantenCounts", &ShantenCalculator::GetTwoShantenCounts)
        .def("GetOneShantenCounts", &ShantenCalculator::GetOneShantenCounts);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}