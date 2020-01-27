#include <Windows.h>
#include <cmath>
#include "ShantenCalculator.h"
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

PYBIND11_MODULE(ShantenLib, m) {
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
    py::class_<ShantenCalculator::ImprovementCountList>(m, "ImprovementCountList")
        .def(py::init())
        .def_readwrite("Neg", &ShantenCalculator::ImprovementCountList::Neg)
        .def_readwrite("Zero", &ShantenCalculator::ImprovementCountList::Zero)
        .def_readwrite("One", &ShantenCalculator::ImprovementCountList::One)
        .def_readwrite("Two", &ShantenCalculator::ImprovementCountList::Two)
        .def_readwrite("Pairs", &ShantenCalculator::ImprovementCountList::TwoImprovers)
        .def("__repr__",
            [](const ShantenCalculator::ImprovementCountList& a) {
                std::stringstream fmt;
                fmt << "ImprovementCount(Neg=" << a.Neg << ", Zero=" << a.Zero << ", One=" << a.One << ", Two=" << a.Two << ")";
                return fmt.str();
            });
    py::class_<ShantenCalculator>(m, "ShantenCalculator")
        .def(py::init())
        .def("GetTwoStepCounts", &ShantenCalculator::CalculateTwoStep)
        .def("GetTwoShantenCounts", &ShantenCalculator::GetTwoShantenCounts)
        .def("GetOneShantenCounts", &ShantenCalculator::GetOneShantenCounts)
        .def("CalculateTwoStepList", &ShantenCalculator::CalculateTwoStepList);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}