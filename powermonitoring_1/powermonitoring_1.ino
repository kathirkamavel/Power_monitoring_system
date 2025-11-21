// Power Line Monitoring System - Button Control Version
// This code controls a stepper motor with buttons and sends SMS
// notifications via a GSM module.

// Define pins for the A4988 Stepper Driver
#define STEP_PIN 2   // Connect to STEP pin on A4988
#define DIR_PIN 3    // Connect to DIR pin on A4988
#define ENABLE_PIN 4 // Connect to ENABLE pin on A4988

// Define pins for buttons
#define BUTTON_SMS 5     // Button 1 - Send power break SMS
#define BUTTON_CLOCKWISE 6  // Button 2 - Rotate motor clockwise
#define BUTTON_ANTICLOCKWISE 7 // Button 3 - Rotate motor anti-clockwise

// Define pins for the GSM module (SoftwareSerial)
#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(8, 9); // RX, TX pins for GSM module

// Define the phone number to send SMS notifications
String phoneNumber = "YOUR_PHONE_NUMBER"; // Replace with your phone number

// Define motor parameters
const int stepsPerRevolution = 200; // 1.8 degree stepper motors
const int motorSpeed = 1000; // Microseconds delay (adjust for speed)
const int motorRotations = 10; // Number of full rotations

// Button debounce variables
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
const unsigned long debounceDelay = 50; // Debounce time in milliseconds

// Button state variables
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int lastButtonState3 = HIGH;

void setup() {
  // Set up pin modes for motor driver
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Set up pin modes for buttons with internal pull-up resistors
  pinMode(BUTTON_SMS, INPUT_PULLUP);
  pinMode(BUTTON_CLOCKWISE, INPUT_PULLUP);
  pinMode(BUTTON_ANTICLOCKWISE, INPUT_PULLUP);
  
  // Enable the stepper motor driver
  digitalWrite(ENABLE_PIN, LOW); // LOW enables the A4988
  
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Start serial communication with the GSM module
  gsmSerial.begin(9600);
  delay(2000); // Wait for the GSM module to initialize
  
  Serial.println("Button-Controlled Power Monitoring System Initialized");
  Serial.println("Button 1 (Pin 5): Send Power Break SMS");
  Serial.println("Button 2 (Pin 6): Rotate Clockwise");
  Serial.println("Button 3 (Pin 7): Rotate Anti-Clockwise");
  
  // Initialize GSM module
  initializeGSM();
  
  // Send startup notification
  sendSms("Power monitoring system is online and ready.");
}

void loop() {
  // Read button states
  int buttonState1 = digitalRead(BUTTON_SMS);
  int buttonState2 = digitalRead(BUTTON_CLOCKWISE);
  int buttonState3 = digitalRead(BUTTON_ANTICLOCKWISE);
  
  // Check Button 1 - Send Power Break SMS
  if (buttonState1 == LOW && lastButtonState1 == HIGH) {
    if ((millis() - lastDebounceTime1) > debounceDelay) {
      Serial.println("Button 1 pressed - Sending power break SMS...");
      sendSms("ALERT: Power system is breaked! Immediate attention required.");
      lastDebounceTime1 = millis();
    }
  }
  lastButtonState1 = buttonState1;
  
  // Check Button 2 - Rotate Clockwise
  if (buttonState2 == LOW && lastButtonState2 == HIGH) {
    if ((millis() - lastDebounceTime2) > debounceDelay) {
      Serial.println("Button 2 pressed - Rotating clockwise...");
      rotateClockwise(motorRotations);
      lastDebounceTime2 = millis();
    }
  }
  lastButtonState2 = buttonState2;
  
  // Check Button 3 - Rotate Anti-Clockwise
  if (buttonState3 == LOW && lastButtonState3 == HIGH) {
    if ((millis() - lastDebounceTime3) > debounceDelay) {
      Serial.println("Button 3 pressed - Rotating anti-clockwise...");
      rotateAntiClockwise(motorRotations);
      lastDebounceTime3 = millis();
    }
  }
  lastButtonState3 = buttonState3;
  
  // Small delay to prevent excessive CPU usage
  delay(10);
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