#include <Arduino.h>
#include <AccelStepper.h>

// Define pins
#define DIR_PIN_HORIZONTAL 12
#define STEP_PIN_HORIZONTAL 14
#define ENABLE_PIN_HORIZONTAL 13  // Optional

#define DIR_PIN_VERTICAL 15
#define STEP_PIN_VERTICAL 2
#define ENABLE_PIN_VERTICAL 0

// Create an AccelStepper object
AccelStepper stepperHorizontal(AccelStepper::DRIVER, STEP_PIN_HORIZONTAL, DIR_PIN_HORIZONTAL);
AccelStepper stepperVertical(AccelStepper::DRIVER, STEP_PIN_VERTICAL, DIR_PIN_VERTICAL);

// Variables to store incoming data
int target_steps_x = 0;  
int target_steps_y = 0; 
unsigned long last_received_time = 0;  // Timestamp of last received data
const unsigned long TIMEOUT = 1000;  // Timeout in milliseconds (1 second)

// Sweep parameters
const int SWEEP_MIN_STEPS = 0;  // Minimum steps for sweep (e.g., 0°)
const int SWEEP_MAX_STEPS = 200;  // Maximum steps for sweep (e.g., 180°)

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  Serial.println("Stepper Motor Control Initialized!");

  // Set the maximum speed and acceleration
  stepperHorizontal.setMaxSpeed(1000);  // Adjust as needed
  stepperHorizontal.setAcceleration(500);  // Adjust as needed

  stepperVertical.setMaxSpeed(1000);
  stepperVertical.setAcceleration(500);

  // Enable the motor (if using ENABLE_PIN)
  pinMode(ENABLE_PIN_HORIZONTAL, OUTPUT);
  digitalWrite(ENABLE_PIN_HORIZONTAL, LOW);  // LOW to enable, HIGH to disable
  Serial.println("Horizontal Motor Enabled");

  pinMode(ENABLE_PIN_VERTICAL, OUTPUT);
  digitalWrite(ENABLE_PIN_VERTICAL, LOW);
  Serial.println("Vertical Motor Enabled");
}

void loop() {
  // Handle serial commands
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    last_received_time = millis();

    if (data.startsWith("xy:")) {
      int commaIdx = data.indexOf(',');
      if (commaIdx != -1) {
        // Get relative movement steps
        int move_x = data.substring(3, commaIdx).toInt();
        int move_y = data.substring(commaIdx + 1).toInt();
        
        // Move relative to current position
        stepperHorizontal.move(move_x);
        stepperVertical.move(move_y);
        
        Serial.print("Moving X:");
        Serial.print(move_x);
        Serial.print(" Y:");
        Serial.println(move_y);
      }
    }
  }

  // Timeout handling - stop motors
  if (millis() - last_received_time > TIMEOUT) {
    stepperHorizontal.stop();
    stepperVertical.stop();
  }

  // Run steppers
  stepperHorizontal.run();
  stepperVertical.run();
}