# Power Monitoring System

## 1. Project Overview

This project implements a power monitoring and control system using an Arduino-compatible board. The system is designed to detect a "power break" or "cable cut" event, automatically trigger a mechanical action via a stepper motor, and send an SMS alert to a predefined phone number. It also includes manual controls for the motor.

## 2. Circuit Diagram

The following diagram illustrates the wiring of the components.

**File:** `B080EDB6-CB8E-4BE5-9B1D-683663ACD261_1_201_a.jpeg`

*(This image shows the connection of an Arduino, an A4988 stepper motor driver, a stepper motor, a relay/sensor module, and other components.)*

## 3. System Logic and Operation

The core logic is implemented in the `powermonitoring_2/powermonitoring_2.ino` file. The system has two primary modes of operation:

### a. Automatic "Cable Cut" Detection

- A sensor (connected to `RELAY_SENSOR_PIN 10`) monitors the state of a power line.
- If the sensor signal goes LOW (indicating a "cable cut" or power break), the system automatically:
    1.  Rotates a stepper motor clockwise for 10 full revolutions. This action is likely intended to engage a physical brake or switch.
    2.  Sends an SMS alert: `"ALERT: Power cable cut detected! Auto clockwise rotation activated."`

### b. Manual Control

- **Button 1 (Pin 5):** Rotates the motor clockwise and sends a manual activation SMS.
- **Button 2 (Pin 6):** Rotates the motor anti-clockwise (to reset or disengage the system).
- **Button 3 (Pin 9):** Reserved for future SMS functions.

## 4. SMS Notifications

The system uses a GSM module connected via SoftwareSerial to send alerts. Upon a power break event, it sends a message to the configured phone number.

**Example Notification:**

The screenshot below shows the type of message received from the system.

**File:** `output_sms.jpeg`

*(This image shows repeated "Power Braked" messages on a smartphone.)*

## 5. Core Components

-   **Microcontroller:** Arduino or a compatible board.
-   **Stepper Motor Driver:** A4988
-   **Stepper Motor:** NEMA 17 or similar.
-   **GSM Module:** A SIM800L or similar module for sending SMS.
-   **Sensor:** A relay module or custom sensor circuit to detect the power line status.
-   **Push Buttons:** For manual control.

## 6. Code

The primary code for this project is `powermonitoring_2/powermonitoring_2.ino`. It integrates the stepper motor control, sensor reading, and GSM communication.

The other `.ino` files appear to be for testing and development:
-   `powermonitoring_1.ino`: An earlier version with only button controls.
-   `steper_test.ino`: A basic script to test the stepper motor.
-   `test_1.ino`: A test for relay-based motor control.
