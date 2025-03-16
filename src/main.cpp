#include <Arduino.h>
#include <AccelStepper.h>

// Define pins
#define DIR_PIN 12
#define STEP_PIN 14
#define ENABLE_PIN 13  // Optional

// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Variables to store incoming data
int target_steps = 0;  // Steps received from Python
unsigned long last_received_time = 0;  // Timestamp of last received data
const unsigned long TIMEOUT = 1000;  // Timeout in milliseconds (1 second)

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  Serial.println("Stepper Motor Control Initialized!");

  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(1000);  // Adjust as needed
  stepper.setAcceleration(500);  // Adjust as needed

  // Enable the motor (if using ENABLE_PIN)
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);  // LOW to enable, HIGH to disable
  Serial.println("Motor Enabled.");
}

void loop() {
  // Check for incoming serial data
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read the incoming data
    target_steps = data.toInt();  // Convert the data to an integer
    last_received_time = millis();  // Update the last received time
    Serial.print("Received steps: ");
    Serial.println(target_steps);
  }

  // Stop the motor if no new data is received within the timeout period
  if (millis() - last_received_time > TIMEOUT) {
    target_steps = 0;  // Stop the motor
    Serial.println("No data received. Stopping motor.");
  }

  // Move the motor to the target position
  stepper.move(target_steps);

  // Run the motor
  stepper.run();
}