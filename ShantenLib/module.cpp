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
        .def_readwrite("Three", &ShantenCalculator::ImprovementCount::Three)
        .def("__repr__",
            [](const ShantenCalculator::ImprovementCount& a) {
                std::stringstream fmt;
                fmt << "ImprovementCount(Neg=" << a.Neg << ", Zero=" << a.Zero << ", One=" << a.One << ", Two=" << a.Two << ", Three=" << a.Three << ")";
                return fmt.str();
            });

    py::class_<ShantenCalculator>(m, "ShantenCalculator")
        .def(py::init())
        .def("GetTwoStepCounts", &ShantenCalculator::CalculateTwoStep)
        .def("GetThreeStepCounts", &ShantenCalculator::CalculateThreeStep)
        .def("GetThreeStepOnlyCounts", &ShantenCalculator::CalculateThreeStepOnly)
        .def("GetOneShantenCounts", &ShantenCalculator::GetOneShantenCounts)
        .def("GetShanten", &ShantenCalculator::CalculateShanten);
        

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}