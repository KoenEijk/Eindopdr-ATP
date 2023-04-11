#include <pybind11/pybind11.h>
#include <random>
#include <chrono>


class SEN0308 {
private:
    const int maxAnalogReading = 1023;
    int sensorValue;

    int addNoise(int val) {
        int noisedVal = 0;
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(-10, 10);
        std::uniform_int_distribution<int> spikeChange(0, 10);
        std::uniform_int_distribution<int> spikeVal(-500, 500);

        int spikeChangeVal = spikeChange(generator);

            if (spikeChangeVal == 1) { // kans van 1 op 10
                // geef een piek (storing)
                int spike = spikeVal(generator);
                noisedVal = val + spike;
            }
            else {
                int noise = distribution(generator);
                // normale ruis
                noisedVal = val + noise;
            }
        // voorkom dat ruis buiten de mogelijke range valt
        if (noisedVal < 0) {
            noisedVal = 0;
        }
        else if (noisedVal > maxAnalogReading) {
            noisedVal = maxAnalogReading;
        }
        return noisedVal;
    }
public:
    SEN0308(int val = 540) { // standaard 540 = lucht
        setAnalogValue(val);
    }

    void setAnalogValue(int val) {
        if (val < 0 || val > maxAnalogReading) {
            throw std::invalid_argument("De analoge waarde moet tussen 0 en 1023 liggen");
        }
        else {
            sensorValue = val;
        }
    }

    int read() {
        return addNoise(sensorValue);
    }
};

// Referenties:
// Datasheet, https://wiki.dfrobot.com/Waterproof_Capacitive_Soil_Moisture_Sensor_SKU_SEN0308
// Info fabrikant,https://wiki.dfrobot.com/Waterproof_Capacitive_Soil_Moisture_Sensor_SKU_SEN0308
// NB: voor de analoge waarde is uitgegaan van de Arduino Uno
// https://cdn.sparkfun.com/assets/resources/4/4/Input_scaling_for_Arduino_analog_readings.pdf

namespace py = pybind11;

   PYBIND11_MODULE(sen0308, m) {
   m.doc() = "SEN0308-sensorclass";
   py::class_<SEN0308>(m, "SEN0308")
       .def(py::init<int>())
       .def("setAnalogValue",   &SEN0308::setAnalogValue)
       .def("read",             &SEN0308::read);
}