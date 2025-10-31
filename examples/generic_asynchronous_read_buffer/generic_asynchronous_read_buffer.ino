/*
 * Example: generic_asynchronous_read_buffer
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Description:
 * This example demonstrates the asynchronous reading capabilities of the PCF8575 library.
 * The `readBuffer()` function is called periodically to update an internal buffer with the pin states.
 * The main loop can then read from this buffer without directly accessing the I2C bus every time,
 * which can be more efficient in some scenarios.
 *
 * Connections:
 * PCF8575    ----- Microcontroller
 * A0, A1, A2 ----- GND (for I2C address 0x20)
 * VSS        ----- GND
 * VDD        ----- 5V/3.3V
 * SDA        ----- SDA pin of your microcontroller (e.g., A4 on Arduino Uno)
 * SCL        ----- SCL pin of your microcontroller (e.g., A5 on Arduino Uno)
 *
 * P1         ----- Button (connected to VCC, using INPUT mode)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"

// Set the I2C address of the PCF8575
PCF8575 pcf8575(0x20);

unsigned long last_check_time = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Generic Asynchronous Read Buffer Example");

  // Set pin P1 to INPUT mode. The button should be connected to VCC.
  pcf8575.pinMode(P1, INPUT);

  // Initialize the PCF8575
  if (!pcf8575.begin()) {
    Serial.println("PCF8575 not found");
    while (1);
  }
  Serial.println("PCF8575 initialized.");

  last_check_time = millis();
}

void loop() {
  // Periodically call readBuffer() to update the internal state of the pins.
  // This is a non-blocking call that reads from the I2C device.
  pcf8575.readBuffer();

  // Every 2 seconds, we check the state of the button from the internal buffer.
  if (millis() > last_check_time + 2000) {
    // digitalRead() will now read from the buffer that readBuffer() updated.
    // This is much faster than a direct I2C read.
    uint8_t val = pcf8575.digitalRead(P1);

    if (val == HIGH) {
      Serial.println("Button on P1 was pressed (read from buffer).");
    }

    last_check_time = millis();
  }

  // Your main loop can continue doing other tasks here without being blocked by I2C reads.
}
