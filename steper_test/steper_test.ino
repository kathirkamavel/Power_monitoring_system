// A4988 Stepper Motor Driver Test Code
// This code tests a stepper motor using A4988 driver
// Perfect for beginners to verify motor and driver connections

// Define pins connected to A4988 driver
#define STEP_PIN 2    // Connected to STEP pin on A4988
#define DIR_PIN 3     // Connected to DIR pin on A4988  
#define ENABLE_PIN 4  // Connected to ENABLE pin on A4988 (optional but recommended)

// Motor specifications
const int stepsPerRevolution = 200;  // For 1.8 degree stepper motor
const int motorSpeed = 1000;         // Microseconds delay between steps (adjust for speed)

void setup() {
  // Initialize pins as outputs
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  // Enable the motor driver (LOW = enabled for A4988)
  digitalWrite(ENABLE_PIN, LOW);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== A4988 Stepper Motor Test Started ===");
  Serial.println("Motor will rotate clockwise, then counter-clockwise");
  Serial.println("Check connections if motor doesn't move");
  Serial.println("");
  
  delay(2000); // Wait 2 seconds before starting
}

void loop() {
  // Test 1: Rotate clockwise - 5 revolutions
  Serial.println("🔄 Rotating CLOCKWISE - 5 full revolutions");
  rotateClockwise(5);  // 5 revolutions
  delay(2000);         // Wait 2 seconds
  
  // Test 2: Rotate counter-clockwise - 5 revolutions
  Serial.println("🔄 Rotating COUNTER-CLOCKWISE - 5 full revolutions");
  rotateCounterClockwise(5);  // 5 revolutions
  delay(3000);                // Wait 3 seconds before repeating
  
  Serial.println("--- Test cycle completed, repeating... ---");
  Serial.println("");
}

// Function to rotate motor clockwise
void rotateClockwise(int revolutions) {
  digitalWrite(DIR_PIN, HIGH);  // Set direction to clockwise
  
  Serial.print("  Starting clockwise rotation for ");
  Serial.print(revolutions);
  Serial.println(" revolution(s)...");
  
  // Perform the rotation
  for (int rev = 0; rev < revolutions; rev++) {
    for (int step = 0; step < stepsPerRevolution; step++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(motorSpeed);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(motorSpeed);
    }
    Serial.print("  ✓ Revolution ");
    Serial.print(rev + 1);
    Serial.println(" completed");
  }
  Serial.println("  ✅ Clockwise rotation finished!");
}

// Function to rotate motor counter-clockwise
void rotateCounterClockwise(int revolutions) {
  digitalWrite(DIR_PIN, LOW);   // Set direction to counter-clockwise
  
  Serial.print("  Starting counter-clockwise rotation for ");
  Serial.print(revolutions);  
  Serial.println(" revolution(s)...");
  
  // Perform the rotation
  for (int rev = 0; rev < revolutions; rev++) {
    for (int step = 0; step < stepsPerRevolution; step++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(motorSpeed);
      digitalWrite(STEP_PIN, LOW); 
      delayMicroseconds(motorSpeed);
    }
    Serial.print("  ✓ Revolution ");
    Serial.print(rev + 1);
    Serial.println(" completed");
  }
  Serial.println("  ✅ Counter-clockwise rotation finished!");
}

// Optional: Function to test different speeds
void testDifferentSpeeds() {
  Serial.println("🚀 Testing different motor speeds...");
  
  int speeds[] = {2000, 1500, 1000, 500};  // Different delay values
  String speedNames[] = {"Slow", "Medium", "Fast", "Very Fast"};
  
  for (int i = 0; i < 4; i++) {
    Serial.print("  Testing ");
    Serial.print(speedNames[i]);
    Serial.print(" speed (");
    Serial.print(speeds[i]);
    Serial.println(" μs delay)");
    
    digitalWrite(DIR_PIN, HIGH);  // Clockwise
    
    for (int step = 0; step < stepsPerRevolution; step++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(speeds[i]);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(speeds[i]);
    }
    
    delay(1000);  // Pause between speed tests
  }
  
  Serial.println("  ✅ Speed test completed!");
}

// Optional: Function to disable motor (saves power and reduces heat)
void disableMotor() {
  digitalWrite(ENABLE_PIN, HIGH);  // HIGH = disabled
  Serial.println("🔌 Motor disabled (power saving mode)");
}

// Optional: Function to enable motor  
void enableMotor() {
  digitalWrite(ENABLE_PIN, LOW);   // LOW = enabled
  Serial.println("🔌 Motor enabled");
}