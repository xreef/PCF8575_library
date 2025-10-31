/*
 * Example: generic_interrupt_single_button
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This example demonstrates how to use an interrupt to read a single button press from a PCF8575.
 * When the button is pressed, it triggers an interrupt, and the microcontroller reads the specific pin.
 * This example is for an Arduino Uno, but can be adapted for other microcontrollers.
 *
 * Connections:
 * PCF8575    ----- Arduino Uno
 * A0, A1, A2 ----- GND (for I2C address 0x20, but here we use 0x39 as an example)
 * VSS        ----- GND
 * VDD        ----- 5V
 * SDA        ----- A4
 * SCL        ----- A5
 * INT        ----- D2 (one of the interrupt-capable pins on Arduino Uno)
 *
 * P1         ----- Button (connected to GND, using INPUT_PULLUP)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"

// On Arduino Uno, pins 2 and 3 are interrupt-capable.
#define ARDUINO_UNO_INTERRUPT_PIN 2

// Flag to indicate that an interrupt has occurred
volatile bool button_pressed_flag = false;

// Interrupt Service Routine (ISR)
void onButtonPress() {
  // This function is called when the interrupt occurs.
  // It should be kept as short as possible. We just set a flag here.
  button_pressed_flag = true;
}

// Set I2C address and specify the interrupt pin and ISR
PCF8575 pcf8575(0x39, ARDUINO_UNO_INTERRUPT_PIN, onButtonPress);

void setup() {
  Serial.begin(115200);
  Serial.println("Generic PCF8575 Interrupt Test - Single Button");

  // Set pin P1 to INPUT_PULLUP mode
  pcf8575.pinMode(P1, INPUT_PULLUP);

  // Initialize the PCF8575
  if (!pcf8575.begin()) {
    Serial.println("PCF8575 not found");
    while (1);
  }
  Serial.println("PCF8575 initialized. Waiting for button press...");
}

void loop() {
  // Check if the interrupt flag has been set by the ISR
  if (button_pressed_flag) {
    Serial.println("Interrupt triggered! Reading button state:");

    // Read the value of the button on pin P1
    uint8_t button_state = pcf8575.digitalRead(P1);

    // The value will be LOW if the button is pressed
    Serial.print("Button on P1 is: ");
    Serial.println(button_state == LOW ? "Pressed" : "Not Pressed");

    // Reset the flag so we can detect the next interrupt
    button_pressed_flag = false;
  }
}
