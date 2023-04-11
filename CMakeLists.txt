cmake_minimum_required(VERSION 3.4...3.18)
project(simulatie)

add_subdirectory(pybind11)

# sensors
pybind11_add_module(sen0308 src/sensors/SEN0308.cpp)
pybind11_add_module(dht22_module src/sensors/DHT22.cpp)

# actuators
pybind11_add_module(linearActuator_module src/actuators/LinearActuator.cpp)
pybind11_add_module(waterPump_module src/actuators/WaterPump.cpp)

# microcontroller module
#pybind11_add_module(sensor src/microcontroller.cpp)

target_compile_definitions(sen0308
                        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

target_compile_definitions(dht22_module
                        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

target_compile_definitions(linearActuator_module
                        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
     
target_compile_definitions(waterPump_module
                        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
