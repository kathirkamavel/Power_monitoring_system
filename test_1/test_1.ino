// Power Line Monitoring System - Fixed Version
// This code monitors the relay circuit status,
// controls a stepper motor with an A4988 driver, and sends SMS
// notifications via a GSM module.

// Define pins for the A4988 Stepper Driver
#define STEP_PIN 2   // Connect to STEP pin on A4988
#define DIR_PIN 3    // Connect to DIR pin on A4988
#define ENABLE_PIN 4 // Connect to ENABLE pin on A4988 (optional)

// Define pin for the relay module
#define RELAY_PIN 5  // Connect to the control pin of the relay module

// Define pins for the GSM module (SoftwareSerial)
#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(7, 8); // RX, TX pins for GSM module

// Define the phone number to send SMS notifications
String phoneNumber = "+919360454833"; // Replace with your phone number

// Define motor parameters
const int stepsPerRevolution = 200; // 1.8 degree stepper motors
const int motorSpeed = 1000; // Microseconds delay (adjust for speed)

// Variable to track the previous state of the relay circuit
int previousRelayState = -1; // -1 for initial state, 0 for OPEN, 1 for CLOSED

void setup() {
  // Set up pin modes
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT); // Optional: to enable/disable motor
  pinMode(RELAY_PIN, INPUT_PULLUP); // Use internal pull-up resistor
  
  // Enable the stepper motor driver
  digitalWrite(ENABLE_PIN, LOW); // LOW enables the A4988
  
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Start serial communication with the GSM module
  gsmSerial.begin(9600);
  delay(2000); // Wait for the GSM module to initialize
  
  Serial.println("Relay Circuit Monitoring System Initialized");
  
  // Initialize GSM module
  initializeGSM();
  
  // Send startup notification
  sendSms("Relay monitoring system is online.");
}

void loop() {
  // Read the state of the relay circuit
  int currentRelayState = digitalRead(RELAY_PIN);
  
  // Check if the relay circuit state has changed
  if (currentRelayState != previousRelayState && previousRelayState != -1) {
    
    Serial.print("Relay circuit state changed to: ");
    Serial.println(currentRelayState ? "CLOSED" : "OPEN");
    
    if (currentRelayState == HIGH) {
      // Relay circuit is CLOSED (HIGH signal)
      Serial.println("Relay circuit is CLOSED. Rotating anti-clockwise 10 times.");
      rotateAntiClockwise(10);  // 10 full rotations anti-clockwise
      sendSms("ALERT: Relay circuit is CLOSED.");
      
    } else {
      // Relay circuit is OPEN (LOW signal)
      Serial.println("Relay circuit is OPEN. Rotating clockwise 10 times.");
      rotateClockwise(10);      // 10 full rotations clockwise
      sendSms("ALERT: Relay circuit is OPEN.");
    }
    
    // Update the previous state
    previousRelayState = currentRelayState;
    
  } else if (previousRelayState == -1) {
    // First reading - just update the state without action
    previousRelayState = currentRelayState;
    Serial.println("Initial relay circuit state detected: " + String(currentRelayState ? "CLOSED" : "OPEN"));
  }
  
  // Debounce delay
  delay(500);
}

// Function to initialize GSM module
void initializeGSM() {
  Serial.println("Initializing GSM module...");
  gsmSerial.println("AT"); // Check if GSM module is responding
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  Serial.println("GSM module initialized.");
}

// Function to rotate the stepper motor clockwise
void rotateClockwise(int revolutions) {
  digitalWrite(DIR_PIN, HIGH); // Set direction to clockwise
  
  Serial.println("Starting clockwise rotation...");
  for (int j = 0; j < revolutions; j++) {
    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(motorSpeed);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(motorSpeed);
    }
    Serial.print("Revolution ");
    Serial.print(j + 1);
    Serial.print(" of ");
    Serial.print(revolutions);
    Serial.println(" completed");
  }
  Serial.println("Clockwise rotation completed - 10 full turns done!");
}

// Function to rotate the stepper motor anti-clockwise
void rotateAntiClockwise(int revolutions) {
  digitalWrite(DIR_PIN, LOW); // Set direction to anti-clockwise
  
  Serial.println("Starting anti-clockwise rotation...");
  for (int j = 0; j < revolutions; j++) {
    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(motorSpeed);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(motorSpeed);
    }
    Serial.print("Revolution ");
    Serial.print(j + 1);
    Serial.print(" of ");
    Serial.print(revolutions);
    Serial.println(" completed");
  }
  Serial.println("Anti-clockwise rotation completed - 10 full turns done!");
}

// Function to send an SMS using the GSM module
void sendSms(String message) {
  Serial.println("Sending SMS...");
  
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);
  
  gsmSerial.print(message + " Time: ");
  gsmSerial.print(millis() / 1000); // Add timestamp
  gsmSerial.println(" seconds");
  delay(500);
  
  gsmSerial.println((char)26);   // Send CTRL+Z to terminate
  delay(3000);
  
  Serial.println("SMS sent successfully.");
}