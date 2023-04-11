import dht22_module
import sen0308
import linearActuator_module
import waterPump_module
import random
from collections import namedtuple
from statistics import mean, variance, median
from typing import Generator
import math
import itertools
import time

# deze variablen zijn niet FP  
# maar noodzakelijk om het overzichtelijk te houden.
init_bodemvochtigheid = 900
init_temp = 22.9
init_hum = 50.29

def time_exec_logger(f):
    # schrijf runtijd van functie naar timer-log.txt
    def inner(*args, **kwargs):
        start = time.time()
        res   = f(*args, **kwargs)
        end   = time.time()
        with open('timer-log.txt', 'a') as file:
            file.write(f"Functie {f.__name__} werd aangeroepen op {time.strftime('%Y-%m-%d %H:%M')} en duurde {end-start} seconden.\n")
        return res
    return inner

# datastream bodemvochtigheidssensor
def soil_moist_stream(soil_sensor) -> Generator[int, None, None]:
    while True:
        sensor_reading = soil_sensor.read()
        yield sensor_reading

# datastream temperatuur en luchtvochtigheid
def temp_stream(temp_hum_sensor) -> Generator[int, None, None]:
    while True:
        temp     = round(temp_hum_sensor.readTemperature(), 2)
        yield temp

def hum_stream(temp_hum_sensor) -> Generator[int, None, None]:
    while True:
        humidity = round(temp_hum_sensor.readHumidity(), 2)
        yield humidity

def median_filter(values, window_size): # niet functioneel
    window = []
    filtered_values = []
    for value in values:
        window.append(value)
        if len(window) > window_size:
            window.pop(0)
        filtered_values.append(median(window))
    return filtered_values

def average(values):
    def inner(values, acc, counter):
        if not values:
            return acc / counter
        else:
            return inner(values[1:], acc + values[0], counter + 1)

    return inner(values, 0.0, 0)

@time_exec_logger
def control_loop():
    setpoint_soil = 500
    setpoint_temp = 21
    setpoint_hum = 60

    # sensors
    soil_sensor     = sen0308.SEN0308(init_bodemvochtigheid)
    temp_hum_sensor = dht22_module.DHT22(init_temp, init_hum)

    # actuators
    water_pump      = waterPump_module.WaterPump()
    window_actuator = linearActuator_module.LinearActuator()

    # data streams
    soil_moist_values   = itertools.islice(soil_moist_stream(soil_sensor), 10) # verkrijg de laatste 10 metingen
    filtered_soil_moist = median_filter(soil_moist_values, window_size=5) # pas filter toe (om pieken te filteren)
    average_soil_moist   = average(filtered_soil_moist) # bereken gemiddelde van gefilterde metingen

    temp_values   = itertools.islice(temp_stream(temp_hum_sensor), 10) 
    filtered_temp_values = median_filter(temp_values, window_size=5)
    average_temp_value   = average(filtered_temp_values) 
    
    hum_values   = itertools.islice(hum_stream(temp_hum_sensor), 10) 
    filtered_hum_values = median_filter(hum_values, window_size=5)
    average_hum_value   = average(filtered_hum_values) 

    # regeling van actuatoren obv sensorwaarden en setpoints

    # waterpomp
    if (average_soil_moist < setpoint_soil):
        water_pump.turnOn()
        time.sleep(1) # wacht één seconde
        new_soil_most_val = average_soil_moist + random.uniform(10, 50)
        soil_sensor.setAnalogValue(int(new_soil_most_val))  # simulatie van effect waterpomp
        water_pump.turnOff()    

    # raam 
    if average_temp_value > setpoint_temp:
        window_actuator.moveInch(40) # raam open
    elif average_hum_value > setpoint_hum:
        window_actuator.moveInch(1) # raam dicht
            
    # =================================================
    # deze code is puur voor de simulatie van verhoging/verlaging sensorwaarden 
    # (daarom niet geheel functioneel)
    new_temp        = 0
    new_hum         = 0
    new_soil_moist  = 0

    if (window_actuator.getPositionInch() > 1): # raam is open
        print("raam is open") # < voor de integratietest
        # dus:
        # temp omlaag
        # hum omlaag
        # bovemvochtigheid langzaam omlaag
        new_temp        = temp_hum_sensor.readTemperature() + random.uniform(0.5, 1.1)
        new_hum         = temp_hum_sensor.readHumidity()    + random.uniform(1, 5)
        new_soil_moist  = soil_sensor.read() - random.uniform(10, 40)

        temp_hum_sensor.setTemperature(new_temp)
        temp_hum_sensor.setHumidity(new_hum)
        soil_sensor.setAnalogValue(int(new_soil_moist))
    elif (window_actuator.getPositionInch() <= 1): # raam is dicht
        print("raam is dicht") # < voor de integratietest
        # dus:
        # temp omhoog
        # hum omhoog
        # bovemvochtigheid sneller (tov gesloten raam) omlaag
        new_temp        = temp_hum_sensor.readTemperature() - random.uniform(0.5, 1.1)
        new_hum         = temp_hum_sensor.readHumidity()    - random.uniform(1, 5)
        new_soil_moist  = soil_sensor.read() - random.uniform(25, 60)

        temp_hum_sensor.setTemperature(new_temp)
        temp_hum_sensor.setHumidity(new_hum)
        soil_sensor.setAnalogValue(int(new_soil_moist))
    
    # voor de blackboxtest
    print(f"sensorwaarden: soil_sensor: {soil_sensor.read()}, temperatuursensor: {round(temp_hum_sensor.readTemperature(),2)}, luchtvochtigheidssensor: {round(temp_hum_sensor.readHumidity(),2)}")

    # instelbare snelheid om het overzicht te bewaren tijdens het testen
    milliseconds = 1000
    seconds = milliseconds / 1000
    time.sleep(seconds)

while True:
    control_loop()