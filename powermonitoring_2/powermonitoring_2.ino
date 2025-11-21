// Motor Test with Relay Sensor - Manual & Automatic Control
// Button controls + Automatic cable cut detection

// Define pins for the A4988 Stepper Driver
#define STEP_PIN 2   // Connect to STEP pin on A4988
#define DIR_PIN 3    // Connect to DIR pin on A4988
#define ENABLE_PIN 4 // Connect to ENABLE pin on A4988

// Define pins for the GSM module (SoftwareSerial)
#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(7, 8); // RX, TX pins for GSM module

// Define the phone number to send SMS notifications
String phoneNumber = "YOUR_PHONE_NUMBER"; // Replace with your phone number

// Define pins for buttons
#define BUTTON_SMS  9            // Button 1 - Reserved for SMS (future use)
#define BUTTON_CLOCKWISE 5       // Button 2 - Rotate motor clockwise + Send SMS
#define BUTTON_ANTICLOCKWISE 6      // Button 3 - Rotate motor anti-clockwise

// Define pin for relay sensor (cable cut detection)
#define RELAY_SENSOR_PIN 10         // Relay that detects cable cut

// Define motor parameters
const int stepsPerRevolution = 200; // 1.8 degree stepper motors
const int motorSpeed = 1000;        // Microseconds delay (adjust for speed)
const int motorRotations = 10;      // Number of full rotations

// Button debounce variables
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
const unsigned long debounceDelay = 50; // Debounce time in milliseconds

// Button state variables
int lastButtonState1 = HIGH;
int lastButtonState2 = HIGH;
int lastButtonState3 = HIGH;

// Relay sensor variables
int previousRelayState = -1; // -1 for initial state
int lastRelayState = HIGH;
unsigned long lastRelayDebounceTime = 0;

void setup() {
  // Set up pin modes for motor driver
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Set up pin modes for buttons with internal pull-up resistors
  pinMode(BUTTON_SMS, INPUT_PULLUP);
  pinMode(BUTTON_CLOCKWISE, INPUT_PULLUP);
  pinMode(BUTTON_ANTICLOCKWISE, INPUT_PULLUP);
  
  // Set up pin mode for relay sensor with internal pull-up resistor
  pinMode(RELAY_SENSOR_PIN, INPUT_PULLUP);
  
  // Enable the stepper motor driver
  digitalWrite(ENABLE_PIN, LOW); // LOW enables the A4988
  
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Start serial communication with the GSM module
  gsmSerial.begin(9600);
  delay(2000); // Wait for the GSM module to initialize
  
  Serial.println("Motor Control System - Manual & Auto Detection");
  Serial.println("Button 1 (Pin 5): Send SMS Alert");
  Serial.println("Button 2 (Pin 6): Rotate Clockwise + Send SMS");
  Serial.println("Button 3 (Pin 7): Rotate Anti-Clockwise");
  Serial.println("Relay (Pin 10): Auto Cable Cut Detection -> Clockwise + SMS");
  Serial.println("System Ready!");
  
  // Initialize GSM module
  initializeGSM();
  
  // Read initial relay state
  delay(100);
  previousRelayState = digitalRead(RELAY_SENSOR_PIN);
  Serial.print("Initial cable state: ");
  Serial.println(previousRelayState == HIGH ? "NORMAL (Connected)" : "CUT (Disconnected)");
  Serial.println();
}

void loop() {
  // Read all button states
  int buttonState1 = digitalRead(BUTTON_SMS);
  int buttonState2 = digitalRead(BUTTON_CLOCKWISE);
  int buttonState3 = digitalRead(BUTTON_ANTICLOCKWISE);
  
  // Read relay sensor state
  int currentRelayState = digitalRead(RELAY_SENSOR_PIN);
  
  // Check Button 1 - SMS (Reserved for future use)
  if (buttonState1 == LOW && lastButtonState1 == HIGH) {
    if ((millis() - lastDebounceTime1) > debounceDelay) {
      Serial.println("Button 1 pressed - SMS function (not implemented in test)");
      lastDebounceTime1 = millis();
    }
  }
  lastButtonState1 = buttonState1;
  
  // Check Button 2 - Rotate Clockwise (Manual) + Send SMS
  if (buttonState2 == LOW && lastButtonState2 == HIGH) {
    if ((millis() - lastDebounceTime2) > debounceDelay) {
      Serial.println("\n=== MANUAL: Button 2 Pressed ===");
      Serial.println("Rotating Clockwise...");
      rotateClockwise(motorRotations);
      Serial.println("Sending SMS notification...");
      sendSms("MANUAL: Clockwise rotation activated via button.");
      Serial.println("=== Clockwise Complete ===\n");
      lastDebounceTime2 = millis();
    }
  }
  lastButtonState2 = buttonState2;
  
  // Check Button 3 - Rotate Anti-Clockwise (Manual)
  if (buttonState3 == LOW && lastButtonState3 == HIGH) {
    if ((millis() - lastDebounceTime3) > debounceDelay) {
      Serial.println("\n=== MANUAL: Button 3 Pressed ===");
      Serial.println("Rotating Anti-Clockwise...");
      rotateAntiClockwise(motorRotations);
      Serial.println("=== Anti-Clockwise Complete ===\n");
      lastDebounceTime3 = millis();
    }
  }
  lastButtonState3 = buttonState3;
  
  // Check Relay Sensor - Automatic Cable Cut Detection
  if (currentRelayState != previousRelayState && previousRelayState != -1) {
    if ((millis() - lastRelayDebounceTime) > debounceDelay) {
      
      Serial.println("\n========================================");
      Serial.println("=== AUTOMATIC: Cable State Changed! ===");
      Serial.print("Relay state: ");
      Serial.println(currentRelayState == HIGH ? "NORMAL (Cable Connected)" : "TRIGGERED (Cable Cut)");
      
      if (currentRelayState == LOW) {
        // Relay triggered - Cable is cut - AUTO CLOCKWISE ROTATION + SMS
        Serial.println("*** ALERT: Cable Cut Detected! ***");
        Serial.println("Auto-triggering CLOCKWISE rotation...");
        rotateClockwise(motorRotations);
        Serial.println("Sending SMS alert...");
        sendSms("ALERT: Power cable cut detected! Auto clockwise rotation activated.");
        Serial.println("=== Auto Clockwise Complete ===");
      } else {
        // Cable reconnected
        Serial.println("Cable reconnected - System back to normal");
      }
      
      Serial.println("========================================\n");
      
      previousRelayState = currentRelayState;
      lastRelayDebounceTime = millis();
    }
  } else if (previousRelayState == -1) {
    // First reading - just update the state without action
    previousRelayState = currentRelayState;
  }
  
  lastRelayState = currentRelayState;
  
  // Small delay to prevent excessive CPU usage
  delay(10);
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

// Function to initialize GSM module
void initializeGSM() {
  Serial.println("Initializing GSM module...");
  gsmSerial.println("AT"); // Check if GSM module is responding
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(1000);
  Serial.println("GSM module initialized.");
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