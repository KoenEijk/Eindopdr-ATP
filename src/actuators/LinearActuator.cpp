//#include <pybind11/pybind11.h>
#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include <iostream>
#include <sstream>

class LinearActuator {
private:
    const double minPositionInch = 1.0;
    const double maxPositionInch = 40.0;
    double positionInch;

    double cmToInch(double centimeter) {
        const double cmToInchConstant = 0.3937;
        return centimeter * cmToInchConstant;
    }

    double inchToCm(double inch) {
        const double inchToCmConstant = 2.54;
        return inch * inchToCmConstant;
    }
public:
    LinearActuator() {
        positionInch = minPositionInch;
    }

    void moveCM(double distanceCM) {
        if (cmToInch(distanceCM) < 1.0 || cmToInch(distanceCM) > 40.0) {
        std::stringstream errorMessage;
            errorMessage << "De afstand in centimeter moet tussen"
                << cmToInch(minPositionInch)
            << " en "
                << cmToInch(maxPositionInch)
            << " liggen";
            throw std::invalid_argument(errorMessage.str());
        }
        else {
            positionInch = cmToInch(distanceCM);
        }
    }

    void moveInch(double distanceInch) {
        if (distanceInch < 1.0 || distanceInch > 40.0) {
        std::stringstream errorMessage;
            errorMessage << "De afstand in inches moet tussen"
                << minPositionInch
                << " en "
                << maxPositionInch
                << " liggen";
            throw std::invalid_argument(errorMessage.str());
        }
        else {
            positionInch = distanceInch;
        }
    }

    double getPositionCM() {
        return inchToCm(positionInch);
    }

    double getPositionInch() {
        return positionInch;
    }
};

// Referenties
// https://components101.com/articles/types-of-actuators
// https://f.hubspotusercontent40.net/hubfs/7717445/PDF%20Manuals/Actuator%20datasheets/PA-14%20datasheet.pdf

// namespace py = pybind11;

//    PYBIND11_MODULE(linearActuator_module, m) {
//    m.doc() = "LinearActuatorclass";
//    py::class_<LinearActuator>(m, "LinearActuator")
//        .def(py::init<>())
//        .def("moveCM",           &LinearActuator::moveCM)
//        .def("moveInch",         &LinearActuator::moveInch)
//        .def("getPositionCM",    &LinearActuator::getPositionCM)
//        .def("getPositionInch",  &LinearActuator::getPositionInch);
// }

TEST_CASE("LinearActuator moveCM", "[LinearActuator]") {
    LinearActuator actuator;

    SECTION("binnen toegestaan bereik") {
        actuator.moveCM(10.0);
        REQUIRE(actuator.getPositionCM() == 10.0);
    }

    SECTION("buiten toegestane bereik") {
        REQUIRE_THROWS_AS(actuator.moveCM(0.33), std::invalid_argument);
        REQUIRE_THROWS_AS(actuator.moveCM(50.0), std::invalid_argument);
    }
}

TEST_CASE("LinearActuator moveInch", "[LinearActuator]") {
    LinearActuator actuator;

    SECTION("binnen toegestaan bereik") {
        actuator.moveInch(10.0);
        REQUIRE(actuator.getPositionInch() == 10.0);
    }

    SECTION("buiten toegestane bereik") {
        REQUIRE_THROWS_AS(actuator.moveInch(0.5), std::invalid_argument);
        REQUIRE_THROWS_AS(actuator.moveInch(50.0), std::invalid_argument);
    }
}

TEST_CASE("LinearActuator getPositionCM", "[LinearActuator]") {
    LinearActuator actuator;
    actuator.moveInch(20.0);

    SECTION("inch naar cm") {
        REQUIRE(actuator.getPositionCM() == 50.8);
        // referentie: https://www.wolframalpha.com/
    }
}