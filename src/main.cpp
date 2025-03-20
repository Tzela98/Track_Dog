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
  Serial.println('Horizontal Motor Enabled');

  pinMode(ENABLE_PIN_VERTICAL, OUTPUT);
  digitalWrite(ENABLE_PIN_VERTICAL, LOW);
  Serial.println('Vertical Motor Enabled');
}

void loop() {
  // Check for incoming serial data
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read the incoming data
    data.trim();  // Remove any extra whitespace or newline characters
    last_received_time = millis();  // Update the last received time

    // Check if the data starts with "x:" or "y:"
    if (data.startsWith("x:")) {
      // Extract x steps
      target_steps_x = data.substring(2).toInt();  // Convert the data to an integer
      Serial.print("Received x steps: ");
      Serial.println(target_steps_x);
    } else if (data.startsWith("y:")) {
      // Extract y steps
      target_steps_y = data.substring(2).toInt();  // Convert the data to an integer
      Serial.print("Received y steps: ");
      Serial.println(target_steps_y);
    } else {
      Serial.println("Invalid data format");
    }
  }

  // Stop the motor if no new data is received within the timeout period
  if (millis() - last_received_time > TIMEOUT) {
    target_steps_x = stepperHorizontal.currentPosition();  // Stay at the current position
    target_steps_y = stepperVertical.currentPosition();
    Serial.println("No data received. Stopping motor.");
  }


  // Move the motor to the target position (absolute positioning)
  stepperHorizontal.runToNewPosition(target_steps_x);
  stepperVertical.runToNewPosition(target_steps_y)
}