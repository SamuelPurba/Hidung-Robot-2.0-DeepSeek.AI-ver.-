#define MQ2_PIN A0

#include <Wire.h>

// Sensor calibration constants
const float RL = 10.0;     // Load resistance in kΩ
const float RO_CLEAN_AIR = 9.83; // Ro in clean air

// Aroma detection thresholds
const struct AromaThreshold {
  const char* name;
  int minValue;
  int maxValue;
} AROMA_THRESHOLDS[] = {
  {"Kopi", 250, 300},
  {"Teh", 200, 250},
  {"Daging Sapi", 450, 500},
  {"Apel", 200, 250},
  {"Brokoli", 200, 250}
};

const int NUM_AROMAS = sizeof(AROMA_THRESHOLDS) / sizeof(AROMA_THRESHOLDS[0]);

// Sensor variables
int sensorValue = 0;
float sensorVoltage = 0;
float rsGas = 0;
float ratio = 0;

void detectAromas(int value) {
  bool aromaDetected = false;
  
  for (int i = 0; i < NUM_AROMAS; i++) {
    if (value >= AROMA_THRESHOLDS[i].minValue && value <= AROMA_THRESHOLDS[i].maxValue) {
      Serial.print("Aroma Terdeteksi: ");
      Serial.println(AROMA_THRESHOLDS[i].name);
      aromaDetected = true;
    }
  }
  
  if (!aromaDetected) {
    Serial.println("Tidak ada aroma yang dikenali");
  }
}

void readSensor() {
  sensorValue = analogRead(MQ2_PIN);
  sensorVoltage = sensorValue * (5.0 / 1023.0);
  rsGas = (5.0 - sensorVoltage) / sensorVoltage;
  ratio = rsGas / RO_CLEAN_AIR;
}

void printSensorData() {
  Serial.println("=== MQ2 Sensor Data ===");
  Serial.print("Raw Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(sensorVoltage, 2);
  Serial.print("V | Rs: ");
  Serial.print(rsGas, 2);
  Serial.print("kΩ | Ratio: ");
  Serial.println(ratio, 2);
  
  Serial.print("Mapped Value: ");
  int mappedValue = map(sensorValue, 0, 1023, 0, 500);
  Serial.println(mappedValue);
  
  Serial.println("------------------------");
}

void setup() {
  Serial.begin(115200);
  pinMode(MQ2_PIN, INPUT);
  
  Serial.println("MQ2 Gas Sensor Initialized");
  Serial.println("Monitoring started...");
  Serial.println();
  
  // Warm-up delay for sensor stabilization
  delay(2000);
}

void loop() {
  readSensor();
  
  Serial.println();
  printSensorData();
  
  int mappedValue = map(sensorValue, 0, 1023, 0, 500);
  detectAromas(mappedValue);
  
  delay(1000); // Read every second
}