# Project 2: Automated Irrigation Controller

## 1. Project Overview

The **Automated Irrigation Controller** is a microcontroller-based closed-loop control system that automatically activates a water pump when the soil becomes too dry.

The system continuously reads an **analog soil moisture sensor**, converts the sensor signal into a digital value using the microcontroller's **ADC (Analog-to-Digital Converter)**, and compares the measured value with a predefined moisture threshold.

If the soil is too dry, the microcontroller activates a **5V relay module**, which represents the switching mechanism for a water pump. When sufficient moisture is detected, the relay is turned off.

This project demonstrates the basic principles of **sensor-based automation, ADC, threshold logic, relay control, and closed-loop feedback**.

---

## 2. Project Goal

The main goal is to develop an automated irrigation system that makes a decision based on real-time soil moisture measurements.

The system should:

* Read an analog signal from a soil moisture sensor.
* Convert the analog signal using the microcontroller's ADC.
* Compare the moisture value against a predefined threshold.
* Determine whether the soil is too dry.
* Turn a relay ON when irrigation is required.
* Turn the relay OFF when sufficient moisture is detected.
* Continuously repeat the process to create a closed-loop system.

---

## 3. Key Requirements

### Hardware Requirements

| Component               | Quantity | Purpose                              |
| ----------------------- | -------: | ------------------------------------ |
| ESP32 Development Board |        1 | Main controller                      |
| Soil Moisture Sensor    |        1 | Measures soil moisture               |
| 5V Relay Module         |        1 | Switches the pump actuator           |
| DC Water Pump           |        1 | Optional; represents actual actuator |
| External Power Supply   |        1 | Powers pump if required              |
| Breadboard              |        1 | Circuit assembly                     |
| Jumper Wires            |  Several | Electrical connections               |

For simulation, the water pump can be represented by an **LED** or another suitable load.

---

## 4. System Architecture

```text
       ┌────────────────────────┐
       │   Soil Moisture Sensor │
       │                        │
       │     Analog Output      │
       └────────────┬───────────┘
                    │
                    │ Analog Signal
                    ▼
       ┌────────────────────────┐
       │         ESP32          │
       │                        │
       │          ADC           │
       │                        │
       │  Read Moisture Value   │
       │          ↓             │
       │  Compare Threshold     │
       │          ↓             │
       │   Dry / Moist Decision │
       └────────────┬───────────┘
                    │
             Digital Output
                    │
                    ▼
       ┌────────────────────────┐
       │      5V Relay Module   │
       │                        │
       │       ON / OFF         │
       └────────────┬───────────┘
                    │
                    ▼
       ┌────────────────────────┐
       │      Water Pump        │
       │       Actuator         │
       └────────────────────────┘
```

The pump changes the environmental condition, while the sensor continuously measures the soil again. This creates a **closed-loop feedback system**.

---

## 5. ESP32 Pin Configuration

A simple ESP32 configuration is:

| Component            | Pin | ESP32 Pin |
| -------------------- | --- | --------- |
| Soil Moisture Sensor | VCC | 3.3V      |
| Soil Moisture Sensor | GND | GND       |
| Soil Moisture Sensor | AO  | GPIO 34   |
| Relay Module         | VCC | 5V        |
| Relay Module         | GND | GND       |
| Relay Module         | IN  | GPIO 26   |

### Important

GPIO 34 is an **input-only ADC pin** on the ESP32, making it suitable for reading the analog sensor output.

---

## 6. Soil Moisture Sensor

The soil moisture sensor produces an analog voltage related to the moisture level.

The microcontroller reads this signal using its ADC.

Conceptually:

```text
Dry Soil
   │
   ▼
Sensor Output
   │
   ▼
Analog Voltage
   │
   ▼
ESP32 ADC
   │
   ▼
Digital ADC Value
```

The exact relationship between ADC value and moisture percentage depends on the particular sensor and its calibration.

---

## 7. ADC Operation

The ESP32 reads the analog sensor output and converts it into a digital value.

For example, the program may produce values such as:

```text
ADC Value
   │
   ├── 3000 → Dry
   ├── 2500 → Moderately Dry
   ├── 1800 → Moist
   └── 1000 → Very Moist
```

> The actual values depend on the sensor, wiring, ESP32 ADC configuration, and calibration. Therefore, the threshold should be determined experimentally rather than assuming these example values are universal.

---

## 8. Threshold Logic

The central decision-making mechanism is a threshold comparison.

For a sensor where **higher ADC values indicate drier soil**, the logic can be represented as:

```text
IF Soil Moisture ADC Value > Threshold
        │
        ▼
      Soil Dry
        │
        ▼
     Relay ON
        │
        ▼
    Pump ON

ELSE
        │
        ▼
     Soil Moist
        │
        ▼
     Relay OFF
        │
        ▼
     Pump OFF
```

For example:

```cpp
if (soilMoisture > threshold) {
    digitalWrite(RELAY_PIN, HIGH);
}
else {
    digitalWrite(RELAY_PIN, LOW);
}
```

The comparison direction must be changed if the selected sensor produces higher values for wetter soil.

---

## 9. Closed-Loop Feedback

The project is a closed-loop system because the controller does not simply turn the pump on once.

Instead, it continuously performs:

```text
Measure
   ↓
Compare
   ↓
Decide
   ↓
Actuate
   ↓
Environment Changes
   ↓
Measure Again
   ↓
Repeat
```

For irrigation:

```text
Soil becomes dry
       ↓
Sensor detects dryness
       ↓
ESP32 compares value
       ↓
Relay turns ON
       ↓
Pump operates
       ↓
Soil becomes wetter
       ↓
Sensor detects increased moisture
       ↓
ESP32 turns relay OFF
```

This feedback mechanism is the fundamental principle of automated irrigation.

---

## 10. Relay Module Operation

A relay acts as an electrically controlled switch.

The ESP32 does not directly power the water pump. Instead, it sends a control signal to the relay module.

```text
ESP32 GPIO
     │
     ▼
Relay Control Circuit
     │
     ▼
Relay Switch
     │
     ▼
Water Pump
```

When the relay is activated, its contacts switch the pump circuit.

### Relay States

| Soil Condition | Relay | Pump |
| -------------- | ----- | ---- |
| Too Dry        | ON    | ON   |
| Moist Enough   | OFF   | OFF  |

> Relay modules can be **active HIGH** or **active LOW**. Check the module being used before finalizing the logic.

---

## 11. Basic Arduino/ESP32 Code

```cpp
#define SOIL_PIN 34
#define RELAY_PIN 26

// Adjust this value after testing your sensor
int moistureThreshold = 2500;

void setup() {
  Serial.begin(115200);

  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF initially
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Automated Irrigation Controller");
  Serial.println("--------------------------------");
}

void loop() {

  // Read analog soil moisture value
  int soilMoisture = analogRead(SOIL_PIN);

  Serial.print("Soil Moisture ADC: ");
  Serial.println(soilMoisture);

  // Threshold logic
  if (soilMoisture > moistureThreshold) {

    Serial.println("Soil is DRY");
    Serial.println("Pump: ON");

    digitalWrite(RELAY_PIN, HIGH);

  } else {

    Serial.println("Soil is MOIST");
    Serial.println("Pump: OFF");

    digitalWrite(RELAY_PIN, LOW);
  }

  Serial.println("--------------------------------");

  delay(1000);
}
```

---

## 12. Example Serial Monitor Output

When the soil is dry:

```text
Automated Irrigation Controller
--------------------------------
Soil Moisture ADC: 2870
Soil is DRY
Pump: ON
--------------------------------
```

When the soil becomes sufficiently moist:

```text
Soil Moisture ADC: 1750
Soil is MOIST
Pump: OFF
--------------------------------
```

The system continuously monitors the sensor and changes the relay state according to the threshold.

---

## 13. Threshold Calibration

Threshold calibration is an important part of the project.

First, observe the sensor value when the soil is:

### Dry

Record several readings:

```text
Dry Soil:
2700
2810
2750
2860
```

### Moist

Record several readings:

```text
Moist Soil:
1600
1700
1650
1720
```

A suitable threshold could then be selected between the two ranges, for example:

```text
Threshold = 2200
```

The exact threshold should be determined from your actual sensor readings.

---

## 14. Optional Hysteresis

A simple threshold can cause the relay to rapidly switch ON and OFF when the sensor value is close to the threshold.

A better control system can use **hysteresis**.

For example:

```text
Dry threshold    = 2500
Wet threshold    = 2000
```

Logic:

```text
ADC > 2500
     ↓
Pump ON

ADC < 2000
     ↓
Pump OFF

2000–2500
     ↓
Keep previous state
```

This prevents unnecessary relay switching.

---

## 15. Improved Code with Hysteresis

```cpp
#define SOIL_PIN 34
#define RELAY_PIN 26

int dryThreshold = 2500;
int wetThreshold = 2000;

bool pumpState = false;

void setup() {
  Serial.begin(115200);

  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Automated Irrigation Controller");
  Serial.println("--------------------------------");
}

void loop() {

  int soilMoisture = analogRead(SOIL_PIN);

  Serial.print("Soil Moisture ADC: ");
  Serial.println(soilMoisture);

  if (soilMoisture > dryThreshold) {
    pumpState = true;
  }
  else if (soilMoisture < wetThreshold) {
    pumpState = false;
  }

  if (pumpState) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Soil: DRY");
    Serial.println("Pump: ON");
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Soil: MOIST");
    Serial.println("Pump: OFF");
  }

  Serial.println("--------------------------------");

  delay(1000);
}
```

---

## 16. Testing Procedure

### Test 1: Sensor Reading

Connect the soil moisture sensor and open the Serial Monitor.

Verify that the ADC value changes when the sensor condition changes.

### Test 2: Dry Soil

Place the sensor in dry soil.

The controller should detect a dry condition and activate the relay.

Expected result:

```text
Soil: DRY
Pump: ON
```

### Test 3: Moist Soil

Place the sensor in moist soil.

The controller should detect sufficient moisture and turn the relay off.

Expected result:

```text
Soil: MOIST
Pump: OFF
```

### Test 4: Relay Operation

Observe the relay indicator LED if your relay module has one.

The relay should activate when irrigation is required.

### Test 5: Closed-Loop Operation

Simulate or observe the complete process:

```text
Dry Soil
   ↓
Pump ON
   ↓
Water Applied
   ↓
Soil Moisture Increases
   ↓
Pump OFF
```

---

## 17. Safety Considerations

For the initial prototype, it is recommended to use a **low-voltage DC pump** or an LED to simulate the pump.

Do not connect mains AC directly to a breadboard or microcontroller circuit.

If a real pump is used:

* Use an appropriate external power supply.
* Ensure the relay's voltage and current ratings are suitable for the pump.
* Keep the pump power circuit separate from the ESP32's logic supply where appropriate.
* Use proper insulation and electrical protection.

---

## 18. Key Skills Demonstrated

### Analog-to-Digital Conversion (ADC)

The ESP32 converts the analog soil sensor signal into a digital value that can be processed by software.

### Threshold Logic

The controller uses conditional logic to determine whether irrigation is required.

```cpp
if (soilMoisture > threshold)
```

### Relay Control

A digital GPIO output controls the relay module, allowing the microcontroller to switch an actuator.

### Closed-Loop Control

The system continuously measures the environment and changes its output according to the latest sensor measurement.

### Embedded Programming

The project demonstrates:

* GPIO configuration
* ADC reading
* Conditional statements
* Digital output control
* Serial debugging
* Continuous execution

---

## 19. Expected Outcome

After successful implementation, the Automated Irrigation Controller should:

* Continuously measure soil moisture.
* Convert the analog sensor signal using the ESP32 ADC.
* Compare the reading against a calibrated threshold.
* Detect when the soil is too dry.
* Activate the relay when irrigation is required.
* Deactivate the relay when sufficient moisture is detected.
* Demonstrate a complete closed-loop feedback process.

---

## 20. Possible Future Improvements

The project can be expanded into a complete **IoT Smart Irrigation System** by adding:

* ESP32 Wi-Fi connectivity.
* Real-time cloud monitoring.
* MQTT communication.
* Mobile notifications.
* Web-based dashboard.
* Multiple soil moisture sensors.
* Water-level monitoring.
* Weather API integration.
* Automatic irrigation scheduling.
* Flow-rate measurement.
* Water consumption monitoring.
* Machine-learning-based irrigation prediction.

---

## 21. Conclusion

The **Automated Irrigation Controller** demonstrates how a microcontroller can make an automatic decision using real-time environmental data.

The soil moisture sensor provides the input, the ESP32 performs ADC conversion and threshold evaluation, and the relay provides the actuator control. The resulting system forms a closed feedback loop in which the environmental condition is continuously measured and the irrigation actuator is automatically controlled.

This project establishes the foundation for more advanced **IoT-based smart agriculture and precision irrigation systems**.
