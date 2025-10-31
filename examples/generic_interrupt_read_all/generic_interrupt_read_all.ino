/*
 * Example: generic_interrupt_read_all
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This example demonstrates how to use interrupts to read all input pins from a PCF8575 I/O expander.
 * When any of the input pins change state, an interrupt is triggered, and the microcontroller reads all pins.
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
 * P0-P3      ----- Buttons (connected to GND, using INPUT_PULLUP)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"

// On Arduino Uno, pins 2 and 3 are interrupt-capable.
#define ARDUINO_UNO_INTERRUPT_PIN 2

// Flag to indicate that an interrupt has occurred
volatile bool pcf8575_interrupt_flag = false;

// Interrupt Service Routine (ISR)
void onPcf8575Interrupt() {
  // This function is called when the interrupt occurs.
  // It should be kept as short as possible. We just set a flag here.
  pcf8575_interrupt_flag = true;
}

// Set I2C address and specify the interrupt pin and ISR
PCF8575 pcf8575(0x39, ARDUINO_UNO_INTERRUPT_PIN, onPcf8575Interrupt);

void setup() {
  Serial.begin(115200);
  Serial.println("Generic PCF8575 Interrupt Test - Read All");

  // Set pins P0, P1, P2, and P3 to INPUT_PULLUP mode
  pcf8575.pinMode(P0, INPUT_PULLUP);
  pcf8575.pinMode(P1, INPUT_PULLUP);
  pcf8575.pinMode(P2, INPUT_PULLUP);
  pcf8575.pinMode(P3, INPUT_PULLUP);

  // Initialize the PCF8575
  if (!pcf8575.begin()) {
    Serial.println("PCF8575 not found");
    while (1);
  }
  Serial.println("PCF8575 initialized. Waiting for interrupts...");
}

void loop() {
  // Check if the interrupt flag has been set by the ISR
  if (pcf8575_interrupt_flag) {
    Serial.println("Interrupt triggered! Reading all pins:");

    // Read all pin values from the PCF8575
    PCF8575::DigitalInput all_pins = pcf8575.digitalReadAll();

    // Print the status of the pins we are monitoring
    Serial.print("P0: "); Serial.print(all_pins.p0);
    Serial.print(" | P1: "); Serial.print(all_pins.p1);
    Serial.print(" | P2: "); Serial.print(all_pins.p2);
    Serial.print(" | P3: "); Serial.println(all_pins.p3);

    // Reset the flag so we can detect the next interrupt
    pcf8575_interrupt_flag = false;
  }
}
