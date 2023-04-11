#include <pybind11/pybind11.h>


class WaterPump {
private:
    bool enabled = false;
public:
    WaterPump() {}

    void turnOn() {
        enabled = true;
    }

    void turnOff() {
        enabled = false;
    }

    bool status() {
        return enabled;
    }
};

// Referenties
// https://www.tinytronics.nl/shop/nl/mechanica-en-actuatoren/motoren/pompen/waterpomp-12v

namespace py = pybind11;

PYBIND11_MODULE(waterPump_module, m) {
    m.doc() = "WaterPump-sensorclass";
    py::class_<WaterPump>(m, "WaterPump")
        .def(py::init<>())
        .def("turnOn",  &WaterPump::turnOn)
        .def("turnOff", &WaterPump::turnOff)
        .def("status",  &WaterPump::status);
}