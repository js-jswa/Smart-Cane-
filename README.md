# Smart Cane Using Arduino UNO R4 WiFi

## Project Overview

The Smart Cane is an assistive mobility prototype designed to detect nearby obstacles and wet conditions using distance sensors and a water sensor. It uses an Arduino UNO R4 WiFi to process sensor readings and activate an audible buzzer and vibration module when a potential hazard is detected.

## Features

- **Obstacle Detection:** Uses the VL53L0X Time of Flight sensor and HC-SR04 ultrasonic sensor.
- **Water Detection:** Monitors analog readings from a water sensor.
- **Audible Alerts:** Activates an active buzzer when a hazard is detected.
- **Vibration Alerts:** Provides tactile feedback through a vibration module.
- **Serial Monitoring:** Displays sensor readings and hazard status for testing and debugging.

## Hardware Requirements

- Arduino UNO R4 WiFi
- VL53L0X Time of Flight sensor
- HC-SR04 ultrasonic sensor
- Active buzzer module
- Vibration motor module with suitable driver
- Analog water sensor
- Jumper wires and breadboard
- USB data cable

## Pin Connections

| Component | Arduino Pin |
|---|---|
| VL53L0X SDA | SDA |
| VL53L0X SCL | SCL |
| HC-SR04 TRIG | D7 |
| HC-SR04 ECHO | D8 |
| Active buzzer SIG | D5 |
| Vibration module SIG | D6 |
| Water sensor AO | A0 |
| Module grounds | GND |

Connect power according to each module's voltage specifications. Use a voltage divider for the HC-SR04 ECHO output if required by the connected input specifications. Do not connect a bare vibration motor directly to an Arduino GPIO pin.

## Software Requirements

- Arduino IDE
- Arduino UNO R4 Boards support package
- Adafruit VL53L0X library

## Installation and Setup

1. Install and open Arduino IDE.
2. Open Library Manager and install **Adafruit VL53L0X**, including any required dependencies.
3. Open `Smart_Cane.ino`.
4. Select **Arduino UNO R4 WiFi** from the board menu.
5. Select the correct serial port.
6. Click **Verify** to compile the sketch.
7. Click **Upload** to upload the program.
8. Open Serial Monitor and set the baud rate to **115200**.

## Configuration and Calibration

The default settings in the sketch are:

- VL53L0X obstacle threshold: 800 mm (80 cm)
- HC-SR04 obstacle threshold: 80 cm
- Water sensor threshold: 500

These values should be calibrated for the actual hardware and mounting position.

### Water Sensor Calibration

1. Observe the sensor's analog reading while dry.
2. Record its reading under the intended wet condition.
3. Select a threshold that separates the dry and wet readings.
4. If the analog value decreases when wet, adjust the comparison in the code accordingly.

The current sketch assumes that the water sensor reading increases when wet.

## Testing Guide

### 1. VL53L0X Sensor

Place an object at different distances in front of the sensor. Confirm that the Serial Monitor displays distance measurements in millimeters.

### 2. HC-SR04 Sensor

Move an object toward and away from the ultrasonic sensor. Confirm that the distance readings in centimeters change accordingly.

### 3. Water Sensor

Compare dry and wet readings. Verify that the configured threshold correctly identifies the wet condition.

### 4. Buzzer and Vibration Module

Trigger an obstacle or water condition. Confirm that both alert modules activate. The current sketch assumes active-HIGH signal control.

### 5. Combined Testing

Test each sensor individually and then test multiple hazards simultaneously. The alerts should remain active while at least one hazard condition is detected.

## Troubleshooting

| Problem | Suggested Solution |
|---|---|
| VL53L0X not detected | Check SDA, SCL, power, ground, and library installation. |
| HC-SR04 has no valid echo | Check TRIG, ECHO, power, ground, and sensor alignment. |
| Water sensor always detects water | Recalibrate the threshold and inspect the sensor for residual moisture. |
| Buzzer does not activate | Check the module's signal polarity, power, and wiring. |
| Vibration motor does not activate | Check the driver circuit, power supply, and module connections. |
| Compilation fails | Confirm that the correct board and Adafruit VL53L0X library are installed. |

## Limitations and Safety

This project is an educational prototype and is not a certified mobility aid. Sensor performance may vary depending on obstacle material, orientation, distance, environmental conditions, and sensor placement.

The water sensor detects conditions at its sensing surface and cannot reliably identify every slippery surface. The current program activates both alerts for any detected hazard and does not provide different alert patterns for different sensors.

The WiFi capability of the Arduino UNO R4 WiFi is not used in the current sketch.

Always supervise prototype testing and do not rely on this system as the user's only means of detecting hazards.

## Project File

- `Smart_Cane.ino` — Contains the sensor reading functions, hazard detection logic, alert control, and Serial Monitor diagnostics.
