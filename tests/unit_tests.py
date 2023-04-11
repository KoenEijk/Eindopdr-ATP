import dht22_module
import linearActuator_module

print("dht22_module")
dht22 = dht22_module.DHT22(20.0, 50.0)
print("readTemperature()", dht22.readTemperature())
print("readHumidity()", dht22.readHumidity())
dht22.setTemperature(4.0) 
print("readHumidity()", dht22.readTemperature())

lin = linearActuator_module.LinearActuator()

print("linearActuator_module")
lin.moveInch(40)
print("getPositionInch()", lin.getPositionInch())
print("getPositionCM()", lin.getPositionCM())
