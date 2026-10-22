/*
  Project Title: Smart Cane
  Board: Arduino UNO R4 WiFi

  Components:
  - VL53L0X Time of Flight Sensor
  - HC-SR04 Ultrasonic Sensor
  - Active Buzzer Module
  - Vibration Motor Module
  - Water Level Sensor

  Functions:
  - Detect nearby obstacles using two distance sensors
  - Detect water using an analog water sensor
  - Activate buzzer and vibration motor when a hazard is detected
  - Display sensor readings in the Serial Monitor

  Required library:
  - Adafruit VL53L0X (install through Arduino IDE Library Manager)
*/

#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// ---------------- PIN CONFIGURATION ----------------

const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int BUZZER_PIN = 5;
const int MOTOR_PIN = 6;
const int WATER_PIN = A0;

// ---------------- DETECTION SETTINGS ----------------

// Obstacle detection thresholds
const int TOF_THRESHOLD_MM = 800;  // 80 cm
const int ULTRASONIC_THRESHOLD_CM = 80;

// Water sensor threshold; calibrate for your actual sensor.
const int WATER_THRESHOLD = 500;

// HC-SR04 settings
const unsigned long ECHO_TIMEOUT_US = 20000UL;

// ---------------- SENSOR OBJECT ----------------

Adafruit_VL53L0X tofSensor;

// ---------------- SENSOR DATA ----------------

int tofDistance = -1;
int ultrasonicDistance = -1;
int waterValue = 0;

bool tofAvailable = false;
bool tofObstacle = false;
bool ultrasonicObstacle = false;
bool waterDetected = false;
bool hazardDetected = false;

// ---------------- SETUP ----------------

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);

  analogReadResolution(10);
  Wire.begin();

  Serial.println("================================");
  Serial.println("       SMART CANE SYSTEM");
  Serial.println("     Arduino UNO R4 WiFi");
  Serial.println("================================");

  if (tofSensor.begin()) {
    tofAvailable = true;
    Serial.println("VL53L0X: READY");
  } else {
    tofAvailable = false;
    Serial.println("VL53L0X: NOT DETECTED");
    Serial.println("Check SDA, SCL, power and GND.");
  }

  Serial.println("HC-SR04: READY");
  Serial.println("Water sensor: READY");
  Serial.println("System initialized.");
  Serial.println();
}

// ---------------- VL53L0X READING ----------------

void readToFSensor() {
  tofObstacle = false;
  tofDistance = -1;

  if (!tofAvailable) {
    return;
  }

  VL53L0X_RangingMeasurementData_t measurement;
  tofSensor.rangingTest(&measurement, false);

  // RangeStatus 4 indicates an out-of-range result.
  if (measurement.RangeStatus == 0 &&
      measurement.RangeMilliMeter > 0) {
    tofDistance = measurement.RangeMilliMeter;
    tofObstacle = (tofDistance <= TOF_THRESHOLD_MM);
  }
}

// ---------------- HC-SR04 READING ----------------

int readUltrasonicSensor() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    return -1;
  }

  int distance = duration / 58;

  if (distance < 2 || distance > 350) {
    return -1;
  }

  return distance;
}

// ---------------- WATER SENSOR READING ----------------

void readWaterSensor() {
  waterValue = analogRead(WATER_PIN);

  // Assumes the analog value increases when wet.
  waterDetected = (waterValue >= WATER_THRESHOLD);
}

// ---------------- ALERT CONTROL ----------------

void updateAlerts() {
  hazardDetected = tofObstacle || ultrasonicObstacle || waterDetected;

  // Assumes active-HIGH buzzer and vibration modules.
  digitalWrite(BUZZER_PIN, hazardDetected ? HIGH : LOW);
  digitalWrite(MOTOR_PIN, hazardDetected ? HIGH : LOW);
}

// ---------------- SERIAL MONITOR ----------------

void printSensorStatus() {
  Serial.println("------ SMART CANE STATUS ------");

  Serial.print("VL53L0X: ");
  if (!tofAvailable) {
    Serial.println("UNAVAILABLE");
  } else if (tofDistance < 0) {
    Serial.println("No valid reading");
  } else {
    Serial.print(tofDistance);
    Serial.println(" mm");

    Serial.print("ToF obstacle: ");
    Serial.println(tofObstacle ? "YES" : "NO");
  }

  Serial.print("HC-SR04: ");
  if (ultrasonicDistance < 0) {
    Serial.println("No valid echo");
  } else {
    Serial.print(ultrasonicDistance);
    Serial.println(" cm");

    Serial.print("Ultrasonic obstacle: ");
    Serial.println(ultrasonicObstacle ? "YES" : "NO");
  }

  Serial.print("Water sensor value: ");
  Serial.println(waterValue);

  Serial.print("Water detected: ");
  Serial.println(waterDetected ? "YES" : "NO");

  Serial.print("Hazard status: ");
  Serial.println(hazardDetected ? "DETECTED" : "CLEAR");

  Serial.println("-------------------------------");
  Serial.println();
}

// ---------------- MAIN LOOP ----------------

void loop() {
  readToFSensor();

  ultrasonicDistance = readUltrasonicSensor();
  ultrasonicObstacle =
      (ultrasonicDistance >= 0 &&
       ultrasonicDistance <= ULTRASONIC_THRESHOLD_CM);

  readWaterSensor();
  updateAlerts();
  printSensorStatus();

  delay(100);
}
