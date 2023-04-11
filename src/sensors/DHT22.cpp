#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

//#include <pybind11/pybind11.h>
#include <random>
#include <chrono>

class DHT22 {
private:
    double temperature;
    double humidity;

    double capTemperature(double value) {
        // begrens de temperatuur (gebaseerd op datasheet)
        if (value < -40.0) {
            return -40.0;
        }
        else if (value > 80.0) {
            return 80.0;
        }
        else {
            return value;
        }
    }

    double capHumidity(double value) {
        // begrens de vochtigheidswaarde (gebaseerd op datasheet)
        if (value < 0.0) {
            return 0.0;
        }
        else if (value > 100.0) {
            return 100.0;
        }
        else {
            return value;
        }
    }

    double addNoise(double val, char measurement) {
    double noisedVal = 0;
    double noise = 0;
    const double TEMP_ACCURACY = 0.1;
    const double HUM_ACCURACY = 0.5;

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

    if (measurement == 't') {
        std::uniform_real_distribution<double> distribution(-TEMP_ACCURACY, TEMP_ACCURACY);
        noise = distribution(generator);
    }
    else {
        std::uniform_real_distribution<double> distribution(-HUM_ACCURACY, HUM_ACCURACY);
        noise = distribution(generator);
    }
    // normale ruis
    noisedVal = val + noise;

    // voorkom dat ruis buiten de mogelijke range valt
    if (measurement == 't') {
        noisedVal = capTemperature(noisedVal);
    }
    else {
        noisedVal = capHumidity(noisedVal);
    }

    return noisedVal;
    }
public:
    DHT22(double temp = 0.0, double hum = 0.0) {
        temperature = capTemperature(temp);
        humidity = capHumidity(hum);
    }

    double readTemperature() {
        return addNoise(temperature, 't');
    }

    double readHumidity() {
        return addNoise(humidity, 'h');
    }

    void setTemperature(double temp) {
        temperature = temp;
    }

    void setHumidity(double hum) {
        humidity = hum;
    }
};

// Referenties:
// datasheet van DHT22: https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf
// nauwkeurigheid temp = 0.1, luchtvocht. = 0.5

// namespace py = pybind11;

// PYBIND11_MODULE(dht22_module, m) {
//    m.doc() = "DHT22-sensorclass";
//    py::class_<DHT22>(m, "DHT22")
//        .def(py::init<double, double>())
//        .def("readTemperature",  &DHT22::readTemperature)
//        .def("readHumidity",     &DHT22::readHumidity)
//        .def("setTemperature",   &DHT22::setTemperature)
//        .def("setHumidity",      &DHT22::setHumidity);
// }

TEST_CASE("Ruis wordt toegepast") {
    DHT22 sensor(10.0, 60.0);

    REQUIRE(sensor.readTemperature() != 10.0); // met ruis dus NIET hetzelfde
    REQUIRE(sensor.readTemperature() != 60.0);
}

TEST_CASE("De luchtvochtigheid wordt beperkt tot het opgegeven bereik") {
    DHT22 sensor;
    sensor.setHumidity(-123.0);
    REQUIRE(sensor.readHumidity() == 0.0);

    sensor.setHumidity(9999.0);
    REQUIRE(sensor.readHumidity() == 100.0);
}