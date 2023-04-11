

print("CLI-simulator")

def validate_input(value_name, min_value, max_value):
    while True:
        input_value = input(f"Voer {value_name} in ({min_value}-{max_value}): ")
        try:
            input_value = float(input_value)
            if input_value >= min_value and input_value <= max_value:
                return input_value
        except ValueError:
            pass
        print(f"Ongeldige invoer, {value_name} moet tussen {min_value} en {max_value} liggen.")

# de setpunten
print("Stel de setpunten in")
temp_setpoint       = validate_input("setpunt bodemvochtgehalte", 0, 1023)
soil_moist_setpoint = validate_input("setpunt luchtvochtigheid", 0.0, 100.0)
hum_setpoint        = validate_input("setpunt temperatuur", -40.0, 80.0)

# de 'huidige' waarden
print("Initialiseer de sensorwaarden")
soil_most   = validate_input("het bodemvochtgehalte", 0, 1023)
hum         = validate_input("de luchtvochtigheid", 0.0, 100.0)
temp        = validate_input("de temperatuur", -40.0, 80.0)

print("======SIMULATIE======")
print(f"Setpunten: Bodemvochtigheid: {soil_most} | Vochtigheid: {hum} | Temperatuur: {temp}")
print(f"Huidige waarden: Bodemvochtigheid: {soil_most} | Vochtigheid: {hum} | Temperatuur: {temp}")

# helaas niet meer aan toe gekomen