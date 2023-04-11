#include <pybind11/pybind11.h>

#include <vector>
#include <iostream>
#include <string>

// Er zijn 20 pins, nummer 0 t/m 19
class Pin {
private:
    int pinNumber = 0;
    bool pinMode  = false;
    double value  = 0.0;
    static std::vector<int> activePins;
public:
    Pin(int pinNumber, bool pinMode) {
        // check of pin binnen de range valt
        if ( pinNumber < 0 || pinNumber > 19 ) {
            throw std::invalid_argument("Pin moet tussen 0 en 19 vallen");
        }
        // check of pin al bestaat
        for( int pin : activePins ) {
            if ( pin == pinNumber ) {
                std:stringstream errorMessage;
                errorMessage << "Pin " << pinNumber << " is al in gebruik");
                throw std::invalid_argument(errorMessage);
            }
        }
        pinNumber = pinNumber;
        pinMode   = pinMode;
        acttivePins.push_back(pinNumber);
    }

    double read() {
        return value;
    }

    double write(double value) {
        value = value;
    }

    void disconnect() {
        activePins.erase(pinNumber);
        delete this;
    }
};