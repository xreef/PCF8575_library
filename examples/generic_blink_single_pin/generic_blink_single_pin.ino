/*
 * Example: generic_blink_single_pin
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 *
 * Tutorial: https://www.mischianti.org/pcf8575-i2c-16-bit-digital-i-o-expander/
 *
 * Description:
 * This is a basic example to demonstrate how to blink a single LED connected to a PCF8575 I/O expander.
 * It's a generic example that can be used with any supported board (Arduino, ESP8266, ESP32, etc.).
 *
 * Connections:
 * PCF8575    ----- Microcontroller
 * A0, A1, A2 ----- GND (for I2C address 0x20)
 * VSS        ----- GND
 * VDD        ----- 5V/3.3V
 * SDA        ----- SDA pin of your microcontroller (e.g., A4 on Arduino Uno)
 * SCL        ----- SCL pin of your microcontroller (e.g., A5 on Arduino Uno)
 *
 * P0         ----- LED (with a current-limiting resistor)
 *
 */

#include "Arduino.h"
#include "PCF8575.h"

// Set i2c address of the PCF8575
PCF8575 pcf8575(0x20);

void setup()
{
	Serial.begin(115200);

	// Set pin P0 to OUTPUT mode
	pcf8575.pinMode(P0, OUTPUT);

    // Initialize the PCF8575
	if (!pcf8575.begin()){
        Serial.println("PCF8575 not found");
        while(1);
    }
}

void loop()
{
    // Turn the LED on (HIGH is the voltage level)
	pcf8575.digitalWrite(P0, HIGH);
	delay(1000); // Wait for a second

    // Turn the LED off by making the voltage LOW
	pcf8575.digitalWrite(P0, LOW);
	delay(1000); // Wait for a second
}
