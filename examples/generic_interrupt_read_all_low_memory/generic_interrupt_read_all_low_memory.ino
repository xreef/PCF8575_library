/*
 * Example: generic_interrupt_read_all_low_memory
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This example demonstrates the use of interrupts in low memory mode.
 * To enable low memory mode, you must uncomment `#define PCF8575_LOW_MEMORY` in the `PCF8575.h` file.
 * In this mode, `digitalReadAll()` returns a 16-bit integer instead of a struct, saving RAM.
 *
 * Connections:
 * PCF8575    ----- Arduino Uno
 * A0, A1, A2 ----- GND (for I2C address 0x20)
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

// To use this example, you must enable low memory mode in the library by uncommenting:
// #define PCF8575_LOW_MEMORY
// in the PCF8575.h file.

// On Arduino Uno, pins 2 and 3 are interrupt-capable.
#define ARDUINO_UNO_INTERRUPT_PIN 2

// Flag to indicate that an interrupt has occurred
volatile bool interrupt_flag = false;

// Interrupt Service Routine (ISR)
void onInterrupt() {
  interrupt_flag = true;
}

// Set I2C address and specify the interrupt pin and ISR
PCF8575 pcf8575(0x20, ARDUINO_UNO_INTERRUPT_PIN, onInterrupt);

void setup() {
  Serial.begin(115200);
  Serial.println("Generic PCF8575 Interrupt Test - Low Memory Mode");

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
  if (interrupt_flag) {
    Serial.println("Interrupt triggered! Reading all pins (low memory mode):");

    // In low memory mode, digitalReadAll() returns a 16-bit integer
    uint16_t all_pins_value = pcf8575.digitalReadAll();

    Serial.print("Value (binary): ");
    Serial.println(all_pins_value, BIN);

    // To check a specific pin, you can use bitwise operations
    // Note that with INPUT_PULLUP, a pressed button will be LOW (0)
    if (!(all_pins_value & (1 << P0))) { // Check if bit for P0 is 0
        Serial.println("Button on P0 is pressed.");
    }
    if (!(all_pins_value & (1 << P1))) { // Check if bit for P1 is 0
        Serial.println("Button on P1 is pressed.");
    }

    interrupt_flag = false;
  }
}
