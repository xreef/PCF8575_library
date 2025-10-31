/*
 * Example: esp8266_sequential_leds_inverted
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This example demonstrates how to control 16 LEDs sequentially with inverted logic.
 * The LEDs are connected with their anode to the PCF8575 pins, so they turn on when the pin is LOW.
 * This setup is common when using NPN transistors to drive the LEDs.
 *
 * Connections:
 * PCF8575    ----- WeMos D1 Mini
 * A0, A1, A2 ----- GND (for I2C address 0x20)
 * VSS        ----- GND
 * VDD        ----- 5V/3.3V
 * SDA        ----- D2 (GPIO4) (with PullUp resistor)
 * SCL        ----- D1 (GPIO5) (with PullUp resistor)
 *
 * P0-P15     ----- Anode of LEDs (cathode to GND via a current-limiting resistor)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"  // https://github.com/xreef/PCF8575_library

// Set i2c address
PCF8575 pcf8575(0x20);

void setup()
{
  Serial.begin(9600);

  // Set all pins to OUTPUT mode
  for(int i=0;i<16;i++) {
    pcf8575.pinMode(i, OUTPUT);
  }

  // Initially, turn all LEDs off by setting the pins HIGH
  for(int i=0;i<16;i++) {
	  pcf8575.digitalWrite(i, HIGH);
  }

  // Initialize the PCF8575
  if (!pcf8575.begin()){
      Serial.println("PCF8575 not found");
      while(1);
  }
}

void loop()
{
  static int pin = 0;

  // Turn the current LED on by setting the pin LOW
  pcf8575.digitalWrite(pin, LOW);
  delay(1000);

  // Turn the current LED off by setting the pin HIGH
  pcf8575.digitalWrite(pin, HIGH);
  delay(1000);

  // Move to the next pin
  pin++;
  if (pin > 15) pin = 0;
}
