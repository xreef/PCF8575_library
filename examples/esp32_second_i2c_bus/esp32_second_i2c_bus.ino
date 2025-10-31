/*
 * Example: esp32_second_i2c_bus
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This example demonstrates how to use the PCF8575 I/O expander with an ESP32
 * on a secondary I2C bus. The ESP32 has two I2C controllers, so you can use
 * multiple I2C buses simultaneously.
 *
 * Connections:
 * PCF8575    ----- ESP32
 * A0, A1, A2 ----- GND (for I2C address 0x20)
 * VSS        ----- GND
 * VDD        ----- 5V/3.3V
 * SDA        ----- GPIO 21 (for the secondary I2C bus)
 * SCL        ----- GPIO 22 (for the secondary I2C bus)
 *
 * P0-P7      ----- LEDs (each with a current-limiting resistor)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"  // https://github.com/xreef/PCF8575_library

// The ESP32 has two I2C bus interfaces, we can use the second one by creating a new TwoWire instance.
// Wire (the default) is on I2C bus 0.
// We will create I2Ctwo for the second bus (I2C bus 1).
TwoWire I2Ctwo = TwoWire(1);

// I2C Address of the PCF8575
const uint8_t i2cAddress = 0x20;

// Initialize the PCF8575 library, providing the custom TwoWire instance and the I2C address.
// You can also specify the SDA and SCL pins for the secondary I2C bus.
PCF8575 pcf8575(&I2Ctwo, i2cAddress, 21, 22);

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Second I2C Bus Example");

  // Initialize the secondary I2C bus
  I2Ctwo.begin(21, 22, 100000); // SDA pin 21, SCL pin 22, 100kHz frequency

  // Set pins P0 to P7 as OUTPUT
  for (int i = 0; i < 8; i++) {
    pcf8575.pinMode(i, OUTPUT);
  }

  // Initialize the PCF8575
  if (!pcf8575.begin()) {
    Serial.println("PCF8575 not found on the secondary I2C bus");
    while (1);
  }
  Serial.println("PCF8575 initialized.");
}

void loop() {
  static int pin = 0;

  // Blink the LEDs connected to the PCF8575
  pcf8575.digitalWrite(pin, HIGH);
  delay(400);
  pcf8575.digitalWrite(pin, LOW);
  delay(400);

  // Move to the next pin
  pin++;
  if (pin > 7) {
    pin = 0;
  }
}
