#include <pybind11/pybind11.h>
#include "DHT22.hpp"
#include "SEN0308.hpp"

namespace py = pybind11;

PYBIND11_MODULE(sensor, m) {
    m.doc() = "sensorklassen";
    py::class_<DHT22>(m, "DHT22")
        .def(py::init<int, double, double>())
        .def("readTemperature", &DHT22::readTemperature)
        .def("readHumidity",    &DHT22::readHumidity)
        .def("setTemperature",  &DHT22::setTemperature)
        .def("setHumidity",     &DHT22::setHumidity);

    // py::class_<SEN0308>(m, "SEN0308")
    //      .def(py::init<int>())
    //      .def("setAnalogValue", &SEN0308::setAnalogValue)
    //      .def("read",           &SEN0308::read);
}